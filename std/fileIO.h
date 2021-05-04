/*
 * Author:          Riley Bell
 * Creation Date:   16/10/2020
*/

#ifndef CLIB_STD_FILEIO_H
#define CLIB_STD_FILEIO_H

#define EXTENSION_SEPERATING_CHAR '.'

#include <stdio.h>
#include "filePath.h"
#include "string.h"
#include "alist.h"

#define MODE_READ "r"
#define MODE_WRITE "w"
#define DEFAULT_BUFFER_LEN 511
#define MEDIUM_BUFFER_LEN 255
#define SMALL_BUFFER_LEN 63

/* Attempts to open a file in the given mode
 * Quits the program on failure */
FILE *fileio_open_safe_advanced(char *filePath, char *mode);
FILE *fileio_open_safe(char *filePath, int isReading);

/*
 * Returns a pointer to the start of the file extension
 * within this string.
 * Does not modify the input string and does not allocate memory
*/
char *path_file_extension(char *fileName);

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
string_t get_file_name_from_path(string_t* path);

/*
 * returns a string containing the file extension, or an empty
 * string if no such extension exists
*/
string_t get_file_extension(string_t *fileName);

/*
 * Modifies the given cstring, removing the file
 * extension if it is present.
 * 
 * The return value is the same as the recieved
 * char* pointer, allowing nesting in function
 * calls 
*/
string_t *remove_file_extension(string_t* file_name);

char* remove_file_extension_c(char* file_name, unsigned int name_len);

/*
 * Given a list of files, the paths to the files are removed
 * and their filename is returned
 * /User/Desktop/myFile.txt -> myFile.txt
*/
alist_t get_file_names_from_paths(alist_t* files);

/*
 * Given a list of files, removes the file extensions from
 * each file
 * /Folder/myFile.txt -> /Folder/myFile
*/
void remove_file_extensions(alist_t* files);

/*
 * Reads all lines in the given file into a list
 * List is of type string_t
*/
alist_t fileio_read_all_lines_alist(char *fileName);

#endif