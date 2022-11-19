/*
 * Converts the generated files from 'glassCompile' and turns them into a
 * makefile
 */

#include "../std/struc/vector.h"
#include "../std/adv/config.h"
#include "../std/fileIO.h"
#include "../std/string.h"
#include "../std/sys/system.h"
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
    string_t *var = (string_t *)array_get(vars, i);
    string_append_c(&flags, FLAG_INDICATOR);
    string_append(&flags, var);
    if (i != vars->len - 1)
    {
      string_append_c(&flags, " ");
    }
  }

  return flags;
}

string_t get_file_name(string_t *path)
{
  // Determine where the file name starts
  size_t start = path->len - 1;
  char *str = cstr(path);
  for (; start >= 0; start--)
  {
    char c = str[start];
    if (c == '/')
    {
      start += 1;
      break;
    }
  }
  if (start <= 0)
  {
    return string_copy(path);
  }

  // Trim off the start the string, copy and return it
  return string_new(&str[start]);
}

void remove_extension(string_t *file)
{
  // Determine where the extension starts
  char *str = cstr(file);
  size_t i = file->len - 2;
  for (; i > 0; i--)
  {
    char c = str[i];
    if (c == '.')
    {
      break;
    }
  }

  // if an extension is present, remove it
  if (i > 0)
  {
    string_limit(file, i);
  }
}

array_t get_file_names(array_t *files)
{
  array_t names = array_new(files->len, files->element_size, files->destroy);
  for (size_t i = 0; i < files->len; i++)
  {
    string_t name = get_file_name((string_t *)array_get(files, i));
    remove_extension(&name);
    array_set(&names, i, &name);
  }
  return names;
}

int main(int argc, char **argv)
{
  if (argc < REQUIRED_ARGS + 1)
  {
    printf("USAGE:  %s  Path_To_Config  Is_Debugging\n", argv[0]);
    exit(EXIT_ERROR);
  }

  // Read in the cfg file
  dict_t config = config_read(argv[ARG_CONFIG_LOC]);
  dict_t *cfg = &config;

  // Extract variables from the config
  string_t *compiler = cfg_get_value(cfg, VAR_COMPILER);
  string_t *makefile_name = cfg_get_value(cfg, VAR_MAKE_NAME);
  string_t *programs = cfg_get_value(cfg, VAR_PROG_FILES);
  string_t *components = cfg_get_value(cfg, VAR_COMPONENT_FILES);
  string_t common_flags = load_flags_from_cfg(cfg, VAR_FLAGS);
  string_t debug_flags = load_flags_from_cfg(cfg, VAR_DEBUG);
  string_t obj_flags = load_flags_from_cfg(cfg, VAR_OBJ_FLAGS);
  string_t prog_flags = load_flags_from_cfg(cfg, VAR_PROG_FLAGS);
  string_t *obj_out = cfg_get_value(cfg, VAR_OBJ_OUT);
  string_t *obj_ext = cfg_get_value(cfg, VAR_OBJ_EXT);
  string_t *prog_out = cfg_get_value(cfg, VAR_PROG_OUT);

  // Determine the location of the files containing prog and src file names
  string_t component_files_path = string_new(".compile/output/");
  string_append(&component_files_path, components);
  string_t prog_files_path = string_new(".compile/output/");
  string_append(&prog_files_path, programs);

  // Load component and program file names
  array_t prog_files = fileio_read_all_lines(cstr(&prog_files_path));
  if (prog_files.len == 0)
  {
    FILE *makefile = fileio_open_safe(cstr(makefile_name), false);
    fclose(makefile);
    return 0;
  }

  array_t component_files = fileio_read_all_lines(cstr(&component_files_path));

  // Extract just the file names
  array_t prog_names = get_file_names(&prog_files);
  array_t component_names = get_file_names(&component_files);

  // Combine all obj names (with obj extension), separated by whitespace
  string_t all_obj_names = empty_string();
  for (size_t i = 0; i < component_names.len; i++)
  {
    string_t *name = (string_t *)array_get(&component_names, i);
    string_append(&all_obj_names, name);
    string_append_c_multi(&all_obj_names, ".", cstr(obj_ext), NULL);
    if (i != component_names.len - 1)
    {
      string_append_char(&all_obj_names, ' ');
    }
  }

  // Combine all object paths, separated by whitespace
  string_t all_obj_paths = empty_string();
  for (size_t i = 0; i < component_names.len; i++)
  {
    string_t *name = (string_t *)array_get(&component_names, i);
    string_append_c_multi(&all_obj_paths, cstr(obj_out), "/", cstr(name), ".", cstr(obj_ext), NULL);
    if (i != component_names.len - 1)
    {
      string_append_char(&all_obj_paths, ' ');
    }
  }

  // Create the makefile's file
  FILE *makefile = fileio_open_safe(cstr(makefile_name), false);

  // Write the "all" call
  fprintf(makefile, "all: ");
  for (size_t i = 0; i < prog_names.len; i++)
  {
    string_t *name = (string_t *)array_get(&prog_names, i);
    fprintf(makefile, "%s ", cstr(name));
  }
  fprintf(makefile, "\n\n");

  // Writing the make instructions for the programs
  for (size_t i = 0; i < prog_names.len; i++)
  {
    string_t *name = (string_t *)array_get(&prog_names, i);
    fprintf(makefile, "%s: %s.%s %s\n", cstr(name), cstr(name), cstr(obj_ext), cstr(&all_obj_names));
    fprintf(makefile, "\t%s %s %s %s/%s %s/%s.%s %s %s\n\n", cstr(compiler), cstr(&common_flags), cstr(&prog_flags), cstr(prog_out), cstr(name), cstr(obj_out), cstr(name), cstr(obj_ext), cstr(&all_obj_paths), cstr(&debug_flags));
  }

  // Writing the make instructions for the program object files
  for (size_t i = 0; i < prog_names.len; i++)
  {
    string_t *name = (string_t *)array_get(&prog_names, i);
    string_t *path = (string_t *)array_get(&prog_files, i);
    fprintf(makefile, "%s.%s: %s\n", cstr(name), cstr(obj_ext), cstr(path));
    fprintf(makefile, "\t%s %s %s %s/%s.%s %s %s\n\n", cstr(compiler), cstr(&common_flags), cstr(&obj_flags), cstr(obj_out), cstr(name), cstr(obj_ext), cstr(path), cstr(&debug_flags));
  }

  // Writing the make instructions for the component obj files
  for (size_t i = 0; i < component_names.len; i++)
  {
    string_t *name = (string_t *)array_get(&component_names, i);
    string_t *path = (string_t *)array_get(&component_files, i);
    fprintf(makefile, "%s.%s: %s\n", cstr(name), cstr(obj_ext), cstr(path));
    fprintf(makefile, "\t%s %s %s %s/%s.%s %s %s\n\n", cstr(compiler), cstr(&common_flags), cstr(&obj_flags), cstr(obj_out), cstr(name), cstr(obj_ext), cstr(path), cstr(&debug_flags));
  }

  // Writing the clean instructions
  fprintf(makefile, "clean:\n");
  fprintf(makefile, "\trm %s/*%s %s/*\n", cstr(obj_out), cstr(obj_ext), cstr(prog_out));

  fclose(makefile);

  return (0);
}
