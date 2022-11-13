#include "../directory.h"

/*
 * Given a path such as 'cool/awesome/file.txt'
 * returns a string containing 'awesome/file.txt'
 */
string_t dir_path_remove_start(string_t *path)
{
  if (path->len == 0)
  {
    return string_new(0);
  }

  /*
   * Find and return a string containing the entire path except
   * for the starting directory
   */
  char *pathstr = cstr(path);
  unsigned int pathsep_len = strlen(PATH_SEPERATOR);
  for (unsigned int c = 0; c < path->len; c++)
  {
    if (cstring_equals_range(&pathstr[c], PATH_SEPERATOR, pathsep_len))
    {
      c += pathsep_len;
      return string_make(&pathstr[c]);
    }
  }

  return string_new(0);
}

DIR *dir_open_safe_c(char *path)
{
  DIR *d = opendir(path);

  if (!d)
  {
    exit_error("Could not open directory", "std/dir/c/directory.c",
               "dir_open_safe_c");
  }

  return d;
}

DIR *dir_open_safe(string_t *path)
{
  return dir_open_safe_c(cstr(path));
}

// RE-CHECKED 04/05/2021
// MEMORY_SAFE 04/05/2021
alist_t dir_all_entries_alist(string_t *path)
{
  struct dirent *entry;
  alist_t entries = alist_new(sizeof(struct dirent));

  // Read all dirents into the alist entries
  DIR *d = dir_open_safe(path);
  while ((entry = readdir(d)))
  {
    alist_append(&entries, entry);
  }
  closedir(d);

  return entries;
}

alist_t dir_all_entry_names(string_t *path,
                            int (*key)(string_t *entry_name, void *extra),
                            void *extra)
{
  struct dirent *entry;
  alist_t files = alist_new(sizeof(string_t));

  DIR *d = dir_open_safe(path);
  while ((entry = readdir(d)))
  {
    string_t entry_name = string_make(entry->d_name);

    if (!key || (key && key(&entry_name, extra) == 0))
    {
      alist_append(&files, &entry_name);
    }
    else
    {
      string_destroy(&entry_name);
    }
  }

  return files;
}

bool dir_entry_is_reg_file(char *entry_name)
{
  struct stat buffer;

  if (lstat(entry_name, &buffer) == -1)
  {
    if (errno != EACCES)
    {
      string_t message = string_make("Error - error running lstat on \"");
      string_write_c(&message, entry_name);
      string_write_c(&message, "\"");

      exit_error(cstr(&message), "std/dir/c/directory.c",
                 "dir_all_file_names");
    }
    else
    {
      string_t message = string_make("Error - could not access file \'");
      string_write_c(&message, entry_name);
      string_write_c(&message, "\n");

      exit_error(cstr(&message), "std/dir/c/directory.c",
                 "dir_all_file_names");
    }
  }

  return S_ISREG(buffer.st_mode);
}

alist_t dir_all_file_names(string_t *path,
                           int (*key)(string_t *file_name, void *extra),
                           void *extra)
{
  struct dirent *entry;
  alist_t files = alist_new(sizeof(string_t));

  string_t path_sep = string_make(PATH_SEPERATOR);

  string_t base_path = string_new(path->len + path_sep.len);
  string_write(&base_path, path);
  string_write(&base_path, &path_sep);

  string_t entry_path = string_new(base_path.len);
  string_write(&entry_path, &base_path);

  DIR *d = dir_open_safe(path);
  while ((entry = readdir(d)))
  {
    string_limit(&entry_path, base_path.len);
    string_write_c(&entry_path, entry->d_name);
    if (dir_entry_is_reg_file(cstr(&entry_path)))
    {
      string_t file_name = string_make(entry->d_name);
      if (!key || (key && key(&file_name, extra) == 0))
      {
        alist_append(&files, &file_name);
      }
      else
      {
        string_destroy(&file_name);
      }
    }
  }

  return files;
}

alist_t dir_all_files_recur(string_t *path,
                            int (*key)(string_t *file_name, void *extra),
                            void *extra, bool include_base_path)
{
  // The return type is a list containing the paths to all matching files
  alist_t valid_files = alist_new(sizeof(string_t));
  valid_files.destroy = void_string_destroy;

  string_t path_sep = string_make(PATH_SEPERATOR);

  string_t base_path = string_new(path->len + path_sep.len);
  string_write(&base_path, path);
  string_write(&base_path, &path_sep);

  // Getting all the directory entries in the current directory
  alist_t entries = dir_all_entries_alist(path);

  string_t file_name = string_new(0);
  string_t entry_path = string_new(base_path.len);
  string_write(&entry_path, &base_path);

  alist_iterator_t it = alist_iterator_new(&entries, true);
  for (struct dirent *entry = it.first(&it); !it.done(&it);
       entry = it.next(&it))
  {
    // Generate the string for the entry-name
    string_clear(&file_name);
    string_write_c(&file_name, entry->d_name);

    if (!is_relative_dir_entry(&file_name))
    {
      // Generate the whole path to the entry
      string_limit(&entry_path, base_path.len);
      string_write(&entry_path, &file_name);

      if (dir_entry_is_reg_file(cstr(&entry_path)))
      {
        bool append = true;
        if (key && !key(&file_name, extra))
        {
          append = false;
        }

        if (append)
        {
          if (include_base_path == true)
          {
            string_t name = string_copy(&entry_path);
            alist_append(&valid_files, &name);
          }
          else
          {
            string_t name = dir_path_remove_start(&entry_path);
            alist_append(&valid_files, &name);
          }
        }
      }
      else
      {
        // Add all files from the sub directory
        DIR *d = opendir(cstr(&entry_path));
        if (d)
        {
          // Get all files from the sub dir which are valid based
          // on the key functiono
          alist_t sub_dir_files =
              dir_all_files_recur(&entry_path, key, extra, include_base_path);
          sub_dir_files.destroy_on_remove = false;
          closedir(d);

          // Add the files to the alist
          alist_combine(&valid_files, &sub_dir_files);
          alist_destroy(&sub_dir_files);
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
bool is_relative_dir_entry(string_t *path)
{
  if (path->len == 1)
  {
    if (cstring_equals(cstr(path), "."))
    {
      return true;
    }
  }
  else if (path->len == 2)
  {
    if (cstring_equals(cstr(path), ".."))
    {
      return true;
    }
  }
  return false;
}