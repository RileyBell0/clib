/*
 * Author:          Riley Bell
 * Creation Date:   16/10/2020
*/

/* * * * * * * * * * * * * * * * * * * * * * * * *
 * TODO
 * instead of having two functions:
 * dir_all_files_recur and dir_all_files_with_extension_recur
 * just have a single method
 * dir_all_files_recur(path, int (*key)(string_t* filename))
 * where only filenames that return true when put into the key function
 * will be added to the resultant list
 * TODO
 * * * * * * * * * * * * * * * * * * * * * * * * *
*/

/*
 * For dealing with directories
 * 
 * Verified to (probably) work with Linux file systems
 * I mean, i havent really done extensive testing on this
 * code but it's worked with what ive needed it for
 * 
 * im reworking all of this
*/

// TODO might benefit from propper input buffering

// TODO -> theres a functoin that gets the extension
// it checks if the first char is . and if it is it ignores it
// but directory entries can be named "." or ".."
// but hidden files should also be catered for named shit like '.my_file.c'

#ifndef CLIB_STD_DIRECTORY_H
#define CLIB_STD_DIRECTORY_H

// TODO have this ony be included if it is available on the system
// so that means posix systems
#define CLIB_STD_DIRECTORY_H_DIRENT_H_INCLUDED

#ifdef CLIB_STD_DIRECTORY_H_DIRENT_H_INCLUDED

#include <dirent.h>
#include "../alist.h"
#include "../fileIO.h"
#include "../string.h"

/*
 * Returns an alist of type (struct dirent) of all entries within the
 * given directory 'path'
 * 
 * Destroy the alist with alist_destroy once finished with
 */
alist_t dir_all_entries_alist(string_t *path);

/*
 * Returns an alist of type (string_t) containing all files within the given
 * directory 'path'
 * 
 * destroy the alist with alist_destroy once finished with
 */
alist_t dir_all_files_recur(string_t *path);

/*
 * Returns an alist of type (string_t) containing all files with the given
 * extension within the given directory 'path'
 * 
 * destroy the alist with alist_destroy once finished with
 */
alist_t dir_files_with_extension_recur(string_t* path, string_t* extension);

/*
 * Returns TRUE if the given d_name string is either "." or ".."
 * returns FALSE otherwise
*/
int is_relative_dir_entry(string_t* dirName);


alist_t dir_all_entries_alist(string_t *path);

#endif
#endif
