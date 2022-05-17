/*
 * Converts the generated files from 'glassCompile' and turns them into a
 * makefile
 */

#include "../std/struc/vector.h"
#include "../std/adv/argparse.h"
#include "../std/adv/config.h"
#include "../std/fileIO.h"
#include "../std/path.h"
#include "../std/string.h"
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

typedef struct make_instructions_t {
  string_t compiler;  // The name of the compiler to use
  string_t makefile_name;

  string_t prog_files_list_loc;
  string_t obj_files_list_loc;

  string_t common_flags; // Commmon flags
  string_t prog_flags; // Flags specific for the program
  string_t obj_flags; // Flags specific for objects
  string_t debug_flags; // Debug specific flags

  string_t obj_ext; // The extension to give the compiled objects

  string_t obj_out; // Where to put the compiled objects
} make_instructions_t;

string_t* get_val_from_cfg(config_t* cfg, char* var_name);
string_t load_flags_from_cfg(config_t* cfg, char* var_name);
make_instructions_t load_make_instructions(config_t* cfg);


/*
 * Program Entry Point
 */
int main(int argc, char **argv) {
  if (argc < REQUIRED_ARGS + 1) {
    printf("USAGE:  %s  Path_To_Config  Is_Debugging\n", argv[0]);
    exit(EXIT_ERROR);
  }

  // Constants
  string_t flag_start = string_make("-");
  string_t path_sep = string_make(PATH_SEPERATOR);
  string_t ext_seperator = string_make("."); // Extension seperator

  // Read in the cfg file
  config_t cfg = config_read(argv[ARG_CONFIG_LOC]);
  make_instructions_t instr = load_make_instructions(&cfg);

  string_t component_files_path = string_new(0);.compile/cfg/
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
       element = it.next(&it)) {
    string_write_multi(&all_obj_names, element, &ext_seperator, obj_ext,
                       &whitespace, NULL);
  }
  string_limit(&all_obj_names,
               all_obj_names.len - 1); // Remove the trailing whitespace

  // Write all object paths to a string, as need to use this multiple times
  string_t all_obj_paths = string_new(DEFAULT_BUFFER_LEN);
  it = alist_iterator_new(&obj_names, true);
  for (string_t *element = it.first(&it); !it.done(&it);
       element = it.next(&it)) {
    string_write_multi(&all_obj_paths, obj_out, &path_sep, element,
                       &ext_seperator, obj_ext, &whitespace, NULL);
  }
  string_limit(&all_obj_paths,
               all_obj_paths.len - 1); // Remove the trailing whitespace

  string_t *compiler = var_compiler->values.data;

  string_t global_flags = string_new(DEFAULT_BUFFER_LEN);
  for (unsigned int i = 0; i < var_flags->values.len; i++) {
    string_write_multi(&global_flags, &flag_start,
                       &((string_t *)var_flags->values.data)[i], &whitespace,
                       NULL);
  }
  string_limit(&global_flags,
               global_flags.len - 1); // Remove the trailing whitespace

  string_t obj_flags = string_new(DEFAULT_BUFFER_LEN);
  for (unsigned int i = 0; i < var_obj_flags->values.len; i++) {
    string_write_multi(&obj_flags, &flag_start,
                       &((string_t *)var_obj_flags->values.data)[i],
                       &whitespace, NULL);
  }
  string_limit(&obj_flags,
               obj_flags.len - 1); // Remove the trailing whitespace

  string_t prog_flags = string_new(DEFAULT_BUFFER_LEN);
  for (unsigned int i = 0; i < var_prog_flags->values.len; i++) {
    string_write_multi(&prog_flags, &flag_start, &((string_t*)var_prog_flags->values.data)[i],
                       &whitespace, NULL);
  }
  string_limit(&prog_flags,
               prog_flags.len - 1); // Remove the trailing whitespace

  string_t debug_flags = string_new(DEFAULT_BUFFER_LEN);
  for (unsigned int i = 0; i < var_debug->values.len; i++) {
    string_write_multi(&debug_flags, &flag_start, &((string_t*)var_debug->values.data)[i],
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
       prog_name = it.next(&it)) {
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
       prog_name = it.next(&it)) {
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
       obj_name = it.next(&it)) {
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


string_t* get_val_from_cfg(config_t* cfg, char* var_name) {
  array_t result = config_get(cfg, var_name);
  if (result.len == 0) {
    fprintf(stderr, "Error - \"%s\" not in config %s\n", var_name, cstr(&cfg->path_to_cfg));
    exit_error("Value not in config", "projectMake.c", "get_val_from_cfg");
  }

  return (string_t*)array_get(&result, 0);
}

// There will be a trailing whitespace in the resultant string
string_t load_flags_from_cfg(config_t* cfg, char* var_name) {
  string_t flags = string_new(0);

  array_t vars = config_get(cfg, var_name);
  for (int i = 0; i < vars.len; i++) {
    string_t* var = array_get(&vars, i);
    string_write_c(&flags, FLAG_INDICATOR);
    string_write(&flags, var);
    string_write_c(&flags, " ");
  }

  return flags;
}

make_instructions_t load_make_instructions(config_t* cfg) {
  make_instructions_t instr;
  instr.compiler = string_copy(get_val_from_cfg(cfg, VAR_COMPILER));
  instr.makefile_name = string_copy(get_val_from_cfg(cfg, VAR_MAKE_NAME));

  instr.prog_files_list_loc = string_copy(get_val_from_cfg(&cfg, VAR_PROG_FILES));
  instr.obj_files_list_loc = string_copy(get_val_from_cfg(&cfg, VAR_COMPONENT_FILES));
  
  instr.common_flags = load_flags_from_cfg(cfg, VAR_FLAGS);
  instr.debug_flags = load_flags_from_cfg(cfg, VAR_DEBUG);
  instr.obj_flags = load_flags_from_cfg(cfg, VAR_OBJ_FLAGS);
  instr.prog_flags = load_flags_from_cfg(cfg, VAR_PROG_FLAGS);

  instr.obj_out = string_copy(get_val_from_cfg(cfg, VAR_OBJ_OUT));
  instr.obj_ext = string_copy(get_val_from_cfg(cfg, VAR_OBJ_EXT));
  instr.prog_out = string_copy(get_val_from_cfg(cfg, VAR_PROG_OUT));
}