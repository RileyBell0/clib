#include "../config.h"

//////////////////////////////
// Initialisation
//////////////////////////////

// RE-CHECKED 01/05/2021
config_var_t config_var_new(string_t *var_name) {

  config_var_t var;

  var.values = array_new(0, sizeof(string_t));
  var.var_name = string_copy(var_name);

  return var;
}


/*
 * returns the position in the string the function exited at
 *
 * complexString is set to true iff the returned string was surrounded by quotes
 */// TODO this function is way too long
bool extract_field(string_t *str, unsigned int *pos, string_t *dest,
                  bool *complex_field) {
  string_clear(dest);

  // Why am i even stealing complex field?? if im just chanigng it to false
  *complex_field = false;
  if (str->len <= *pos) {
    return false;
  }

  /*
   * Ignore leading whitespaces and tabs
   */
  char *str_start = cstr(str);
  unsigned int i;
  for (i = *pos; i < str->len; i++) {
    if (str_start[i] != CONFIG_IGNORE_CHAR && str_start[i] != '\t') {
      // Done with leading whitespaces
      break;
    }
  }
  *pos = i;

  /*
   * If we've reached an = as its own field
   */
  if (str_start[*pos] == CONFIG_FIELD_DECLARATION_CHAR) {
    string_write_char(dest, str_start[*pos]);
    string_null_terminate(dest);
    *pos += 1;
    return true;
  }

  /*
   * If we've reached a # as its own field
   */
  if (str_start[*pos] == CONFIG_COMMENT_CHAR) {
    string_write_char(dest, str_start[*pos]);
    string_null_terminate(dest);
    *pos += 1;
    return true;
  }

  /*
   * If we've reached an array declaration square brace
   */
  if (str_start[*pos] == CONFIG_ARRAY_START_CHAR ||
      str_start[*pos] == CONFIG_ARRAY_END_CHAR) {
    string_write_char(dest, str_start[*pos]);
    string_null_terminate(dest);
    *pos += 1;
    return true;
  }

  /*
   * If the current field is within quotes
   */
  bool inQuotes = false;
  if (str_start[*pos] == CONFIG_STRING_ENCLOSE_CHAR) {
    *complex_field = true;
    *pos += 1;
    inQuotes = true;
  }

  if (inQuotes) {
    /*
     * Read until the string is escaped
     */
    for (i = *pos; i < str->len; i++) {
      char c = str_start[i];

      if (c == CONFIG_STRING_ENCLOSE_CHAR) {
        // Move to the next character then exit the loop
        i++;
        break;
      }
      if (c == CONFIG_ESCAPE_CHAR &&
          str_start[i + 1] == CONFIG_STRING_ENCLOSE_CHAR) {
        // reached a quote intended to be within a string
        string_write_char(dest, str_start[i + 1]);
        i++;
      } else {
        string_write_char(dest, c);
      }
    }
  } else {
    /*
     * Read until a whitespace or special char related to
     * the config file is read
     */
    for (i = *pos; i < str->len; i++) {
      char c = str_start[i];

      if (c == CONFIG_IGNORE_CHAR) {
        // Move to the next character then exit the loop
        i++;
        break;
      } else if (c == CONFIG_FIELD_DECLARATION_CHAR ||
                 c == CONFIG_STRING_ENCLOSE_CHAR || c == CONFIG_COMMENT_CHAR ||
                 c == CONFIG_ARRAY_START_CHAR || c == CONFIG_ARRAY_END_CHAR) {
        break;
      } else {
        string_write_char(dest, c);
      }
    }
  }

  // Null terminate the field we've extracted
  string_null_terminate(dest);

  // Return the position in the string of the next unread char
  *pos = i;
  return true;
}

// RE-CHECKED 01/05/2021
config_t read_config_file(char *file_path) {
  // Storing the processed data
  dynamic_array_t results = dynamic_array_new(sizeof(config_var_t));

  // Stores the fields associated with the current variable
  dynamic_array_t fields = dynamic_array_new(sizeof(string_t));
  config_var_t var;
  config_t config;

  // Open the config file
  FILE *config_file = fopen(file_path, MODE_READ);

  // Stores lines read in from the config file
  string_t buffer = string_new(DEFAULT_BUFFER_LEN);

  // Storing a processed and formatted field string ready to be converted into a
  // var
  string_t field = string_new(DEFAULT_BUFFER_LEN);

  /*
   * Read every line in the file
   */
  bool within_array_declaration = false;
  int curr_field_type = CONFIG_FIELD_NAME;
  bool within_quotes = false;
  bool current_var_finalised = true;
  /*
   * Has the current var thats being loaded in got at least one
   * value loaded in from the file
   */
  bool var_has_value = false;

  // Read in and process the whole config file
  while (fileio_next_line(config_file, &buffer)) {
    // Read in a line from the file

    unsigned int pos = 0;
    while (extract_field(&buffer, &pos, &field, &within_quotes)) {
      if (field.len == 0) {
        continue;
      }

      // Process field decleration chars
      if (!within_quotes && field.len == 1) {
        // Extract the char from the string representing the field_type
        char field_type = cstr(&field)[0];

        if (field_type == CONFIG_COMMENT_CHAR) {
          // Ignore the whole line, comment has started
          break;
        }

        switch (field_type) {
        case CONFIG_FIELD_DECLARATION_CHAR:
          curr_field_type = CONFIG_FIELD_DECLARATION;
          continue;
        case CONFIG_ARRAY_START_CHAR:
          within_array_declaration = true;
          continue;
        case CONFIG_ARRAY_END_CHAR:
          curr_field_type = CONFIG_FIELD_NAME;
          within_array_declaration = false;
          continue;
        }
      }

      switch (curr_field_type) {
      case CONFIG_FIELD_NAME:
        // TODO had if(!has_var_been_named) around whole section, is this
        // needed? Reached a new config_var
        current_var_finalised = false;

        // Name the new variable
        var = config_var_new(&field);
        var_has_value = false;

        // Reset the array for storing the new data
        fields.len = 0;
        break;
      case CONFIG_FIELD_DECLARATION:
        if (within_array_declaration || !var_has_value) {
          // Associate the current field with the current variable
          string_t field_value = string_copy(&field);
          dynamic_array_append(&fields, &field_value);
        }
        break;
      }
    }

    // Check if the current var is completed, if so, save it to the config
    // Only arrays can be multi-line, therefore if its not in the array
    // declaration and the line is over, -> the variable is done
    if (!within_array_declaration) {
      if (!current_var_finalised) {

        // Convert to an array
        var.values = dynamic_array_to_array(&fields);

        // Save the current variable to the given array
        dynamic_array_append(&results, &var);

        // Saved
        current_var_finalised = true;
      }

      // Ready for the next field
      curr_field_type = CONFIG_FIELD_NAME;
    }
  }

  // Whole file has been loaded in now
  fclose(config_file);

  // This should only be called when within an array declaration
  // at the end of the file, save what data we can anyway
  if (!current_var_finalised) {
    // Convert to an array
    // Load the array into the var
    var.values = dynamic_array_to_array(&fields);

    // Save the current var to the given array
    dynamic_array_append(&results, &var);
  }

  // Sort the vars (ready for log(n) var retrieval time)
  array_t unsorted_vars = dynamic_array_to_array(&results);
  config.variables =
      tree_sort(unsorted_vars, sizeof(config_var_t), config_var_compare);
  array_destroy(&unsorted_vars);

  config.modified = false;
  config.path_to_cfg = string_new(0);
  string_clear(&config.path_to_cfg);
  string_write_c(&config.path_to_cfg, file_path);

  // Cleanup
  string_destroy(&buffer);
  string_destroy(&field);
  dynamic_array_destroy(&results);
  dynamic_array_destroy(&fields);

  return config;
}

void config_encode(string_t *dest, string_t *toEncode) {
  dest->len = 0;
  // Write a quote-formatted version of the string
  for (int x = 0; x < toEncode->len; x++) {
    char c = cstr(toEncode)[x];
    if (c == CONFIG_STRING_ENCLOSE_CHAR) {
      string_write_char(dest, CONFIG_ESCAPE_CHAR);
      string_write_char(dest, CONFIG_STRING_ENCLOSE_CHAR);
    } else {
      string_write_char(dest, c);
    }
  }
  string_null_terminate(dest);
}

/*
 * If any variables in the config have been modified, saves the config file.
 * If you have modified any variables directly, ensure to set the modified flag
 * to true (1) or the config file will not save
 *
 * returns true if successful, false if file failed to open
 */
bool config_save(config_t config) {
  /*
   * Nothing to save...
   */
  if (!config.modified) {
    return true;
  }

  string_t processed = string_new(DEFAULT_BUFFER_LEN);

  FILE *cfgOut = fopen(cstr(&config.path_to_cfg), MODE_WRITE);

  if (!cfgOut) {
    return false;
  }

  /*
   * Write the standard config header comment
   */
  fprintf(cfgOut, CONFIG_STANDARD_DESCRIPTION);
  for (int i = 0; i < config.variables.len; i++) {
    config_var_t* var = (config_var_t*)array_get(&config.variables, i);
    config_encode(&processed, &var->var_name);

    fprintf(cfgOut, "\"%s\" = [\n", cstr(&processed));

    if (var->values.len == 0) {
      fprintf(cfgOut, "\t%s\n", CONFIG_NO_ENTRY);
    } else {
      for (int j = 0; j < var->values.len; j++) {
        string_t* str = (string_t*)array_get(&var->values, j);
        config_encode(&processed, str);

        fprintf(cfgOut, "\t\"%s\"\n", cstr(&processed));
      }
    }

    fprintf(cfgOut, "\t]\n\n");
  }

  string_destroy(&processed);
  fclose(cfgOut);

  return true;
}

config_var_t *config_get_var(config_t *config, char *name) {
  /*
   * Convert the name into a searchable format
   */
  config_var_t key;

  // TODO what does this comment even mean??
  // len is 1 because a config_var_t can store multiple values
  key.values.len = 1;

  key.var_name = cstring_wrap(name);

  /*
   * Search the sorted config list for the requested var
   */
  config_var_t *var =
      bsearch(&key, config->variables.data, config->variables.len,
              sizeof(config_var_t), config_var_compare);

  return var;
}

int config_var_compare(const void *var1, const void *var2) {
  config_var_t *a = (config_var_t *)var1;
  config_var_t *b = (config_var_t *)var2;
  char *astr = cstr(&a->var_name);
  char *bstr = cstr(&b->var_name);

  if (!a || !astr) {
    if (!b || !bstr) {
      return 0;
    } else {
      // A is less than b
      return -1;
    }
  } else if (!b || !bstr) {
    if (!a || !astr) {
      return 0;
    } else {
      // b is greather than a
      return 1;
    }
  }

  // Both strings exist, comparing
  return strcmp(astr, bstr);
}

/*
 * Free's all dynamically allocated data in a Config
 */
void config_destroy(config_t config) {
  for (int i = 0; i < config.variables.len; i++) {
    config_var_t *var = array_get(&config.variables, i);

    string_destroy(&var->var_name);
    for (int y = 0; y < var->values.len; y++) {
      string_t* str = (string_t*)array_get(&var->values, y);
      string_destroy(str);
    }
    array_destroy(&var->values);
  }
  array_destroy(&config.variables);
}

void config_print_vars(config_t config) {
  // printf("CONFIG Contents:\n-----------------\n");
  // for (int i = 0; i < config.len; i++) {
  //   printf("%s\n", cstr(&config.vars[i].var_name));
  //   if (config.vars[i].len == 0) {
  //     printf("\t\"(null)\"\n");
  //   } else {
  //     for (int j = 0; j < config.vars[i].len; j++) {
  //       printf("\t\"%s\"\n", cstr(&config.vars[i].data[j]));
  //     }
  //   }
  //   printf("\n");
  // }
  // printf("-----------------\n");
}