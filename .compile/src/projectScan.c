/*
 * Gets all 'c' files that are to be compiled
 */

#include "clib/fileIO.h"
#include "clib/string.h"
#include "clib/string.h"
#include "clib/adv/config.h"
#include "clib/sys/directory.h"
#include "clib/sys/system.h"
#include "clib/struc/filter.h"
#include "clib/struc/dict.h"
#include "clib/struc/array.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define REQUIRED_ARGS 1
#define ARG_CONFIG 1

#define VAR_SRC_DIRS "SRC_DIRS"
#define VAR_MAIN_DIRS "MAIN_DIRS"
#define VAR_EXTENSION "PROGRAM_TYPE"
#define VAR_COMPONENT_OUT "COMPONENT_OUT"
#define VAR_MAIN_OUT "MAIN_OUT"
#define VAR_CONFIG_DIR "CONFIG_DIR"

#define EXIT_ERROR 1
#define SPACE_FOR_PERIOD 1
#define MIN_LEN_FOR_EXTENSION 2

bool extension_equals(string_t *path, string_t *extension);
bool extension_filter(void *elem, void *extra);
void add_src_files_from_dir(FILE *out_file, string_t *base_path, string_t *extension);
string_t *cfg_get_value(dict_t *cfg, char *key);
array_t *cfg_get_values(dict_t *cfg, char *key);

bool extension_equals(string_t *path, string_t *extension)
{
  // Try and extract the extension from the file "path"
  bool found_extension = false;
  size_t extension_pos = path->len - 2; //  one char before the last char
  char *str = cstr(path);
  for (; extension_pos > 0; extension_pos--)
  {
    if (str[extension_pos] == '.')
    {
      found_extension = true;
      break;
    }
  }

  // Determine if the file extension matches the requested
  if (found_extension)
  {
    string_t path_extension = string_new(&str[extension_pos + 1]);
    bool equal = string_equals(&path_extension, extension);
    string_destroy(&path_extension);

    return equal;
  }
  else
  {
    return false;
  }
}

bool extension_filter(void *elem, void *extra)
{
  string_t *path = (string_t *)elem;
  string_t *extension = (string_t *)extra;
  return extension_equals(path, extension);
}

void add_src_files_from_dir(FILE *out_file, string_t *base_path, string_t *extension)
{
  // Get all files
  array_t files = dir_all_files_recur(cstr(base_path));
  array_filter(&files, extension_filter, extension);

  for (size_t i = 0; i < files.len; i++)
  {
    string_t *file = (string_t *)array_get(&files, i);
    fprintf(out_file, "%s\n", cstr(file));
  }

  array_destroy(&files);
}

string_t *cfg_get_value(dict_t *cfg, char *key)
{
  return (string_t *)((array_t *)dict_get(cfg, key))->data;
}
array_t *cfg_get_values(dict_t *cfg, char *key)
{
  return (array_t *)dict_get(cfg, key);
}

int main(int argc, char **argv)
{
  // Check Arguments
  if (argc < REQUIRED_ARGS + 1)
  {
    printf("USAGE: %s Path_To_Config\n", argv[0]);
    exit(EXIT_ERROR);
  }

  dict_t config = config_read(argv[ARG_CONFIG]);
  dict_t *cfg = &config;

  // ------------ SETTING UP VARAIBLES
  array_t *src_dirs = cfg_get_values(cfg, VAR_SRC_DIRS);
  array_t *main_dirs = cfg_get_values(cfg, VAR_MAIN_DIRS);

  string_t *extension = cfg_get_value(cfg, VAR_EXTENSION);
  string_t *component_out = cfg_get_value(cfg, VAR_COMPONENT_OUT);
  string_t *main_out = cfg_get_value(cfg, VAR_MAIN_OUT);
  string_t *config_dir = cfg_get_value(cfg, VAR_CONFIG_DIR);

  // Making input args more usable
  string_t path_sep = string_new(PATH_SEP);
  string_t components_file_loc = string_concat_multi(config_dir, &path_sep, component_out, NULL);
  string_t programs_file_loc = string_concat_multi(config_dir, &path_sep, main_out, NULL);

  // Discover and write the paths to all component files
  FILE *components_file = fileio_open_safe(cstr(&components_file_loc), false);
  for (size_t i = 0; i < src_dirs->len; i++)
  {
    string_t *component_dir_base_path = (string_t *)array_get(src_dirs, i);
    add_src_files_from_dir(components_file, component_dir_base_path,
                           extension);
  }
  fclose(components_file);

  // Discover and write the paths to all program files
  FILE *programs_file = fileio_open_safe(cstr(&programs_file_loc), false);
  for (size_t i = 0; i < main_dirs->len; i++)
  {
    string_t *program_dir_base_path = (string_t *)array_get(main_dirs, i);
    add_src_files_from_dir(programs_file, program_dir_base_path, extension);
  }
  fclose(programs_file);

  // Cleanup
  string_destroy(&path_sep);
  string_destroy(&components_file_loc);
  string_destroy(&programs_file_loc);
  dict_destroy(cfg);

  return 0;
}