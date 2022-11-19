/*
 * Converts the generated files from 'glassCompile' and turns them into a
 * makefile
 */

#include "clib/struc/vector.h"
#include "clib/adv/config.h"
#include "clib/fileIO.h"
#include "clib/string.h"
#include "clib/sys/system.h"
#include <stdio.h>
#include <stdbool.h>

#define REQUIRED_ARGS 1
#define ARG_CONFIG_LOC 1

#define EXIT_ERROR 1
#define SPACE_FOR_PERIOD 1
#define MIN_LEN_FOR_EXTENSION 2
#define EXTRA_CHARS 9
#define DEFAULT_STR_LEN_PROJMAKE 16

// Variables expected in the config file
#define FLAG_INDICATOR "-"
#define VAR_OBJ_OUT "COMPONENT_OUT"
#define VAR_MAKE_NAME "MAKEFILE_NAME"
#define VAR_COMPILER "COMPILER"
#define VAR_FLAGS "FLAGS"
#define VAR_OBJ_FLAGS "OBJECT_FLAGS"
#define VAR_LINK "LINK"
#define VAR_DEBUG "DEBUG_FLAGS"
#define VAR_OBJ_EXT "OBJ_EXTENSION"
#define VAR_CFG_DIR "CONFIG_DIRECTORY"
#define VAR_PROG_FLAGS "PROGRAM_FLAGS"
#define VAR_PROG_FILES "PROGRAM_FILES"
#define VAR_COMPONENT_FILES "COMPONENT_FILES"
#define VAR_PROG_OUT "PROG_OUT"
#define VAR_COMPONENT_OUT "COMPONENT_OUT"

string_t load_flags_from_cfg(dict_t *cfg, char *var_name);
string_t *cfg_get_value(dict_t *cfg, char *key);
array_t *cfg_get_values(dict_t *cfg, char *key);

string_t *cfg_get_value(dict_t *cfg, char *key)
{
  return (string_t *)((array_t *)dict_get(cfg, key))->data;
}
array_t *cfg_get_values(dict_t *cfg, char *key)
{
  return (array_t *)dict_get(cfg, key);
}

// no leading or trailing whitespaces
string_t load_flags_from_cfg(dict_t *cfg, char *var_name)
{
  string_t flags = empty_string();

  array_t *vars = (array_t *)dict_get(cfg, var_name);
  for (size_t i = 0; i < vars->len; i++)
  {
    string_t *var = (string_t *)array_get(&vars, i);
    string_write_c(&flags, FLAG_INDICATOR);
    string_write(&flags, var);
    if (i != vars->len - 1)
    {
      string_write_c(&flags, " ");
    }
  }

  return flags;
}

int main(int argc, char **argv)
{
  if (argc < REQUIRED_ARGS + 1)
  {
    printf("USAGE:  %s  Path_To_Config  Is_Debugging\n", argv[0]);
    exit(EXIT_ERROR);
  }

  // Constants
  string_t flag_start = string_new("-");
  string_t path_sep = string_new(PATH_SEP);
  string_t ext_seperator = string_new(".");

  // Read in the cfg file
  dict_t config = config_read(argv[ARG_CONFIG_LOC]);
  dict_t *cfg = &config;

  // Extract variables from the config
  string_t *compiler = cfg_get_value(cfg, VAR_COMPILER);
  string_t *makefile_name = cfg_get_value(cfg, VAR_MAKE_NAME);
  string_t *prog_files_list_loc = cfg_get_value(cfg, VAR_PROG_FILES);
  string_t *obj_files_list_loc = cfg_get_value(cfg, VAR_COMPONENT_FILES);
  string_t common_flags = load_flags_from_cfg(cfg, VAR_FLAGS);
  string_t debug_flags = load_flags_from_cfg(cfg, VAR_DEBUG);
  string_t obj_flags = load_flags_from_cfg(cfg, VAR_OBJ_FLAGS);
  string_t prog_flags = load_flags_from_cfg(cfg, VAR_PROG_FLAGS);
  string_t *obj_out = cfg_get_value(cfg, VAR_OBJ_OUT);
  string_t *obj_ext = cfg_get_value(cfg, VAR_OBJ_EXT);
  string_t *prog_out = cfg_get_value(cfg, VAR_PROG_OUT);

  string_t component_files_path = string_concat_multi()
      string_write_multi(&component_files_path, var_cfg_dir->values.data, &path_sep,
                         var_obj_files->values.data, NULL);

  string_t prog_files_path = string_new(0);
  string_write_multi(&prog_files_path, var_cfg_dir->values.data, &path_sep,
                     var_prog_files->values.data, NULL);

  /*
   * Reading in from files
   */
  array_t prog_file_paths = fileio_read_all_lines(cstr(&prog_files_paths));
  alist_t obj_file_paths = fileio_read_all_lines(cstr(&component_files_path));

  /*
   * Parsing read data
   */
  alist_t prog_names = get_file_names_from_paths(&prog_file_paths);
  alist_t obj_names = get_file_names_from_paths(&obj_file_paths);
  remove_file_extensions(&prog_names);
  remove_file_extensions(&obj_names);

  // Write all obj names to a string, as need to use this multiple times
  string_t all_obj_names = string_new(DEFAULT_BUFFER_LEN);
  it = alist_iterator_new(&obj_names, true);
  string_t whitespace = string_make(" ");
  for (string_t *element = it.first(&it); !it.done(&it);
       element = it.next(&it))
  {
    string_write_multi(&all_obj_names, element, &ext_seperator, obj_ext,
                       &whitespace, NULL);
  }
  string_limit(&all_obj_names,
               all_obj_names.len - 1); // Remove the trailing whitespace

  // Write all object paths to a string, as need to use this multiple times
  string_t all_obj_paths = string_new(DEFAULT_BUFFER_LEN);
  it = alist_iterator_new(&obj_names, true);
  for (string_t *element = it.first(&it); !it.done(&it);
       element = it.next(&it))
  {
    string_write_multi(&all_obj_paths, obj_out, &path_sep, element,
                       &ext_seperator, obj_ext, &whitespace, NULL);
  }
  string_limit(&all_obj_paths,
               all_obj_paths.len - 1); // Remove the trailing whitespace

  string_t *compiler = var_compiler->values.data;

  string_t global_flags = string_new(DEFAULT_BUFFER_LEN);
  for (unsigned int i = 0; i < var_flags->values.len; i++)
  {
    string_write_multi(&global_flags, &flag_start,
                       &((string_t *)var_flags->values.data)[i], &whitespace,
                       NULL);
  }
  string_limit(&global_flags,
               global_flags.len - 1); // Remove the trailing whitespace

  string_t obj_flags = string_new(DEFAULT_BUFFER_LEN);
  for (unsigned int i = 0; i < var_obj_flags->values.len; i++)
  {
    string_write_multi(&obj_flags, &flag_start,
                       &((string_t *)var_obj_flags->values.data)[i],
                       &whitespace, NULL);
  }
  string_limit(&obj_flags,
               obj_flags.len - 1); // Remove the trailing whitespace

  string_t prog_flags = string_new(DEFAULT_BUFFER_LEN);
  for (unsigned int i = 0; i < var_prog_flags->values.len; i++)
  {
    string_write_multi(&prog_flags, &flag_start, &((string_t *)var_prog_flags->values.data)[i],
                       &whitespace, NULL);
  }
  string_limit(&prog_flags,
               prog_flags.len - 1); // Remove the trailing whitespace

  string_t debug_flags = string_new(DEFAULT_BUFFER_LEN);
  for (unsigned int i = 0; i < var_debug->values.len; i++)
  {
    string_write_multi(&debug_flags, &flag_start, &((string_t *)var_debug->values.data)[i],
                       &whitespace, NULL);
  }
  string_limit(&debug_flags,
               debug_flags.len - 1); // Remove the trailing whitespace

  // Create the makefile's file
  FILE *out_file = fileio_open_safe(cstr(var_makeName->values.data), false);

  // Write the all call
  write_make_all_call(out_file, &prog_names); // all: prog_1 prog_2 ... prog_n
  fprintf(out_file, "\n");

  // Writing the make instructions for the programs
  it = alist_iterator_new(&prog_names, true);
  for (string_t *prog_name = it.first(&it); !it.done(&it);
       prog_name = it.next(&it))
  {
    fprintf(out_file, "%s: %s%s%s %s\n", cstr(prog_name), cstr(prog_name),
            cstr(&ext_seperator), cstr(obj_ext), cstr(&all_obj_names));
    fprintf(out_file, "\t%s %s %s %s%s%s %s%s%s%s%s %s %s\n\n", cstr(compiler),
            cstr(&global_flags), cstr(&prog_flags), cstr(var_prog_out->values.data),
            cstr(&path_sep), cstr(prog_name), cstr(obj_out), cstr(&path_sep),
            cstr(prog_name), cstr(&ext_seperator), cstr(obj_ext),
            cstr(&all_obj_paths), cstr(&debug_flags));
  }

  // Writing the make instructions for the program object files
  it = alist_iterator_new(&prog_names, true);
  it_2 = alist_iterator_new(&prog_file_paths, true);
  it_2.first(&it_2);
  for (string_t *prog_name = it.first(&it); !it.done(&it);
       prog_name = it.next(&it))
  {
    // Write call line
    fprintf(out_file, "%s%s%s: %s\n", cstr(prog_name), cstr(&ext_seperator),
            cstr(obj_ext), cstr(it_2.element));

    // Write cmd line
    fprintf(out_file, "\t%s %s %s %s%s%s%s%s %s %s\n\n", cstr(compiler),
            cstr(&global_flags), cstr(&obj_flags), cstr(obj_out),
            cstr(&path_sep), cstr(prog_name), cstr(&ext_seperator),
            cstr(obj_ext), cstr(it_2.element), cstr(&debug_flags));

    it_2.next(&it_2);
  }

  // Writing the make instructions for the component obj files
  it = alist_iterator_new(&obj_names, true);
  it_2 = alist_iterator_new(&obj_file_paths, true);
  it_2.first(&it_2);
  for (string_t *obj_name = it.first(&it); !it.done(&it);
       obj_name = it.next(&it))
  {
    fprintf(out_file, "%s%s%s: %s\n", cstr(obj_name), cstr(&ext_seperator),
            cstr(obj_ext), cstr(it_2.element));
    fprintf(out_file, "\t%s %s %s %s%s%s%s%s %s %s\n\n", cstr(compiler),
            cstr(&global_flags), cstr(&obj_flags), cstr(obj_out),
            cstr(&path_sep), cstr(obj_name), cstr(&ext_seperator),
            cstr(obj_ext), cstr(it_2.element), cstr(&debug_flags));
    it_2.next(&it_2);
  }

  // Writing the clean instructions
  fprintf(out_file, "clean:\n\trm %s%s*%s", cstr(var_obj_out->values.data),
          PATH_SEPERATOR, cstr(&extension));
  fprintf(out_file, " %s%s*\n", cstr(var_prog_out->values.data), PATH_SEPERATOR);

  fclose(out_file);

  // Cleanup
  string_destroy(&debug_flags);
  string_destroy(&prog_flags);
  string_destroy(&obj_flags);
  string_destroy(&global_flags);
  string_destroy(&all_obj_paths);
  string_destroy(&whitespace);
  string_destroy(&all_obj_names);
  string_destroy(&prog_files_path);
  string_destroy(&component_files_path);
  string_destroy(&extension);
  string_destroy(&ext_seperator);
  string_destroy(&path_sep);
  string_destroy(&flag_start);

  config_destroy(cfg);

  return (0);
}
