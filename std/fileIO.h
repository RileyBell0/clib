/*
 * Author:          Riley Bell
 * Creation Date:   16/10/2020
*/

#ifndef CLIB_STD_FILEIO_H
#define CLIB_STD_FILEIO_H

#define EXTENSION_SEPERATING_CHAR '.'

#include <stdio.h>
#include <assert.h>
#include "general.h"
#include "bool.h"
#include "string.h"
#include "path.h"

#define MODE_READ "r"
#define MODE_WRITE "w"

/* Attempts to open a file in the given mode
 * Quits the program on failure */
FILE *fileio_open_safe_advanced(char *filePath, char *mode);
FILE *fileio_open_safe(char *filePath, int isReading);
FILE *fileio_open(char *filePath, int isReading);
FILE *fileio_open_advanced(char *filePath, char *mode);

/* Closes the file if it is open, otherwise does nothing */
void fileio_close(FILE *file);

/* Attempts to read a line of input into the buffer
 * If the given string is too small for the new line it will be extended
 * returns TRUE (1) on success or FALSE (0) on failure */
int fileio_next_line(FILE *file, string_t *buffer);

string_t getFileName(string_t path);

char *removeFileExtension(char *fileName);

#endif