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
 * TODOs
 * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * For dealing with directories
 *
 * Verified to (probably) work with Linux file systems
 * I mean, i havent really done extensive testing on this
 * code but it's worked with what ive needed it for
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

#include "../alist.h"
#include "../path.h"
#include "../string.h"
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

/*
 * Returns an alist of type (struct dirent) of all entries within the
 * given directory 'path'
 *
 * Destroy the alist with alist_destroy once finished with
 */
alist_t dir_all_entries_alist(string_t *path);

/*
 * Returns an alist of type (string_t) containing all files within the given
 * path matching the given key
 *
 * Key and extra are optional arguments:
 * - Key is the function that each filename will be compared by, if Key
 *   returns TRUE, the filename will be included
 *
 * - extra is any extra data needed to be passed to the key function, for
 *   instance, if basing around the extension of a filename, the key may be
 *   a function comparing filename and extension, and the 'extra' var might be
 *   a pointer to a string containing the required extension
 *
 * destroy the alist with alist_destroy once finished with
 */
alist_t dir_all_files_recur(string_t *path,
                            int (*key)(string_t *file_name, void *extra),
                            void *extra, bool include_base_path);

alist_t dir_all_file_names(string_t *path,
                           int (*key)(string_t *file_name, void *extra),
                           void *extra);

alist_t dir_all_entry_names(string_t *path,
                      int (*key)(string_t *entry_name, void *extra),
                      void *extra);

/*
 * Returns TRUE if the given d_name string is either "." or ".."
 * returns FALSE otherwise
 */
bool is_relative_dir_entry(string_t *dirName);

alist_t dir_all_entry_names(string_t *path,
                            int (*key)(string_t *entry_name, void *extra),
                            void *extra);



DIR* dir_open_safe_c(char* path);
DIR* dir_open_safe(string_t* path);

alist_t dir_all_entries_alist(string_t *path);

#endif
#endif
