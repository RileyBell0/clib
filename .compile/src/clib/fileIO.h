#ifndef CLIB_STD_FILEIO_H
#define CLIB_STD_FILEIO_H

#define EXTENSION_SEPERATING_CHAR '.'

#include "string.h"
#include <stdio.h>
#include <assert.h>

#define MODE_READ "r"
#define MODE_WRITE "w"
#define DEFAULT_BUFFER_LEN 511
#define MEDIUM_BUFFER_LEN 255
#define SMALL_BUFFER_LEN 63

/* Attempts to open a file in the given mode
 * Quits the program on failure */
FILE *fileio_open_safe_advanced(char *path, char *mode);
FILE *fileio_open_safe(char *path, bool reading);

/*
 * Attempts to open a file in the given mode
 *
 * Returns NULL on failure
 */
FILE *fileio_open_advanced(char *path, char *mode);
FILE *fileio_open(char *path, bool reading);

/* If the given FILE pointer is not-null, the file is closed */
void fileio_close(FILE *file);

/* Attempts to read a line of input into the buffer
 * If the given string is too small for the new line it will be extended
 * to fit the whole line
 * returns TRUE (1) on success or FALSE (0) on failure */
bool fileio_next_line(FILE *file, string_t *buffer);

#endif