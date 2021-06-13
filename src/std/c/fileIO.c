#include "../fileIO.h"

// RE-CHECKED 04/05/2021
// MEMORY_SAFE 04/05/2021
char *get_file_extension_start(char *file_name) {
  char *extension = NULL;

  int i = 0;
  while (file_name[i] != '\0') {
    if (file_name[i] == EXTENSION_SEPERATING_CHAR) {
      // We havent reached the end of the string
      if (file_name[i + 1] != '\0') {
        // We have the start of what could be a valid extension
        // The last valid potential extension start-point is returned
        extension = &file_name[i + 1];
      }
    }

    ++i;
  }

  if (!extension) {
    // Return a string of length 0
    return &file_name[i];
  }
  return extension;
}

// RE-CHECKED 04/05/2021
// MEMORY_SAFE 04/05/2021
FILE *fileio_open_safe_advanced(char *file_path, char *mode) {
  FILE *file = NULL;

  file = fopen(file_path, mode);

  assert(file);

  return file;
}

// RE-CHECKED 04/05/2021
// MEMORY_SAFE 04/05/2021
FILE *fileio_open_safe(char *file_path, int is_reading) {
  FILE *file = NULL;

  if (is_reading) {
    file = fopen(file_path, MODE_READ);
  } else {
    file = fopen(file_path, MODE_WRITE);
  }

  assert(file);

  return file;
}

// RE-CHECKED 04/05/2021
// MEMORY_SAFE 04/05/2021
FILE *fileio_open(char *file_path, int is_reading) {
  FILE *new_file = NULL;

  if (is_reading) {
    new_file = fopen(file_path, MODE_READ);
  } else {
    new_file = fopen(file_path, MODE_WRITE);
  }

  return new_file;
}

// RE-CHECKED 04/05/2021
// MEMORY_SAFE 04/05/2021
FILE *fileio_open_advanced(char *file_path, char *mode) {
  FILE *file = NULL;

  file = fopen(file_path, mode);

  return file;
}

// RE-CHECKED 04/05/2021
// MEMORY_SAFE 04/05/2021
void fileio_close(FILE *file) {
  if (file) {
    fclose(file);
  }
}

// MEMORY_SAFE 05/05/2021
int fileio_next_line(FILE *file, string_t *buffer) {
  char c;
  char *buffer_str = cstr(buffer);
  unsigned int chars_written = 0;
  while ((c = getc(file)) != EOF && c != '\n' && c != '\r') {
    // Need more room to store the next line
    if (chars_written == buffer->max_len) {
      string_extend(buffer);
    }

    buffer_str[chars_written++] = c;
  }
  buffer->len = chars_written;

  buffer_str[buffer->len] = '\0';

  // Failed to read anything
  if (c == EOF && chars_written == 0) {
    return FALSE;
  }

  // Read something from the file
  return TRUE;
}

int fileio_has_extension_key(string_t *file_name, void *extension) {
  return fileio_has_extension(file_name, (string_t *)extension);
}

int fileio_has_extension(string_t *file_name, string_t *extension) {
  int has_extension = FALSE;

  // Add the current file if it has the required extension
  string_t file_extension = get_file_extension(file_name);

  if (string_equals(&file_extension, extension) == TRUE) {
    has_extension = TRUE;
  }

  string_destroy(&file_extension);
  return has_extension;
}

// RE-CHECKED 04/05/2021
// MEMORY_SAFE 05/05/2021
string_t get_file_name_from_path(string_t *path) {
  char *path_str = cstr(path);

  for (int i = path->len; i >= 0; i--) {
    // The string is part of a path,
    if (path_str[i] == PATH_SEPERATOR_CHAR) {

      // The file name starts at the next character
      return string_make(&path_str[i + 1]);
    }
  }

  // The whole string is the file name
  return string_copy(path);
}

// RE-CHECKED 05/05/2021
// MEMORY_SAFE 05/05/2021
char *remove_file_extension_c(char *file_name, unsigned int name_len) {
  // Start at the end of the string and work backwards
  // until the extension seperating char is recieved
  for (unsigned int i = name_len; i >= 0; i--) {
    if (file_name[i] == EXTENSION_SEPERATING_CHAR) {
      file_name[i] = '\0';
      return file_name;
    }
  }
  return file_name;
}

// RE-CHECKED 04/05/2021
// MEMORY_SAFE 05/05/2021
string_t *remove_file_extension(string_t *file_name) {
  char *file_name_str = cstr(file_name);

  // If the first char is . -> hidden file, dont remove the rest
  // of the str, hence i >= 1
  for (unsigned int i = file_name->len; i >= 1; i--) {
    if (file_name_str[i] == EXTENSION_SEPERATING_CHAR) {
      string_limit(file_name, i);
      return file_name;
    }
  }

  return file_name;
}

// RE-CHECKED 05/05/2021
// MEMORY_SAFE 05/05/2021
void remove_file_extensions(alist_t *files) {
  // Removing the extension from the program names
  alist_iterator_t it = new_alist_iterator(files, TRUE);

  for (string_t *file = it.first(&it); !it.done(&it); file = it.next(&it)) {
    remove_file_extension(file);
  }
}

// RE-CHECKED 05/05/2021
// MEMORY_SAFE 05/05/2021
string_t get_file_extension(string_t *file_name) {
  // Start at the end of the string and work backwards
  // until the extension seperating char is recieved
  char *file_str = cstr(file_name);
  for (unsigned int i = file_name->len - 1; i > 0; i--) {

    // Check if the start of the extension has been reached
    if (file_str[i] == EXTENSION_SEPERATING_CHAR) {

      // Return a pointer to the char after the extension seperating char
      string_t extension = new_string(file_name->len - i);
      string_write_c_len(&extension, &file_str[i + 1], file_name->len - i - 1);
      return extension;
    }
  }

  return new_string(0);
}

// RE-CHECKED 04/05/2021
// MEMORY_SAFE 05/05/2021
alist_t get_file_names_from_paths(alist_t *files) {
  alist_t file_names = new_alist(sizeof(string_t));
  file_names.destroy = void_string_destroy;

  alist_iterator_t it = new_alist_iterator(files, TRUE);
  for (string_t *file = it.first(&it); !it.done(&it); file = it.next(&it)) {
    // Extract the file names from the file paths
    string_t file_name = get_file_name_from_path(file);
    alist_append(&file_names, &file_name);
  }

  return file_names;
}

// RE-CHECKED 04/05/2021
// MEMORY_SAFE 05/05/2021
alist_t fileio_read_all_lines_alist(char *file_name) {
  alist_t lines = new_alist(sizeof(string_t));
  lines.destroy = void_string_destroy;

  // Open the given file
  FILE *file = fopen(file_name, MODE_READ);

  // If the file exists and could be opened
  if (file) {
    /*
     * Creating the buffer, will be resized automatically to fit lines if
     * not large enough
     */
    string_t buffer = new_string(DEFAULT_BUFFER_LEN);

    /*
     * Reads all the lines of the file and appends them to the given list
     */
    while (fileio_next_line(file, &buffer)) {
      if (buffer.len > 0) {
        string_t line = string_copy(&buffer);
        alist_append(&lines, &line);
      }
    }

    // Done with the file, close it
    fileio_close(file);

    string_destroy(&buffer);
  }

  return lines;
}