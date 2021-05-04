#include "../configLoader.h"

/*
 * returns the position in the string the function exited at
 *
 * complexString is set to TRUE iff the returned string was surrounded by quotes
 */// TODO this function is way too long
int extract_field(string_t* str, unsigned int *pos, string_t *dest,
                  int *complex_field) {
  string_clear(dest);
  
  // Why am i even stealing complex field?? if im just chanigng it to false
  *complex_field = FALSE;
  if (str->len <= *pos) {
    return FALSE;
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
    return TRUE;
  }

  /*
   * If we've reached a # as its own field
   */
  if (str_start[*pos] == CONFIG_COMMENT_CHAR) {
    string_write_char(dest, str_start[*pos]);
    string_null_terminate(dest);
    *pos += 1;
    return TRUE;
  }

  /*
   * If we've reached an array declaration square brace
   */
  if (str_start[*pos] == CONFIG_ARRAY_START_CHAR ||
      str_start[*pos] == CONFIG_ARRAY_END_CHAR) {
    string_write_char(dest, str_start[*pos]);
    string_null_terminate(dest);
    *pos += 1;
    return TRUE;
  }

  /*
   * If the current field is within quotes
   */
  int inQuotes = FALSE;
  if (str_start[*pos] == CONFIG_STRING_ENCLOSE_CHAR) {
    *complex_field = TRUE;
    *pos += 1;
    inQuotes = TRUE;
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
  return TRUE;
}

// RE-CHECKED 01/05/2021
config_var_t new_config_var(string_t* var_name) {

  config_var_t var;

  var.data = NULL;
  var.len = 0;
  var.var_name = string_copy(var_name);

  return var;
}

// RE-CHECKED 01/05/2021
config_t read_config_file(char *filePath) {
  // Storing the processed data
  dynamic_array_t results = new_dynamic_array(sizeof(config_var_t));

  // Stores the fields associated with the current variable
  dynamic_array_t fields = new_dynamic_array(sizeof(string_t));
  config_var_t var;
  config_t config;

  // Open the config file
  FILE *configFile = fopen(filePath, MODE_READ);

  // Stores lines read in from the config file
  string_t buffer = new_string(DEFAULT_BUFFER_LEN);

  // Storing a processed and formatted field string ready to be converted into a var
  string_t field = new_string(DEFAULT_BUFFER_LEN);
  
  /*
   * Read every line in the file
   */
  int within_array_declaration = FALSE;
  int has_var_been_named = FALSE;
  int curr_field_type = CONFIG_FIELD_NAME;
  int within_quotes = FALSE;
  int current_var_finalised = TRUE;
  /*
   * Has the current var thats being loaded in got at least one
   * value loaded in from the file
  */
  int var_has_value = FALSE; 

  // Read in and process the whole config file
  while (fileio_next_line(configFile, &buffer)) {
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
            within_array_declaration = TRUE;
            continue;
          case CONFIG_ARRAY_END_CHAR:
            curr_field_type = CONFIG_FIELD_NAME;
            within_array_declaration = FALSE;
            continue;
        } 
      }
      
      switch (curr_field_type) {
        case CONFIG_FIELD_NAME:
          // TODO had if(!has_var_been_named) around whole section, is this needed?
          // Reached a new config_var
          current_var_finalised = FALSE;

          // Name the new variable
          var = new_config_var(&field);
          has_var_been_named = TRUE;
          var_has_value = FALSE;

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
        array_t convertedFields = dynamic_array_to_array(&fields);

        // Save the array into the var 
        var.data = convertedFields.data;
        var.len = convertedFields.len;

        // Save the current variable to the given array
        dynamic_array_append(&results, &var);

        // Saved
        current_var_finalised = TRUE;
      }

      // Ready for the next field
      curr_field_type = CONFIG_FIELD_NAME;
      has_var_been_named = FALSE;
    }
  }

  // Whole file has been loaded in now
  fclose(configFile);

  // This should only be called when within an array declaration 
  // at the end of the file, save what data we can anyway
  if (!current_var_finalised) {
    // Convert to an array
    array_t convertedFields = dynamic_array_to_array(&fields);
    
    // Load the array into the var
    var.data = convertedFields.data;
    var.len = convertedFields.len;
    
    // Save the current var to the given array
    dynamic_array_append(&results, &var);
  }

  // Sort the vars (ready for log(n) var retrieval time)
  array_t unsorted_vars = dynamic_array_to_array(&results);
  array_t sorted_vars =
      tree_sort(unsorted_vars, sizeof(config_var_t), config_var_compare);

  array_destroy(unsorted_vars);

  config.len = sorted_vars.len;
  config.vars = sorted_vars.data;
  config.modified = FALSE;
  config.configLocation = filePath;

  // Cleanup
  string_destroy(&buffer);
  string_destroy(&field);
  dynamic_array_destroy(results);
  dynamic_array_destroy(fields);

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
 * to TRUE (1) or the config file will not save
 *
 * returns TRUE if successful, FALSE if file failed to open
 */
int config_save(config_t config) {
  /*
   * Nothing to save...
   */
  if (!config.modified) {
    return TRUE;
  }

  string_t processed = new_string(DEFAULT_BUFFER_LEN);

  FILE *cfgOut = fopen(config.configLocation, MODE_WRITE);

  if (!cfgOut) {
    return FALSE;
  }

  /*
   * Write the standard config header comment
   */
  fprintf(cfgOut, CONFIG_STANDARD_DESCRIPTION);
  for (int i = 0; i < config.len; i++) {
    config_encode(&processed, &config.vars[i].var_name);

    fprintf(cfgOut, "\"%s\" = [\n", cstr(&processed));

    if (config.vars[i].len == 0) {
      fprintf(cfgOut, "\t%s\n", CONFIG_NO_ENTRY);
    } else {
      for (int j = 0; j < config.vars[i].len; j++) {
        config_encode(&processed, &config.vars[i].data[j]);

        fprintf(cfgOut, "\t\"%s\"\n", cstr(&processed));
      }
    }

    fprintf(cfgOut, "\t]\n\n");
  }

  string_destroy(&processed);
  fclose(cfgOut);

  return TRUE;
}

config_var_t *config_get_var(config_t *config, char *name) {
  /*
   * Convert the name into a searchable format
   */
  config_var_t key;

  // TODO what does this comment even mean??
  // len is 1 because a config_var_t can store multiple values
  key.len = 1;

  key.var_name = cstring_wrap(name);

  /*
   * Search the sorted config list for the requested var
   */
  config_var_t *var = bsearch(&key, config->vars, config->len,
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
  for (int i = 0; i < config.len; i++) {
    config_var_t var = config.vars[i];

    string_destroy(&var.var_name);
    for (int i = 0; i < var.len; i++) {
      string_destroy(&var.data[i]);
    }
    destroy(var.data);
  }
  destroy(config.vars);
}

void config_print_vars(config_t config) {
  printf("CONFIG Contents:\n-----------------\n");
  for (int i = 0; i < config.len; i++) {
    printf("%s\n", cstr(&config.vars[i].var_name));
    if (config.vars[i].len == 0) {
      printf("\t\"(null)\"\n");
    } else {
      for (int j = 0; j < config.vars[i].len; j++) {
        printf("\t\"%s\"\n", cstr(&config.vars[i].data[j]));
      }
    }
    printf("\n");
  }
  printf("-----------------\n");
}