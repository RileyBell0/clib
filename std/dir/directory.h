/*
 * Author:          Riley Bell
 * Creation Date:   16/10/2020
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
 * returns a list of type (struct dirent*)
 * of all entries in the directory at the given path
 * TODO put the destroy function in the alist in this function
*/
alist_t dir_all_entries_alist(string_t* path);

alist_t dir_all_files_recur(string_t* path);

alist_t dir_files_with_extension_recur(string_t* path, string_t* extension);

int is_relative_dir_entry(string_t* dirName);

#endif
#endif
