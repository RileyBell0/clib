#include "../config.h"
static string_t config_read_string();
static bool config_is_permitted_char(char c);
static string_t config_read_basic_field(string_t *line, size_t *curr_char, bool arr_var);
static string_t config_extract_field(string_t *line, size_t *curr_char, bool arr_var);
static void config_remove_read_chars(string_t *line, size_t *curr_char);
static void config_process_line(dict_t *dict, string_t *line, int *state, string_t *key, vector_t *values);

static string_t config_read_string(string_t *line, size_t *curr_char)
{
  // Start after the quotation mark
  bool escape = false;
  bool complete = false;
  char *str = cstr(line);
  string_t key = empty_string();
  for (; *curr_char < line->len; *curr_char += 1)
  {
    char c = str[*curr_char];

    // Handle escape chars
    if (escape)
    {
      escape = false;
      if (c == CONFIG_ESCAPE)
      {
        string_append_char(&key, c);
      }
      else if (c == CONFIG_STRING)
      {
        string_append_char(&key, c);
      }
      else
      {
        exit_error("Invalid cfg file. '\\' must only preceed \\ or \"", "std/adv/c/config.c", "config_process_line");
      }
      continue;
    }
    else if (c == CONFIG_ESCAPE)
    {
      escape = true;
      continue;
    }

    if (c == CONFIG_STRING)
    {
      complete = true;
      break;
    }

    string_append_char(&key, c);
  }

  if (!complete)
  {
    exit_error("Unterminated string in cfg", "std/adv/c/config.c", "config_read_string");
  }

  *curr_char += 1;
  return key;
}

static bool config_is_permitted_char(char c)
{
  if (c == '_' || (c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122))
  {
    return true;
  }
  return false;
}

static string_t config_read_basic_field(string_t *line, size_t *curr_char, bool arr_var)
{
  string_t key = empty_string();
  char *str = cstr(line);
  for (; *curr_char < line->len; *curr_char += 1)
  {
    char c = str[*curr_char];
    if (config_is_permitted_char(c))
    {
      string_append_char(&key, c);
    }
    else if (c == CONFIG_EQUALS || c == ' ' || c == '\t' || c == '#' || (arr_var && (c == ',' || c == ']')))
    {
      break;
    }
    else
    {
      string_t message = string_new("Config key names cannot contain the char \"");
      string_append_char(&message, c);
      string_append_char(&message, '"');
      exit_error(cstr(&message), "std/adv/c/config.c", "config_read_basic_field");
    }
  }

  if (key.len == 0)
  {
    exit_error("A config key cannot be empty", "std/adv/c/config.c", "config_read_basic_field");
  }
  return key;
}

static string_t config_extract_field(string_t *line, size_t *curr_char, bool arr_var)
{
  char *str = cstr(line);
  char c = str[*curr_char];
  if (c == CONFIG_COMMENT)
  {
    exit_error("Expected config field declaration, saw comment char", "std/adv/c/config.c", "config_extract_field");
  }
  if (c == CONFIG_EQUALS)
  {
    exit_error("Unexpected equals sign in cfg file", "std/adv/c/config.c", "config_extract_field");
  }
  if (c == CONFIG_STRING)
  {
    *curr_char += 1;
    return config_read_string(line, curr_char);
  }
  else
  {
    return config_read_basic_field(line, curr_char, arr_var);
  }
}

/*
 * Remove chars we've read so far, and trim away remaining whitespace and tabs
 */
static void config_remove_read_chars(string_t *line, size_t *curr_char)
{
  string_t remaining = string_new(&cstr(line)[*curr_char]);
  string_clear(line);
  *line = remaining;
  string_trim(line);
  *curr_char = 0;
}

static void config_save_var(dict_t *dict, string_t *key, vector_t *values)
{
  array_t final_values = vector_to_array(values);
  final_values.destroy = void_string_destroy;
  dict_set(dict, cstr(key), &final_values);
  string_clear(key);
  vector_clear(values);
}

static void config_process_line(dict_t *dict, string_t *line, int *state, string_t *key, vector_t *values)
{
  // Ignore commented or blank lines
  string_trim(line);
  if (cstr(line)[0] == CONFIG_COMMENT || line->len == 0)
  {
    string_clear(line);
    return;
  }

  // Look for the field key
  if (*state == CONFIG_LOOKING_FOR_KEY)
  {
    if (line->len == 0)
    {
      return;
    }
    // Extract the key and save it
    size_t curr_char = 0;
    string_clear(key);
    *key = config_extract_field(line, &curr_char, false);
    if (key->len == 0)
    {
      exit_error("Config keys must have a length > 0", "std/adv/c/config.c", "config_process_line");
    }
    config_remove_read_chars(line, &curr_char);
    *state = CONFIG_LOOKING_FOR_DECLARATION;
  }
  if (cstr(line)[0] == CONFIG_COMMENT || line->len == 0)
  {
    string_clear(line);
    return;
  }

  // Look for the field equals char
  if (*state == CONFIG_LOOKING_FOR_DECLARATION)
  {
    if (line->len == 0)
    {
      return;
    }

    // Look for an equals sign
    size_t curr_char = 0;
    char c = cstr(line)[curr_char++];
    if (c != CONFIG_EQUALS)
    {
      exit_error("Unexpected char. Expected '=' after config key declaration", "std/adv/c/config.c", "config_process_line");
    }
    config_remove_read_chars(line, &curr_char);

    *state = CONFIG_LOOKING_FOR_VALUE;
  }
  if (cstr(line)[0] == CONFIG_COMMENT || line->len == 0)
  {
    string_clear(line);
    return;
  }

  // Look for and load in a value
  if (*state == CONFIG_LOOKING_FOR_VALUE)
  {
    if (line->len == 0)
    {
      return;
    }

    // Determine if the value is to be an array or singular value
    size_t curr_char = 0;
    char c = cstr(line)[curr_char];
    if (c == '[')
    {
      *state = CONFIG_LOOKING_FOR_VALUE_ARRAY;
      curr_char++;
      config_remove_read_chars(line, &curr_char);
    }
    if (*state == CONFIG_LOOKING_FOR_VALUE)
    {
      // Read in the value field
      string_t value = config_extract_field(line, &curr_char, false);
      config_remove_read_chars(line, &curr_char);

      // Save the value and reset state for the next key-value pair
      vector_append(values, &value);
      config_save_var(dict, key, values);
      if (line->len != 0 && cstr(line)[0] != '#')
      {
        exit_error("New keys must be declared on new lines in a cfg", "std/adv/c/config.c", "config_process_line");
      }
      *state = CONFIG_LOOKING_FOR_KEY;
    }
  }
  if (cstr(line)[0] == CONFIG_COMMENT || line->len == 0)
  {
    string_clear(line);
    return;
  }

  // Look for and load in array values
  if (*state == CONFIG_LOOKING_FOR_VALUE_ARRAY)
  {
    size_t curr_char = 0;
    while (line->len != 0)
    {
      // Ignore commas and whitespace
      while (true)
      {
        char c = cstr(line)[curr_char];
        if (c != ' ' && c != '\t' && c != ',')
        {
          break;
        }
        curr_char++;
      }

      // ignore the line, end the array or read in vars
      char c = cstr(line)[curr_char];
      if (c == '#')
      {
        break;
      }
      else if (c == ']')
      {
        curr_char++;
        config_save_var(dict, key, values);
        config_remove_read_chars(line, &curr_char);
        if (line->len != 0 && cstr(line)[0] != '#')
        {
          exit_error("New keys must be declared on new lines in a cfg", "std/adv/c/config.c", "config_process_line");
        }
        *state = CONFIG_LOOKING_FOR_KEY;
      }
      else
      {
        if (line->len - curr_char == 0)
        {
          // No remaining chars in the line
          break;
        }
        string_t value = config_extract_field(line, &curr_char, true);
        vector_append(values, &value);
        config_remove_read_chars(line, &curr_char);
      }
    }
  }
  if (cstr(line)[0] == CONFIG_COMMENT || line->len == 0)
  {
    string_clear(line);
    return;
  }
}

void config_value_destroy(void *arr)
{
  array_destroy((array_t *)arr);
}

dict_t config_read(char *path)
{
  dict_t results = dict_new(sizeof(array_t), config_value_destroy);

  int state = CONFIG_LOOKING_FOR_KEY;
  string_t buffer = empty_string();
  string_t *buf = &buffer;
  FILE *cfg_file = fileio_open_safe(path, true);
  string_t key = empty_string();
  vector_t values = vector_new(sizeof(string_t), NULL);
  while (true)
  {
    if (!fileio_next_line(cfg_file, buf))
    {
      if (state != CONFIG_LOOKING_FOR_KEY)
      {
        exit_error(cstr_cat("Unterminated config var in ", path, NULL), "std/adv/c/config.c", "config_read");
      }
      break;
    }

    config_process_line(&results, buf, &state, &key, &values);
  }

  fileio_close(cfg_file);

  string_destroy(buf);
  return results;
}