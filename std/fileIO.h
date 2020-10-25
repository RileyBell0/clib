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

/*
 * Attempts to open a file in the given mode
 * 
 * Returns NULL on failure
*/
FILE *fileio_open_advanced(char *filePath, char *mode);
FILE *fileio_open(char *filePath, int isReading);

/* If the given FILE pointer is not-null, the file is closed */
void fileio_close(FILE *file);

/* Attempts to read a line of input into the buffer
 * If the given string is too small for the new line it will be extended
 * to fit the whole line
 * returns TRUE (1) on success or FALSE (0) on failure */
int fileio_next_line(FILE *file, string_t *buffer);

/*
 * When given a string containing the path to a file,
 * the rest of the path is removed and just the file name
 * is returned in a *new* string
 * 
 * MEMORY IS ALLOCATED, ensure you destroy the returned string when
 * done with it
*/
string_t getFileName(string_t path);

/*
 * Modifies the given cstring, removing the file
 * extension if it is present.
 * 
 * The return value is the same as the recieved
 * char* pointer, allowing nesting in function
 * calls 
*/
char *removeFileExtension(char *fileName);

#endif