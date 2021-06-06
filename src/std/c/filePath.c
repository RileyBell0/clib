#include "../filePath.h"

string_t path_enter_directory(string_t base_path, string_t dir_name) {
  string_t path_seperator = string_make(PATH_SEPERATOR);
  string_t src_files =
      new_string(base_path.len + path_seperator.len + dir_name.len);

  string_write_multi(&src_files, &base_path, &path_seperator, &dir_name, NULL);

  return src_files;
}