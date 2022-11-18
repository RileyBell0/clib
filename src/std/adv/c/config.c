#include "../config.h"
static string_t config_read_string();
static bool config_is_permitted_char(char c);
static string_t config_read_basic_field(string_t *line, size_t *curr_char);
static string_t config_extract_field(string_t *line, size_t *curr_char);
static void config_remove_read_chars(string_t *line, size_t *curr_char);
static void config_process_line(dict_t *dict, string_t *line);

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

static string_t config_read_basic_field(string_t *line, size_t *curr_char)
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
    else if (c == CONFIG_EQUALS || c == ' ' || c == '\t')
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

static string_t config_extract_field(string_t *line, size_t *curr_char)
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
    return config_read_basic_field(line, curr_char);
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

static void config_process_line(dict_t *dict, string_t *line)
{
  string_trim(line);
  if (cstr(line)[0] == CONFIG_COMMENT || line->len == 0)
  {
    return;
  }

  // Extract the field
  size_t curr_char = 0;
  string_t key = config_extract_field(line, &curr_char);

  // Move "line" and "curr_char" to the next field
  config_remove_read_chars(line, &curr_char);

  // Ensure there is an equals sign following the key
  char c = cstr(line)[curr_char];
  if (c != '=')
  {
    exit_error("Unexpected char. Expected '=' after config key declaration", "std/adv/c/config.c", "config_process_line");
  }
  curr_char++;

  // Move "line" and "curr_char" to the next field, and ignore comments
  config_remove_read_chars(line, &curr_char);

  string_t value = config_extract_field(line, &curr_char);

  // Ensure the rest of the line is empty, or commented out
  config_remove_read_chars(line, &curr_char);
  if (line->len != 0 && cstr(line)[0] != '#')
  {
    string_t message = string_new("Unexpected token ");
    string_append_char(&message, cstr(line)[0]);
    string_append_c(&message, " after config key value pair definition");
    exit_error(cstr(&message), "std/adv/c/config.c", "config_process_line");
  }

  dict_set(dict, cstr(&key), &value);
  string_destroy(&key);
}

dict_t config_read(char *path)
{
  dict_t results = dict_new(sizeof(string_t), void_string_destroy);

  FILE *cfg_file = fileio_open_safe(path, true);
  string_t buffer = empty_string();
  string_t *buf = &buffer;
  while (fileio_next_line(cfg_file, buf))
  {
    config_process_line(&results, buf);
  }
  fileio_close(cfg_file);

  string_destroy(buf);
  return results;
}