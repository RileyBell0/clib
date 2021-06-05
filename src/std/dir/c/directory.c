#include "../directory.h"

/*
 * Given a path such as 'cool/awesome/file.txt'
 * returns a string containing 'awesome/file.txt'
*/
string_t dir_path_remove_start(string_t* path) {
  if (path->len == 0) {
    return new_string(0);
  }
  
  /*
   * Find and return a string containing the entire path except
   * for the starting directory
  */
  char* pathstr = cstr(path);
  unsigned int pathsep_len = strlen(PATH_SEPERATOR);
  for (unsigned int c = 0; c < path->len; c++){
    if (cstring_equals_range(&pathstr[c], PATH_SEPERATOR, pathsep_len)) {
      c += pathsep_len;
      return string_make(&pathstr[c]);
    }
  }

  return new_string(0);
}

// RE-CHECKED 04/05/2021
// MEMORY_SAFE 04/05/2021
alist_t dir_all_entries_alist(string_t *path) {
  DIR *d = opendir(cstr(path));
  alist_t entries = new_alist(sizeof(struct dirent));

  if (!d) {
    return entries;
  }

  struct dirent *entry;
  while ((entry = readdir(d))) {
    // Copy the contents of 'entry'
    alist_append(&entries, entry);
  }

  // CLEANUP
  closedir(d);

  return entries;
}

alist_t dir_all_files_recur(string_t *path,
                            int (*key)(string_t *file_name, void *extra),
                            void *extra) {
  // The return type is a list containing the paths to all matching files
  alist_t valid_files = new_alist(sizeof(string_t));
  valid_files.destroy = void_string_destroy;
  
  string_t path_sep = string_make(PATH_SEPERATOR);

  string_t base_path = new_string(path->len + path_sep.len);
  string_write(&base_path, path);
  string_write(&base_path, &path_sep);

  // Getting all the directory entries in the current directory
  alist_t entries = dir_all_entries_alist(path);

  string_t file_name = new_string(0);
  string_t entry_path = new_string(base_path.len);
  string_write(&entry_path, &base_path);

  alist_iterator_t it = new_alist_iterator(&entries, TRUE);
  for (struct dirent *entry = it.first(&it); !it.done(&it);
       entry = it.next(&it)) {
    // Generate the string for the entry-name
    string_clear(&file_name);
    string_write_c(&file_name, entry->d_name);

    // Generate the whole path to the entry
    string_limit(&entry_path, base_path.len);
    string_write(&entry_path, &file_name);

    if (!is_relative_dir_entry(&file_name)) {
      // Add all files from the sub directory
      DIR *d = opendir(cstr(&entry_path));
      if (d) {
        // Get all files from the sub dir which are valid based
        // on the key functiono
        alist_t sub_dir_files = dir_all_files_recur(&entry_path, key, extra);
        sub_dir_files.destroy_on_remove = FALSE;
        closedir(d);

        // Add the files to the alist
        alist_combine(&valid_files, &sub_dir_files);
        alist_destroy(&sub_dir_files);

      } else {
        int append = TRUE;
        if (key && !key(&file_name, extra)) {
          append = FALSE;
        }

        if (append) {
          string_t name = dir_path_remove_start(&entry_path);
          alist_append(&valid_files, &name);
        }
      }
    }
  }

  string_destroy(&base_path);
  string_destroy(&file_name);
  string_destroy(&entry_path);
  alist_destroy(&entries);

  return valid_files;
}

// RE-CHECKED 04/05/2021
// MEMORY_SAFE 04/05/2021
int is_relative_dir_entry(string_t *path) {
  if (path->len == 1) {
    if (cstring_equals(cstr(path), ".")) {
      return TRUE;
    }
  } else if (path->len == 2) {
    if (cstring_equals(cstr(path), "..")) {
      return TRUE;
    }
  }
  return FALSE;
}