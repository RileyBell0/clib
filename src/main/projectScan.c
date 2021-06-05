/*
 * Gets all 'c' files that are to be compiled
 */

#include "../std/configLoader.h"
#include "../std/dir/directory.h"
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

config_var_t *safe_cfg_get_var(config_t *config, char *name);
void add_src_files_from_dir(FILE *out_file, string_t *base_path,
                            string_t *path_sep, string_t *extension);

config_var_t *safe_cfg_get_var(config_t *config, char *name) {
  config_var_t *returned_var = config_get_var(config, name);
  if (!returned_var) {
    printf("ERROR - %s could not be found in config file\n", name);
    exit(EXIT_ERROR);
  }
  return returned_var;
}

void add_src_files_from_dir(FILE *out_file, string_t *base_path,
                            string_t *path_sep, string_t *extension) {
  alist_t files =
      dir_all_files_recur(base_path, fileio_has_extension_key, extension);

  // Print all files in the subdirectory
  alist_iterator_t it = new_alist_iterator(&files, TRUE);
  for (string_t *node = it.first(&it); !it.done(&it); node = it.next(&it)) {
    fprintf(out_file, "%s\n", cstr(node));
  }

  // alist_destroy(&files);
}

int main(int argc, char **argv) {
  // Check Arguments
  if (argc < REQUIRED_ARGS + 1) {
    printf("USAGE: %s  Path_To_Config\n", argv[0]);
    exit(EXIT_ERROR);
  }

  config_t cfg = read_config_file(argv[ARG_CONFIG]);

  // ------------ SETTING UP VARAIBLES
  config_var_t *var_src_dirs = safe_cfg_get_var(&cfg, VAR_SRC_DIRS);
  config_var_t *var_main_dirs = safe_cfg_get_var(&cfg, VAR_MAIN_DIRS);
  config_var_t *var_extension = safe_cfg_get_var(&cfg, VAR_EXTENSION);
  config_var_t *var_component_out = safe_cfg_get_var(&cfg, VAR_COMPONENT_OUT);
  config_var_t *var_main_out = safe_cfg_get_var(&cfg, VAR_MAIN_OUT);
  config_var_t *var_config_dir = safe_cfg_get_var(&cfg, VAR_CONFIG_DIR);

  // Making input args more usable
  string_t extension = *var_extension->data;
  string_t path_sep = string_make(PATH_SEPERATOR);
  string_t component_out = string_new_concat_multi(
      var_config_dir->data, &path_sep, var_component_out->data, NULL);
  string_t main_out = string_new_concat_multi(var_config_dir->data, &path_sep,
                                              var_main_out->data, NULL);

  // Finding and recording all program files
  FILE *main_out_file = fileio_open_safe(cstr(&main_out), FALSE);
  for (unsigned int i = 0; i < var_main_dirs->len; i++) {
    string_t dir_path = var_main_dirs->data[i];
    add_src_files_from_dir(main_out_file, &dir_path, &path_sep, &extension);
  }
  fclose(main_out_file);

  // Finding and recording all component files
  FILE *componentOut = fileio_open_safe(cstr(&component_out), FALSE);
  for (unsigned int i = 0; i < var_src_dirs->len; i++) {
    string_t componentDirPath = var_src_dirs->data[i];
    add_src_files_from_dir(componentOut, &componentDirPath, &path_sep, &extension);
  }
  fclose(componentOut);

  string_destroy(&component_out);
  string_destroy(&main_out);

  // Destroying the config
  config_destroy(cfg);

  return (0);
}