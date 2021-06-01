#include "../directory.h"

alist_t dir_all_files_recur(string_t* path);

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

int depth = 0;
FILE* logfile = NULL;
// RE-CHECKED 04/05/2021
// MEMORY_SAFE 04/05/2021
alist_t dir_all_files_recur(string_t* path) {
  if (!logfile){
    logfile = fileio_open_safe("log.txt", FALSE);
  }
  fprintf(logfile, "Path \"%s\" %d\n", cstr(path), path->len);
  // The return type is a list containing the paths to all matching files
  alist_t valid_files = new_alist(sizeof(string_t));
  valid_files.destroy = void_string_destroy;
  depth += 1;
  if (depth > 20){
    fprintf(logfile, "DEPTH COUNTER EXITING\n");
    return valid_files;
  }
  string_t path_sep = string_make(PATH_SEPERATOR);
  if (path->len > 100){
    return valid_files;
  }
  string_t base_path = new_string(path->len + path_sep.len);
  string_write(&base_path, path);
  string_write(&base_path, &path_sep);

  fprintf(logfile, " Base path: \"%s\" %d %d\n", cstr(&base_path), base_path.len, strlen(cstr(&base_path)));
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
        // Get all files from the sub dir with the matching extension
        alist_t sub_dir_files = dir_all_files_recur(&entry_path);
        sub_dir_files.destroy_on_remove = FALSE;
        closedir(d);
        fprintf(logfile, "back to \"%s\" %d\n", cstr(&base_path), base_path.len);

        // Add the files to the alist
        alist_combine(&valid_files, &sub_dir_files);
        alist_destroy(&sub_dir_files);

      } else {
        string_t name = dir_path_remove_start(&entry_path);
        alist_append(&valid_files, &name);
      }
    }
  }

  string_destroy(&base_path);
  string_destroy(&file_name);
  string_destroy(&entry_path);
  alist_destroy(&entries);

  return valid_files;
}

// Includes base path in return value, TODO look at dir_all_files_recur
// and use its solution using dir_path_remove_start
alist_t dir_files_with_extension_recur(string_t *path, string_t *extension) {
  // The return type is a list containing the paths to all matching files
  alist_t valid_files = new_alist(sizeof(string_t));
  valid_files.destroy = void_string_destroy;

  // Converting the system-specific path seperator into a string
  string_t base_path = new_string(path->len + strlen(PATH_SEPERATOR));
  string_write(&base_path, path);
  string_write_c(&base_path, PATH_SEPERATOR);

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
        // Get all files from the sub dir with the matching extension
        // Make sure to not destroy the strings on the destruction of
        // the alist returned
        alist_t sub_dir_files =
            dir_files_with_extension_recur(&entry_path, extension);
        closedir(d);

        // Add the files to the alist
        alist_combine(&valid_files, &sub_dir_files);
        alist_destroy(&sub_dir_files);

      } else {
        // Add the current file if it has the required extension
        string_t file_extension = get_file_extension(&file_name);

        if (string_equals(&file_extension, extension) == TRUE) {
          string_t name = dir_path_remove_start(&entry_path);
          alist_append(&valid_files, &name);
        }

        string_destroy(&file_extension);
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