#include "../fileIO.h"

char *path_file_extension(char *fileName) {
  if (!fileName) {
    return NULL;
  }
  char *extension = NULL;

  int i = 0;
  while (fileName[i] != '\0') {
    if (fileName[i] == EXTENSION_SEPERATING_CHAR) {
      // We havent reached the end of the string
      if (fileName[i + 1] != '\0') {
        // We have the start of what could be a valid extension
        // The last valid potential extension start-point is returned
        extension = &fileName[i + 1];
      }
    }

    ++i;
  }

  return extension;
}

FILE *fileio_open_safe_advanced(char *filePath, char *mode) {
  FILE *newFile = NULL;

  newFile = fopen(filePath, mode);

  assert(newFile);

  return newFile;
}

FILE *fileio_open_safe(char *filePath, int isReading) {
  FILE *newFile = NULL;

  if (isReading) {
    newFile = fopen(filePath, MODE_READ);
  } else {
    newFile = fopen(filePath, MODE_WRITE);
  }

  assert(newFile);

  return newFile;
}

FILE *fileio_open(char *filePath, int isReading) {
  FILE *newFile = NULL;

  if (isReading) {
    newFile = fopen(filePath, MODE_READ);
  } else {
    newFile = fopen(filePath, MODE_WRITE);
  }

  return newFile;
}

FILE *fileio_open_advanced(char *filePath, char *mode) {
  FILE *newFile = NULL;

  newFile = fopen(filePath, mode);

  return newFile;
}

void fileio_close(FILE *file) {
  if (file != NULL) {
    fclose(file);
  }
}

int fileio_next_line(FILE *file, string_t *buffer) {
  if (file == NULL || buffer == NULL) {
    return FALSE;
  }

  char c;
  char *bufferstr = cstr(buffer);
  unsigned int charsWritten = 0;
  while ((c = getc(file)) != EOF && c != '\n' && c != '\r') {
    // Need more room to store the next line
    if (charsWritten == buffer->max_len) {
      string_extend(buffer);
    }

    bufferstr[charsWritten++] = c;
  }
  buffer->len = charsWritten;

  bufferstr[buffer->len] = '\0';

  // Failed to read anything
  if (c == EOF && charsWritten == 0) {
    return FALSE;
  }

  // Read something from the file
  return TRUE;
}

// RE-CHECKED 04/05/2021
string_t get_file_name_from_path(string_t *path) {
  string_t file_name;
  char *path_str = cstr(path);

  for (int i = path->len; i >= 0; i--) {
    // The string is part of a path, 
    if (path_str[i] == PATH_SEPERATOR_CHAR) {
      
      // Allocate enough space for the file_name
      file_name = new_string(path->len - i);

      // The file name starts at the next character
      return string_make(&path_str[i + 1]);
    }
  }

  // The whole string is the file name
  return string_copy(path);
}

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
string_t* remove_file_extension(string_t *file_name) {
  char* file_name_str = cstr(file_name);

  // If the first char is . -> hidden file, dont remove the rest
  // of the str
  for (unsigned int i = file_name->len; i >= 1; i--) {
    if (file_name_str[i] == EXTENSION_SEPERATING_CHAR) {
      string_shrink(file_name, i);
      return file_name;
    }
  }

  return file_name;
}

void remove_file_extensions(alist_t* files) {
  // Removing the extension from the program names
  alist_iterator_t it = new_alist_iterator(files, TRUE);
  
  for (string_t* file = it.first(&it); !it.done(&it); file = it.next(&it)) {
    remove_file_extension(file);
  }
}

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
alist_t get_file_names_from_paths(alist_t* files) {
  alist_t file_names = new_alist(sizeof(string_t));
  
  alist_iterator_t it = new_alist_iterator(files, TRUE);
  for (string_t* file = it.first(&it); !it.done(&it); file = it.next(&it)) {
    // Extract the file names from the file paths
    string_t file_name = get_file_name_from_path(file);
    alist_append(&file_names, &file_name);
  }

  return file_names;
}

// RE-CHECKED 04/05/2021
alist_t fileio_read_all_lines_alist(char *file_name) {
  alist_t lines = new_alist(sizeof(string_t));

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