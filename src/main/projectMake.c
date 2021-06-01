/*
 * Converts the generated files from 'glassCompile' and turns them into a
 * makefile
 */

#include "../std/alist.h"
#include "../std/bool.h"
#include "../std/configLoader.h"
#include "../std/fileIO.h"
#include "../std/filePath.h"
#include "../std/string.h"
#include <stdio.h>

#define REQUIRED_ARGS 1
#define ARG_CONFIG_LOC 1

#define EXIT_ERROR 1
#define SPACE_FOR_PERIOD 1
#define MIN_LEN_FOR_EXTENSION 2
#define EXTRA_CHARS 9
#define DEFAULT_STR_LEN_PROJMAKE 16

// Variables expected in the config file
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
  // The name of the compiler to use, Confirmed to work with "gcc"
  config_var_t *compiler;

  // Flags to compile programs intended to be objects
  config_var_t *flags;

  // Flags to compile programs with 'int main()'
  config_var_t *prog_flags;

  // Flags required if debugging, reccomended "-g"
  config_var_t *debug_flags;

  /*
   * The extension to give the compiled objects. ".o" is
   * reccomended
   */
  config_var_t *obj_ext;

  // Where to put the compiled objects
  config_var_t *obj_out;

  // flags specific for making object files
  config_var_t *obj_flags;
} make_instructions_t;

void write_make_call(FILE *out_file, string_t *name);
void write_make_name_with_extension(FILE *out_file, string_t *name,
                                    string_t *extension);
void write_make_compiler(FILE *out_file, config_var_t *var_compiler);
void write_make_flags(FILE *out_file, config_var_t *var_flags);
void write_make_all_components(FILE *out_file, alist_t *component_names);
void write_make_all_call(FILE *out_file, alist_t *prog_names);
config_var_t *safe_cfg_get_var(config_t *config, char *name);

config_var_t *safe_cfg_get_var(config_t *config, char *name) {
  config_var_t *returnedVar = config_get_var(config, name);
  if (!returnedVar) {
    printf("ERROR - %s could not be found in config file\n", name);
    exit(EXIT_ERROR);
  }
  return returnedVar;
}

void write_make_all_call(FILE *out_file, alist_t *prog_names) {
  fprintf(out_file, "all:");

  alist_iterator_t it = new_alist_iterator(prog_names, TRUE);
  for (string_t *element = it.first(&it); !it.done(&it);
       element = it.next(&it)) {
    fprintf(out_file, " %s", cstr(element));
  }

  fprintf(out_file, "\n");
}

// Add the start of the call to make the current prog -> prog: prog.o
void write_make_call(FILE *out_file, string_t *name) {
  fprintf(out_file, "%s:", cstr(name));
}

void write_make_name_with_extension(FILE *out_file, string_t *name,
                                    string_t *extension) {
  fprintf(out_file, " %s%s", cstr(name), cstr(extension));
}

void write_make_compiler(FILE *out_file, config_var_t *var_compiler) {
  // Only care about the compiler in the first position in the config var
  fprintf(out_file, "\t%s", cstr(var_compiler->data));
}

void write_make_flags(FILE *out_file, config_var_t *var_flags) {
  for (unsigned int i = 0; i < var_flags->len; i++) {
    fprintf(out_file, " -%s", cstr(&var_flags->data[i]));
  }
}

void write_make_all_components(FILE *out_file, alist_t *component_names) {
  // Add calls for the compilation of all components
  // comp1.o comp2.o ... compn.o
  alist_iterator_t it = new_alist_iterator(component_names, TRUE);
  for (string_t *element = it.first(&it); !it.done(&it);
       element = it.next(&it)) {
    fprintf(out_file, " %s", cstr(element));
  }
}

/*
 * Program Entry Point
 */
int main(int argc, char **argv) {
  /*
   * Validate input arguments
   */
  if (argc < REQUIRED_ARGS + 1) {
    printf("USAGE:  %s  Path_To_Config  Is_Debugging\n", argv[0]);
    exit(EXIT_ERROR);
  }

  /*
   * Loading in the config file
   */
  config_t cfg = read_config_file(argv[ARG_CONFIG_LOC]);

  string_t flag_start = string_make("-");
  /*
   * Setting up varaibles
   */
  string_t path_sep = string_make(PATH_SEPERATOR);
  unsigned int path_sep_len = path_sep.len;
  config_var_t *var_makeName = safe_cfg_get_var(&cfg, VAR_MAKE_NAME);
  config_var_t *var_cfg_dir = safe_cfg_get_var(&cfg, VAR_CFG_DIR);
  config_var_t *var_prog_files = safe_cfg_get_var(&cfg, VAR_PROG_FILES);
  config_var_t *var_obj_files = safe_cfg_get_var(&cfg, VAR_COMPONENT_FILES);
  config_var_t *var_obj_flags = safe_cfg_get_var(&cfg, VAR_OBJ_FLAGS);

  config_var_t *var_obj_ext = safe_cfg_get_var(&cfg, VAR_OBJ_EXT);
  string_t *obj_ext = var_obj_ext->data;

  config_var_t *var_obj_out = safe_cfg_get_var(&cfg, VAR_OBJ_OUT);
  string_t *obj_out = var_obj_out->data;

  config_var_t *var_prog_out = safe_cfg_get_var(&cfg, VAR_PROG_OUT);
  config_var_t *var_compiler = safe_cfg_get_var(&cfg, VAR_COMPILER);
  config_var_t *var_flags = safe_cfg_get_var(&cfg, VAR_FLAGS);
  config_var_t *var_debug = safe_cfg_get_var(&cfg, VAR_DEBUG);
  string_t ext_seperator = string_make("."); // Extension seperator
  config_var_t *var_prog_flags = safe_cfg_get_var(&cfg, VAR_PROG_FLAGS);
  string_t extension = string_new_concat(&ext_seperator, var_obj_ext->data);
  alist_iterator_t it, it_2;
  unsigned int req_str_len;

  // config dir / component_files_name_storage_file
  req_str_len =
      var_cfg_dir->data->len + path_sep_len + var_obj_files->data->len;

  string_t component_files_path = new_string(req_str_len);
  string_write_multi(&component_files_path, var_cfg_dir->data, &path_sep,
                     var_obj_files->data, NULL);

  // config dir / program_files_name_storage_file
  req_str_len =
      var_cfg_dir->data->len + path_sep_len + var_prog_files->data->len;
  string_t prog_files_path = new_string(req_str_len);
  string_write_multi(&prog_files_path, var_cfg_dir->data, &path_sep,
                     var_prog_files->data, NULL);

  /*
   * Reading in from files
   */
  alist_t prog_file_paths = fileio_read_all_lines_alist(cstr(&prog_files_path));
  alist_t obj_file_paths =
      fileio_read_all_lines_alist(cstr(&component_files_path));

  /*
  // list of object src_files with path
  // list of program src files with path
  // list of program names
  // list of object names
  // object directory
  // flags n stuff

  // all: all_program_names

  // program_name: program_name all_object_names
  // compiler global_flags program_flags prog_out prog_obj_location
  all_object_locations

  // object_name.o: object_src_file
  // compiler global_flags object_flags dest src debugging
  */

  /*
   * Parsing read data
   */
  alist_t prog_names = get_file_names_from_paths(&prog_file_paths);
  alist_t obj_names = get_file_names_from_paths(&obj_file_paths);
  remove_file_extensions(&prog_names);
  remove_file_extensions(&obj_names);

  /*
  alist_t prog_out_names = new_alist(sizeof(string_t));
  it = new_alist_iterator(&prog_names, TRUE);
  for (string_t* node = it.first(&it); !it.done(&it); node = it.next(&it)) {
    // Make a string with enough space for the out name
    string_t out_name = new_string(var_prog_out->data->len +
                                   path_sep_len +
                                   node->len);

    // Make the out name
    string_write_multi(&out_name,
                       var_prog_out->data,
                       &path_seperator,
                       node,
                       NULL);

    // Save the out name
    alist_append(&prog_out_names, &out_name);
  }

  // This takes all program names, such as 'projectMake' and turns it into
  // objects/projectMake.o
  alist_t prog_obj_out = new_alist(sizeof(string_t));
  it = new_alist_iterator(&prog_names, TRUE);
  for (string_t* node = it.first(&it); !it.done(&it); node = it.next(&it)) {
    // Allocating enought space to store the new string
    string_t out_name = new_string(var_obj_out->data->len +
                                   path_sep_len +
                                   node->len +
                                   extension.len);

    // Combinging strings to make the new string
    string_write_multi(&out_name,
                       var_obj_out->data,
                       &path_seperator,
                       node,
                       &extension,
                       NULL);

    // Save the new name
    alist_append(&prog_obj_out, &out_name);
  }

  // Turns object names from stuff like 'alist' to
  // 'objects/alist
  alist_t obj_out_names = new_alist(sizeof(string_t));
  it = new_alist_iterator(&obj_names, TRUE);
  for (string_t* node = it.first(&it); !it.done(&it); node = it.next(&it)) {
    // Allocating enough space to store the string
    string_t out_name = new_string(var_obj_out->data->len +
                                   path_sep_len +
                                   node->len);

    // Writing the new string
    string_write_multi(&out_name,
                       var_obj_out->data,
                       &path_seperator,
                       node,
                       NULL);

    // Saving the new string
    alist_append(&obj_out_names, &out_name);
  } */

  // Write all obj names to a string, as need to use this multiple times
  string_t all_obj_names = new_string(DEFAULT_BUFFER_LEN);
  it = new_alist_iterator(&obj_names, TRUE);
  string_t whitespace = string_make(" ");
  for (string_t *element = it.first(&it); !it.done(&it);
       element = it.next(&it)) {
    string_write_multi(&all_obj_names, element, &ext_seperator, obj_ext,
                       &whitespace, NULL);
  }
  string_limit(&all_obj_names,
                all_obj_names.len - 1); // Remove the trailing whitespace

  // Write all object paths to a string, as need to use this multiple times
  string_t all_obj_paths = new_string(DEFAULT_BUFFER_LEN);
  it = new_alist_iterator(&obj_names, TRUE);
  for (string_t *element = it.first(&it); !it.done(&it);
       element = it.next(&it)) {
    string_write_multi(&all_obj_paths, obj_out, &path_sep, element,
                       &ext_seperator, obj_ext, &whitespace, NULL);
  }
  string_limit(&all_obj_paths,
                all_obj_paths.len - 1); // Remove the trailing whitespace

  string_t *compiler = var_compiler->data;

  string_t global_flags = new_string(DEFAULT_BUFFER_LEN);
  for (unsigned int i = 0; i < var_flags->len; i++) {
    string_write_multi(&global_flags, &flag_start, &var_flags->data[i],
                       &whitespace, NULL);
  }
  string_limit(&global_flags,
                global_flags.len - 1); // Remove the trailing whitespace

  string_t obj_flags = new_string(DEFAULT_BUFFER_LEN);
  for (unsigned int i = 0; i < var_obj_flags->len; i++) {
    string_write_multi(&obj_flags, &flag_start, &var_obj_flags->data[i],
                       &whitespace, NULL);
  }
  string_limit(&obj_flags,
                obj_flags.len - 1); // Remove the trailing whitespace
  
  string_t prog_flags = new_string(DEFAULT_BUFFER_LEN);
  for (unsigned int i = 0; i < var_prog_flags->len; i++) {
    string_write_multi(&prog_flags, &flag_start, &var_prog_flags->data[i],
                       &whitespace, NULL);
  }
  string_limit(&prog_flags,
                prog_flags.len - 1); // Remove the trailing whitespace

  string_t debug_flags = new_string(DEFAULT_BUFFER_LEN);
  for (unsigned int i = 0; i < var_debug->len; i++) {
    string_write_multi(&debug_flags, &flag_start, &var_debug->data[i],
                       &whitespace, NULL);
  }
  string_limit(&debug_flags,
                debug_flags.len - 1); // Remove the trailing whitespace

  // Create the makefile's file
  FILE *out_file = fileio_open_safe(cstr(var_makeName->data), FALSE);

  // Write the all call
  write_make_all_call(out_file, &prog_names); // all: prog_1 prog_2 ... prog_n
  fprintf(out_file, "\n");

  // Writing the make instructions for the programs
  it = new_alist_iterator(&prog_names, TRUE);
  for (string_t *prog_name = it.first(&it); !it.done(&it);
       prog_name = it.next(&it)) {
    fprintf(out_file, "%s: %s%s%s %s\n", cstr(prog_name), cstr(prog_name),
            cstr(&ext_seperator), cstr(obj_ext), cstr(&all_obj_names));
    fprintf(out_file, "\t%s %s %s %s%s%s %s%s%s%s%s %s %s\n\n", cstr(compiler),
            cstr(&global_flags), cstr(&prog_flags), cstr(var_prog_out->data),
            cstr(&path_sep), cstr(prog_name), cstr(obj_out), cstr(&path_sep),
            cstr(prog_name), cstr(&ext_seperator), cstr(obj_ext),
            cstr(&all_obj_paths), cstr(&debug_flags));
  }

  // Writing the make instructions for the program object files
  it = new_alist_iterator(&prog_names, TRUE);
  it_2 = new_alist_iterator(&prog_file_paths, TRUE);
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
  it = new_alist_iterator(&obj_names, TRUE);
  it_2 = new_alist_iterator(&obj_file_paths, TRUE);
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
  fprintf(out_file, "clean:\n\trm %s%s*%s", cstr(var_obj_out->data),
          PATH_SEPERATOR, cstr(&extension));
  fprintf(out_file, " %s%s*\n", cstr(var_prog_out->data), PATH_SEPERATOR);

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