#include "../directory.h"

// TODO why did i think this method was clunky? i like it now
alist_t dir_all_entries_alist(string_t *path) {
  DIR *d = opendir(cstr(path));
  alist_t entries = new_alist(sizeof(struct dirent));

  if (!d) {
    return entries;
  }

  struct dirent *entry;

  // there's a good chance the filepath may be longer than the small string
  // size, so pre-allocating it
  string_t file_path = new_string(DEFAULT_BUFFER_LEN);
  string_write(&file_path, path);
  string_write_c(&file_path, PATH_SEPERATOR);

  while ((entry = readdir(d))) {
    // Copy the contents of 'entry'
    alist_append(&entries, entry);
  }

  // CLEANUP
  closedir(d);
  string_destroy(&file_path);

  return entries;
}

// RE-CHECKED 04/05/2021
alist_t dir_all_files_recur(string_t* path) {
  // The return type is a list containing the paths to all matching files
  alist_t valid_files = new_alist(sizeof(string_t));

  // Converting the system-specific path seperator into a string
  string_t base_path = new_string(path->len + strlen(PATH_SEPERATOR));
  string_write(&base_path, path);
  string_write_c(&base_path, PATH_SEPERATOR);
  printf("\tbase path: %s\n", cstr(&base_path));

  // Getting all the directory entries in the current directory
  alist_t entries = dir_all_entries_alist(path);
  string_t file_name = new_string(0);

  alist_iterator_t it = new_alist_iterator(&entries, TRUE);
  for (struct dirent *entry = it.first(&it); !it.done(&it);
       entry = it.next(&it)) {

    // Generate the string for the entry-name
    string_clear(&file_name);
    string_write_c(&file_name, entry->d_name);

    // Generate the whole path to the entry
    string_t entry_path = new_string(base_path.len + file_name.len);
    string_write(&entry_path, &base_path);
    string_write(&entry_path, &file_name);

    if (!is_relative_dir_entry(&file_name)) {
      // Add all files from the sub directory
      DIR *d = opendir(cstr(&entry_path));
      if (d) {
        // Get all files from the sub dir with the matching extension
        alist_t sub_dir_files =
            dir_all_files_recur(&entry_path);

        // Add the files to the alist
        alist_combine(&valid_files, &sub_dir_files);

        closedir(d);

        string_destroy(&entry_path);
      } else {
        alist_append(&valid_files, &entry_path);
        printf("\t\tAdding: %s\n", cstr(&entry_path));
      }
    }
    else {
      string_destroy(&entry_path);
    }
  }

  string_destroy(&file_name);

  return valid_files;
}

// RE-CHECKED 04/05/2021
alist_t dir_files_with_extension_recur(string_t *path, string_t *extension) {
  // The return type is a list containing the paths to all matching files
  alist_t valid_files = new_alist(sizeof(string_t));

  // Converting the system-specific path seperator into a string
  string_t base_path = new_string(path->len + strlen(PATH_SEPERATOR));
  string_write(&base_path, path);
  string_write_c(&base_path, PATH_SEPERATOR);

  // Getting all the directory entries in the current directory
  alist_t entries = dir_all_entries_alist(path);
  string_t file_name = new_string(0);

  alist_iterator_t it = new_alist_iterator(&entries, TRUE);
  for (struct dirent *entry = it.first(&it); !it.done(&it);
       entry = it.next(&it)) {

    // Generate the string for the entry-name
    string_clear(&file_name);
    string_write_c(&file_name, entry->d_name);

    // Generate the whole path to the entry
    string_t entry_path = new_string(base_path.len + file_name.len);
    string_write(&entry_path, &base_path);
    string_write(&entry_path, &file_name);

    if (!is_relative_dir_entry(&file_name)) {
      // Add all files from the sub directory
      DIR *d = opendir(cstr(&entry_path));
      if (d) {
        // Get all files from the sub dir with the matching extension
        alist_t sub_dir_files =
            dir_files_with_extension_recur(&entry_path, extension);

        // Add the files to the alist
        alist_combine(&valid_files, &sub_dir_files);

        closedir(d);

        string_destroy(&entry_path);
      } else {
        // Add the current file if it has the required extension
        string_t file_extension = get_file_extension(&file_name);

        if (string_equals(&file_extension, extension) == TRUE) {
          alist_append(&valid_files, &entry_path);
        }
        else {
          string_destroy(&entry_path);
        }
      }
    }
    else {
      string_destroy(&entry_path);
    }
  }

  string_destroy(&file_name);

  return valid_files;
}

// RE-CHECKED 04/05/2021
int is_relative_dir_entry(string_t *dirName) {
  if (dirName->len == 1) {
    if (cstring_equals(cstr(dirName), ".")) {
      return FALSE;
    }
  } else if (dirName->len == 2) {
    if (cstring_equals(cstr(dirName), "..")) {
      return FALSE;
    }
  }
  return TRUE;
}