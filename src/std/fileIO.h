/*
 * Author:          Riley Bell
 * Creation Date:   16/10/2020
 */

#ifndef CLIB_STD_FILEIO_H
#define CLIB_STD_FILEIO_H

#define EXTENSION_SEPERATING_CHAR '.'

#include "struc/alist.h"
#include "struc/vector.h"
#include "path.h"
#include "string.h"
#include <stdio.h>

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
 * Returns a pointer to the start of the file extension
 * within this string.
 * Does not modify the input string and does not allocate memory
 * Returns NULL on failure
 */
char *fileio_get_file_extension_start(char *file_name);

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

/*
 * Given a file name and a required extension, returns TRUE
 * if the given filename has the extension 'extension'
 */
bool fileio_has_extension(string_t *file_name, string_t *extension);

/*
 * Wrapper for fileio_has_extension to ignore compiler warnings
 * TODO is there a better way to do this?
 */
int fileio_has_extension_key(string_t *file_name, void *extension);

/*
 * When given a string containing the path to a file,
 * the rest of the path is removed and just the file name
 * is returned in a *new* string
 *
 * MEMORY may be allocated, ensure you destroy the returned string when
 * done with it using string_destroy
 */
string_t fileio_get_file_name_from_path(string_t *path);

/*
 * returns a string containing the file extension, or an empty
 * string if no such extension exists
 * MEMORY may be allocated, ensure you destroy the returned string with
 * string_destroy when done with it
 */
string_t fileio_get_file_extension(string_t *file_name);

/*
 * Modifies the given cstring, removing the file
 * extension if it is present.
 *
 * The return value is the same as the recieved
 * char* pointer, allowing nesting in function
 * calls
 */
string_t *fileio_remove_file_extension(string_t *file_name);

/*
 * Given a c-string, removes the file extension, modifying the original
 * c-string passed to the function
 */
char *fileio_remove_file_extension_c(char *file_name, unsigned int name_len);

/*
 * Given a list of files, the paths to the files are removed
 * and their file_name is returned in an alist of type (string_t)
 * destroy with alist_destroy when done with the returned alist
 * /User/Desktop/myFile.txt -> myFile.txt
 */
alist_t fileio_get_file_names_from_paths(alist_t *files);

/*
 * Given a list of files, removes the file extensions from
 * each file
 * /Folder/myFile.txt -> /Folder/myFile
 */
void fileio_remove_file_extensions(alist_t *files);

array_t fileio_read_all_lines(char *file_name);

/*
 * Reads all lines in the given file into a list
 * the alist is of type (string_t)
 * destroy with alist_destroy when done with it
 */
alist_t fileio_read_all_lines_alist(char *file_name);

#endif