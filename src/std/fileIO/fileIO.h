#ifndef CLIB_FILEIO_H
#define CLIB_FILEIO_H

#include <stdio.h>
#include <assert.h>
#include "../general/general.h"
#include "../bool/bool.h"
#include "../string/string.h"

#define MODE_READ "r\0"
#define MODE_WRITE "w\0"

/* Attempts to open a file in the given mode
 * Quits the program on failure */
FILE* openFile(char* filePath, int isReading);

/* Closes the file if it is open, otherwise does nothing */
void closeFile(FILE* file);

/* Attempts to read a line of input into the buffer with a maximum length of bufferSize
 * returns TRUE (1) on success or FALSE (0) on failure */
int nextLine(FILE* file, string buffer);

#endif