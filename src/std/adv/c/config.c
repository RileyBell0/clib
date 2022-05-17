#include "../config.h"

static config_var_t config_var_new(string_t *var_name);
static void config_field_handle_char(string_t *field, char last, char current);
static void config_refill_line_buffer(string_t* line_buffer, int *buffer_index, FILE* stream_in);
static string_t config_extract_field(string_t* line_buffer, int buffer_start_pos, FILE *stream_in);
static void config_find_field_definition(string_t *line_buffer, int buffer_start_pos, FILE *stream_in);
static void config_buffer_remove_from_start(string_t* line_buffer, int amount);
static array_t config_load_array_values(config_var_t* var, string_t* line_buffer, int buffer_start_pos, FILE* stream_in);
static void config_load_field_value(config_var_t* var, string_t* line_buffer, int buffer_start_pos, FILE* stream_in);

//////////////////////////////
// Function Definitions
//////////////////////////////

static config_var_t config_var_new(string_t *var_name) {
  config_var_t var;

  var.values = array_new(0, sizeof(string_t));
  var.var_name = string_copy(var_name);

  return var;
}

static void config_field_handle_char(string_t *field, char last, char current) {
  if (last == CONFIG_ESCAPE_CHAR) {
    // Handle escaped character
    switch (current) {
    case CONFIG_ESCAPE_CHAR:
    case CONFIG_FIELD_ENCLOSE_CHAR:
      string_write_char(field, current);
      break;
    case CONFIG_ESCAPED_NEW_LINE:
      string_write_char(field, '\n');
      break;
    case CONFIG_ESCAPED_CARRIAGE_RETURN:
      string_write_char(field, '\r');
      break;
    default:
      exit_error("Invalid character following '\\' in config field",
                 "std/adv/c/config.c", "config_extract_field");
      break;
    }
  } else if (current != CONFIG_FIELD_ENCLOSE_CHAR && current != CONFIG_ESCAPE_CHAR && !cstring_contains(CONFIG_FIELD_IGNORE_CHARS, current)) {
    string_write_char(field, current);
  }
}

static void config_refill_line_buffer(string_t* line_buffer, int *buffer_index, FILE* stream_in) {
    // Refill buffer if it has been spent
    if (*buffer_index >= line_buffer->len) {
      // Keep refilling until 1 or more characters are in the buffer
      do {
        if (!fileio_next_line(stream_in, line_buffer)) {
          exit_error("Invalid field in config - missing closing quote",
                    "std/adv/c/config.c", "config_extract_field");
        }
      } while (line_buffer->len == 0);

      *buffer_index = 0;
    }
}

// The line buffer will be modified after running this function
// The line buffer's first charater will be directly after the field which has
// just been read -> you're up to line_buffer[0]
static string_t config_extract_field(string_t* line_buffer, int buffer_start_pos, FILE *stream_in) {
  string_t field = string_new(0);

  // Read from the input stream into the field
  char c;
  int buffer_index = buffer_start_pos;
  char last = '\0';
  while (true) {
    config_refill_line_buffer(line_buffer, &buffer_index, stream_in);

    // Handle the next character from the file
    c = cstr(line_buffer)[buffer_index];
    config_field_handle_char(&field, last, c);
    buffer_index++;

    // Handle special characters
    if (last == CONFIG_ESCAPE_CHAR) {
      // Break out of escaped character mode
      last = '\0';
    } else if (c == CONFIG_FIELD_ENCLOSE_CHAR) {
      // Field end reached - fix buffer and return
      string_t buffer_leftover = string_make(&cstr(line_buffer)[buffer_index]);
      string_clear(line_buffer);
      string_write(line_buffer, &buffer_leftover);
      string_destroy(&buffer_leftover);

      return field;
    } else {
      last = c;
    }
  }

  exit_error("Invalid field in config - missing closing quote",
             "std/adv/c/config.c", "config_extract_field");
  return field;
}

// The line buffer will be modified after running this function
// This function will either succeed or crash the program thanks to an invalid config file
// The line buffer's first charater will be directly after the field which has
// just been read -> you're up to line_buffer[0]
static void config_find_field_definition(string_t *line_buffer, int buffer_start_pos, FILE *stream_in) {
  char c;
  int buffer_index = buffer_start_pos;
  while (true) {
    config_refill_line_buffer(line_buffer, &buffer_index, stream_in);

    c = cstr(line_buffer)[buffer_index];

    // Look for field declaration and handle unexpected chars
    if (c == CONFIG_FIELD_DECLARATION_CHAR) {
      // Field declaration reached - fix buffer and return
      string_t buffer_leftover = string_make(&cstr(line_buffer)[buffer_index + 1]);
      string_clear(line_buffer);
      string_write(line_buffer, &buffer_leftover);
      string_destroy(&buffer_leftover);

      return;
    }
    else if (c == CONFIG_COMMENT_CHAR) {
      buffer_index = line_buffer->len;
    }
    else if (!cstring_contains(CONFIG_PADDING_CHARS, c)) {
      exit_error("Field name was not followed by '='", "std/adv/c/config.c", "config_find_field_definition");
    }

    buffer_index++;
  }
}

static void config_buffer_remove_from_start(string_t* line_buffer, int amount) {
  if (amount > 0) {
    string_t buffer_leftover = string_make(&cstr(line_buffer)[amount]);
    string_clear(line_buffer);
    string_write(line_buffer, &buffer_leftover);
    string_destroy(&buffer_leftover);
  }
}

static array_t config_load_array_values(config_var_t* var, string_t* line_buffer, int buffer_start_pos, FILE* stream_in) {
  char c;
  int buffer_index = buffer_start_pos;
  vector_t fields = vector_new(sizeof(string_t));
  while (true) {
    config_refill_line_buffer(line_buffer, &buffer_index, stream_in);

    c = cstr(line_buffer)[buffer_index];

    if (c == CONFIG_COMMENT_CHAR) {
      buffer_index = line_buffer->len;
    }
    else if (c == CONFIG_ARRAY_END_CHAR) {
      array_t values = vector_to_array(&fields);
      values.destroy = void_string_destroy;
      
      vector_destroy(&fields);
      
      config_buffer_remove_from_start(line_buffer, buffer_index + 1);

      return values;
    }
    else if (c == CONFIG_FIELD_ENCLOSE_CHAR) {
      string_t value = config_extract_field(line_buffer, buffer_index + 1, stream_in);
      vector_append(&fields, &value);
      buffer_index = 0;
    }
    else if (!cstring_contains(CONFIG_ARRAY_PADDING_CHARS, c)){
      exit_error("Invalid character within config array declaration", "std/adv/c/config.c", "config_load_array_values");
    }
    else {
      buffer_index++;
    }
  }
}

static void config_load_field_value(config_var_t* var, string_t* line_buffer, int buffer_start_pos, FILE* stream_in) {
  // Read from the input stream into the field
  char c;
  int buffer_index = buffer_start_pos;
  while (true) {
    config_refill_line_buffer(line_buffer, &buffer_index, stream_in);

    // Handle the next character from the file
    c = cstr(line_buffer)[buffer_index];

    if (c == CONFIG_COMMENT_CHAR) {
      buffer_index = line_buffer->len;
    }
    else if (c == CONFIG_ARRAY_START_CHAR) {
      // Read in all values, store in the config_var_t
      array_t values = config_load_array_values(var, line_buffer, buffer_index + 1, stream_in);
      var->values = values;
      
      return;
    }
    else if (c == CONFIG_FIELD_ENCLOSE_CHAR) {
      // Single value variable
      string_t field = config_extract_field(line_buffer, buffer_index + 1, stream_in);
      
      // Store the value in the config_var_t var
      array_t values = array_new(1, sizeof(string_t));
      values.destroy = void_string_destroy;
      array_set(&values, 0, &field);
      var->values = values;
      
      return;
    }
    else if (!cstring_contains(CONFIG_PADDING_CHARS, c)) {
      // Something unexpected arrived - not a [ or a "
      exit_error("Unexpected character in config before value definition", "std/adv/c/config.c", "config_load_field_value");
    }
    else {
      buffer_index++;
    }
  }
}

//////////////////////////////
// Config Read / Write
//////////////////////////////

config_t config_read(char *file_path) {
  FILE *config_file = fileio_open_safe(file_path, true);
  string_t line_buffer = string_new(DEFAULT_BUFFER_LEN);
  vector_t variables = vector_new(sizeof(config_var_t));

  char c;
  while (fileio_next_line(config_file, &line_buffer)) {
    for (int i = 0; i < line_buffer.len; i++) {
      c = cstr(&line_buffer)[i];

      if (c == CONFIG_COMMENT_CHAR) {
        // Ignore commented lines
        break;
      } else if (c == CONFIG_FIELD_ENCLOSE_CHAR) {
        // field starts at the next char, extract it
        string_t field = config_extract_field(&line_buffer, i + 1, config_file);

        // Previous function call reset the index we're up to in the buffer to 0
        config_find_field_definition(&line_buffer, 0, config_file);
        
        // Previous function call reset the index we're up to in the buffer to 0
        config_var_t resultant = config_var_new(&field);
        config_load_field_value(&resultant, &line_buffer, 0, config_file);

        // Save this variable and its associated values
        vector_append(&variables, &resultant);

        // Update the position in line_buffer that we're up to
        i = -1;
      } else if (!cstring_contains(CONFIG_PADDING_CHARS, c)) {
        exit_error("Unexpected character in config", file_path, "config_read");
      }
    }
  }
  string_destroy(&line_buffer);

  config_t config;
  config.modified = false;
  config.path_to_cfg = string_make(file_path);
  config.variables = vector_to_array(&variables);
  vector_destroy(&variables);

  return config;
}




bool config_contains(config_t* cfg, string_t* var_name) {
  for (int i = 0; i < cfg->variables.len; i++) {
    config_var_t* var = (config_var_t*)array_get(&cfg->variables, i);
    if (string_equals(&var->var_name, var_name)) {
       return true;
    }
  }
  return false;
}
bool config_contains_c(config_t* cfg, char* var_name) {
  for (int i = 0; i < cfg->variables.len; i++) {
    config_var_t* var = (config_var_t*)array_get(&cfg->variables, i);
    if (strcmp(cstr(&var->var_name), var_name) == 0) {
       return true;
    }
  }
  return false;
}

array_t config_get(config_t* cfg, string_t* var_name) {
  for (int i = 0; i < cfg->variables.len; i++) {
    config_var_t* var = (config_var_t*)array_get(&cfg->variables, i);
    if (string_equals(&var->var_name, var_name)) {
       return var->values;
    }
  }

  return array_new(0, sizeof(string_t));
}
array_t config_get_c(config_t* cfg, char* var_name) {
  for (int i = 0; i < cfg->variables.len; i++) {
    config_var_t* var = (config_var_t*)array_get(&cfg->variables, i);
    if (strcmp(cstr(&var->var_name), var_name)) {
       return var->values;
    }
  }

  return array_new(0, sizeof(string_t));

}