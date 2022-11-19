#include "../directory.h"

static vector_t dir_all_files_recur_vector(char *path);
static DIR *dir_open_safe(char *path);
static bool is_relative_dir_entry(char *path);
bool dir_entry_is_reg_file(char *entry_name);

static DIR *dir_open_safe(char *path)
{
  DIR *d = opendir(path);

  if (!d)
  {
    exit_error("Could not open directory", "std/dir/c/directory.c",
               "dir_open_safe_c");
  }

  return d;
}

array_t dir_all_entries(char *path)
{
  // Read all dirents from the current directory
  struct dirent *entry;
  vector_t entries = vector_new(sizeof(struct dirent), NULL);
  DIR *d = dir_open_safe(path);
  while ((entry = readdir(d)))
  {
    vector_append(&entries, entry);
  }
  closedir(d);

  // Convert results into an array
  array_t res = vector_to_array(&entries);
  vector_destroy(&entries);

  return res;
}

array_t dir_all_files_recur(char *path)
{
  // Get files, and hook up the string destroy function
  vector_t files = dir_all_files_recur_vector(path);
  array_t results = vector_to_array(&files);
  results.destroy = void_string_destroy;

  // Cleanup and destroy
  vector_destroy(&files);
  return results;
}

static vector_t dir_all_files_recur_vector(char *path)
{
  vector_t files = vector_new(sizeof(string_t), NULL);

  size_t path_len = strlen(path);
  string_t entry_path = string_new(path);

  // Getting all the directory entries in the current directory
  array_t entries = dir_all_entries(path);

  for (size_t i = 0; i < entries.len; i++)
  {
    struct dirent *entry = (struct dirent *)array_get(&entries, i);
    string_limit(&entry_path, path_len);
    string_append_c_multi(&entry_path, PATH_SEP, entry->d_name, NULL);

    // Save all contained files to "files"
    if (!is_relative_dir_entry(entry->d_name))
    {
      if (dir_entry_is_reg_file(cstr(&entry_path)))
      {
        string_t filename = string_copy(&entry_path);
        vector_append(&files, &filename);
      }
      else
      {
        // Add all files from the sub directory
        DIR *d = opendir(cstr(&entry_path));
        if (d)
        {
          // Save all files from the sub directory to the files vector
          closedir(d);
          vector_t sub_dir_files = dir_all_files_recur_vector(cstr(&entry_path));
          vector_combine(&files, &sub_dir_files);
          vector_destroy(&sub_dir_files);
        }
      }
    }
  }

  string_destroy(&entry_path);
  array_destroy(&entries);

  return files;
}

bool dir_entry_is_reg_file(char *entry_name)
{
  struct stat buffer;

  if (lstat(entry_name, &buffer) == -1)
  {
    if (errno != EACCES)
    {
      string_t message = string_new("Error - error running lstat on \"");
      string_append_c(&message, entry_name);
      string_append_c(&message, "\"");

      exit_error(cstr(&message), "std/dir/c/directory.c",
                 "dir_all_file_names");
    }
    else
    {
      string_t message = string_new("Error - could not access file \'");
      string_append_c(&message, entry_name);
      string_append_c(&message, "\n");

      exit_error(cstr(&message), "std/dir/c/directory.c",
                 "dir_all_file_names");
    }
  }

  return S_ISREG(buffer.st_mode);
}

static bool is_relative_dir_entry(char *path)
{
  size_t len = strlen(path);
  if (len == 1 && strncmp(path, ".", 1) == 0)
  {
    return true;
  }
  else if (len == 2 && strncmp(path, "..", 2) == 0)
  {
    return true;
  }

  return false;
}