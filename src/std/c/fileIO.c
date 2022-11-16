#include "../fileIO.h"

FILE *fileio_open_safe_advanced(char *file_path, char *mode)
{
  FILE *file = NULL;

  file = fopen(file_path, mode);

  assert(file);

  return file;
}

FILE *fileio_open_safe(char *file_path, bool is_reading)
{
  FILE *file = NULL;

  if (is_reading)
  {
    file = fopen(file_path, MODE_READ);
  }
  else
  {
    file = fopen(file_path, MODE_WRITE);
  }

  assert(file);

  return file;
}

FILE *fileio_open(char *file_path, bool is_reading)
{
  FILE *new_file = NULL;

  if (is_reading)
  {
    new_file = fopen(file_path, MODE_READ);
  }
  else
  {
    new_file = fopen(file_path, MODE_WRITE);
  }

  return new_file;
}

FILE *fileio_open_advanced(char *file_path, char *mode)
{
  FILE *file = NULL;

  file = fopen(file_path, mode);

  return file;
}

void fileio_close(FILE *file)
{
  if (file)
  {
    fclose(file);
  }
}

bool fileio_next_line(FILE *file, string_t *buffer)
{
  int c;
  char *buffer_str = cstr(buffer);
  unsigned int chars_written = 0;
  while ((c = getc(file)) != EOF && c != '\n' && c != '\r')
  {
    // Need more room to store the next line
    if (chars_written == buffer->max_len)
    {
      string_extend(buffer);
    }

    buffer_str[chars_written++] = c;
  }
  buffer->len = chars_written;

  buffer_str[buffer->len] = '\0';

  // Failed to read anything
  if (c == EOF && chars_written == 0)
  {
    return false;
  }

  // Read something from the file
  return true;
}

array_t fileio_read_all_lines(char *file_name)
{

  // Read in all lines and store in vector 'lines'
  vector_t lines = vector_new(sizeof(string_t));
  string_t buffer = string_new(DEFAULT_BUFFER_LEN);
  FILE *file = fileio_open_safe(file_name, true);
  while (fileio_next_line(file, &buffer))
  {
    string_t line = string_copy(&buffer);
    vector_append(&lines, &line);
  }
  fileio_close(file);
  string_destroy(&buffer);

  // Convert to an array and return
  array_t all_lines = vector_to_array(&lines);
  all_lines.destroy = void_string_destroy;
  vector_destroy(&lines);

  return all_lines;
}