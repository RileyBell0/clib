/*
 * Converts the generated files from 'glassCompile' and turns them into a makefile
*/

#include <stdio.h>
#include "../std/filePath.h"
#include "../std/bool.h"
#include "../std/string.h"
#include "../std/fileIO.h"
#include "../std/list.h"
#include "../std/configLoader.h"

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

typedef struct make_instructions_t
{
    // The name of the compiler to use, Confirmed to work with "gcc"
    config_var_t* compiler;

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
    config_var_t* obj_out;

    // flags specific for making object files
    config_var_t* obj_flags;
} make_instructions_t;

void write_make_call(FILE* outFile, string_t name);
void write_make_name_with_extension(FILE* outFile, string_t name, string_t extension);
void write_make_compiler(FILE* outFile, config_var_t* var_compiler);
void write_make_flags(FILE* outFile, config_var_t* var_flags);
void write_make_all_components(FILE* outFile, list_t component_names);
void write_make_all_call(FILE* outFile, list_t prog_names);
config_var_t* safe_cfg_get_var(config_t* config, char* name);
string_t* strnode(list_node_t* node);

config_var_t* safe_cfg_get_var(config_t* config, char* name)
{
    config_var_t* returnedVar = config_get_var(config, name);
    if (!returnedVar)
    {
        printf("ERROR - %s could not be found in config file\n", name);
        exit(EXIT_ERROR);
    }
    return returnedVar;
}

void write_make_all_call(FILE* outFile, list_t prog_names)
{
    fprintf(outFile, "all:");
    list_node_t* node = prog_names.first_node;
    while (node)
    {
        fprintf(outFile, " %s", strnode(node)->_str);
        node = node->next;
    }
    fprintf(outFile, "\n");
}

// Add the start of the call to make the current prog -> prog: prog.o 
void write_make_call(FILE* outFile, string_t name)
{
    fprintf(outFile, "%s:",name._str);
}

void write_make_name_with_extension(FILE* outFile, string_t name, string_t extension)
{
    fprintf(outFile, " %s%s", name._str, extension._str);
}

void write_make_compiler(FILE* outFile, config_var_t* var_compiler)
{
    // Compiler
    fprintf(outFile, "\t%s", var_compiler->data->_str);
}

void write_make_flags(FILE* outFile, config_var_t* var_flags)
{
    // Flags
    for (unsigned int i = 0; i < var_flags->len; i++)
    {
        fprintf(outFile, " -%s", var_flags->data[i]._str);
    }
}

void write_make_all_components(FILE* outFile, list_t component_names)
{
    // Add calls for the compilation of all components
    // comp1.o comp2.o ... compn.o
    list_node_t *comp_node = component_names.first_node;
    while (comp_node)
    {
        fprintf(outFile, " %s", strnode(comp_node)->_str);
        comp_node = comp_node->next;
    }
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
string_t* strnode(list_node_t* node)
{
    return (string_t*)node->data;
}

/*
 * Program Entry Point
*/
int main(int argc, char **argv)
{
    /*
     * Validate input arguments
    */
    if (argc < REQUIRED_ARGS + 1)
    {
        printf("USAGE:  %s  Path_To_Config  Is_Debugging\n", argv[0]);
        exit(EXIT_ERROR);
    }

    /*
     * Loading in the config file
    */
    config_t cfg = read_config_file(argv[ARG_CONFIG_LOC]);

    /*
     * Setting up varaibles
    */
    string_t path_seperator = string_make(PATH_SEPERATOR);
    config_var_t* var_makeName = safe_cfg_get_var(&cfg, VAR_MAKE_NAME);
    config_var_t* var_cfg_dir = safe_cfg_get_var(&cfg, VAR_CFG_DIR);
    config_var_t* var_prog_files = safe_cfg_get_var(&cfg, VAR_PROG_FILES);
    config_var_t* var_obj_files = safe_cfg_get_var(&cfg, VAR_COMPONENT_FILES);
    config_var_t* var_obj_flags = safe_cfg_get_var(&cfg, VAR_OBJ_FLAGS);
    config_var_t* var_obj_ext = safe_cfg_get_var(&cfg, VAR_OBJ_EXT);
    config_var_t* var_obj_out = safe_cfg_get_var(&cfg, VAR_OBJ_OUT);
    config_var_t* var_prog_out = safe_cfg_get_var(&cfg, VAR_PROG_OUT);
    config_var_t* var_compiler = safe_cfg_get_var(&cfg, VAR_COMPILER);
    config_var_t* var_flags = safe_cfg_get_var(&cfg, VAR_FLAGS);
    config_var_t* var_debug = safe_cfg_get_var(&cfg, VAR_DEBUG);
    string_t seperator = string_make(".");
    config_var_t* var_prog_flags = safe_cfg_get_var(&cfg, VAR_PROG_FLAGS);
    string_t extension = string_new_concat(&seperator, var_obj_ext->data);
    string_t component_files_loc = new_string(var_cfg_dir->data[0].len + strlen(PATH_SEPERATOR) + var_obj_files->data[0].len);
    string_write_multi(&component_files_loc, var_cfg_dir->data, &path_seperator, var_obj_files->data, NULL);
    string_t prog_files_loc = new_string(var_cfg_dir->data[0].len + strlen(PATH_SEPERATOR) + var_prog_files->data[0].len);
    string_write_multi(&prog_files_loc, var_cfg_dir->data, &path_seperator, var_prog_files->data, NULL);
    /*
     * Reading in from files
    */
    list_t prog_files = fileio_read_all_lines_list(prog_files_loc._str);
    list_t obj_files = fileio_read_all_lines_list(component_files_loc._str);

    /*
     * Parsing read data
    */
    list_t prog_names = get_file_names(prog_files);
    list_t obj_names = get_file_names(obj_files);
    remove_file_extensions(prog_names);
    remove_file_extensions(obj_names);

    list_t prog_out_names = new_list(sizeof(string_t));
    list_node_t* node = prog_names.first_node;
    while (node)
    {
        string_t out_name = new_string(var_prog_out->data[0].len + strlen(PATH_SEPERATOR) + strnode(node)->len);
        string_write_multi(&out_name, var_prog_out->data, &path_seperator, strnode(node), NULL);
        list_append(&prog_out_names, &out_name);
        node = node->next;
    }
    list_t prog_obj_out = new_list(sizeof(string_t));
    node = prog_names.first_node;
    while (node)
    {
        string_t out_name = new_string(var_obj_out->data[0].len + strlen(PATH_SEPERATOR) + strnode(node)->len + extension.len);
        string_write_multi(&out_name, var_obj_out->data, &path_seperator, strnode(node), &extension, NULL);
        list_append(&prog_obj_out, &out_name);
        node = node->next;
    }
    list_t obj_out_names = new_list(sizeof(string_t));
    node = obj_names.first_node;
    while (node)
    {
        string_t out_name = new_string(var_obj_out->data[0].len + strlen(PATH_SEPERATOR) + strnode(node)->len);
        string_write_multi(&out_name, var_obj_out->data, &path_seperator, strnode(node), NULL);
        list_append(&obj_out_names, &out_name);
        node = node->next;
    }


    // Generating component information
    // Getting each file name, reoving the extension and replacing it with the required extension
    node = obj_names.first_node;
    while (node)
    {
        string_write(strnode(node), &extension);
        node = node->next;
    }

    FILE* outFile = fileio_open_safe(var_makeName->data[0]._str, FALSE);

    write_make_all_call(outFile, prog_names);
    fprintf(outFile, "\n");
    node = prog_names.first_node;
    list_node_t *node_file = prog_files.first_node;
    list_node_t* node_out = prog_out_names.first_node;
    list_node_t* node_obj_loc = prog_obj_out.first_node;
    while (node)
    {
        write_make_call(outFile, *strnode(node));
        write_make_name_with_extension(outFile, *strnode(node), extension);
        write_make_all_components(outFile, obj_names);
        fprintf(outFile, "\n");

        write_make_compiler(outFile, var_compiler);
        write_make_flags(outFile, var_flags);
        write_make_flags(outFile, var_prog_flags);
        fprintf(outFile, " -o");
        fprintf(outFile, " %s", strnode(node_out)->_str);
        fprintf(outFile, " %s", strnode(node_obj_loc)->_str);
        write_make_all_components(outFile, obj_out_names);
        write_make_flags(outFile, var_debug);
        fprintf(outFile, "\n\n");

        // Add the make instructions for the obj file of the program
        fprintf(outFile, "%s%s: %s\n", strnode(node)->_str, extension._str, strnode(node_file)->_str);
        write_make_compiler(outFile, var_compiler);
        write_make_flags(outFile, var_flags);
        write_make_flags(outFile, var_obj_flags);
        fprintf(outFile, " -o");
        fprintf(outFile, " %s", strnode(node_obj_loc)->_str);
        fprintf(outFile, " %s", strnode(node_file)->_str);
        write_make_flags(outFile, var_debug);
        fprintf(outFile, "\n\n");

        node = node->next;
        node_file = node_file->next;
        node_out = node_out->next;
        node_obj_loc = node_obj_loc->next;
    }

    // Writing calls to make objects
    node = obj_names.first_node;
    node_file = obj_files.first_node;
    node_out = obj_out_names.first_node;
    while (node)
    {
        write_make_call(outFile, *strnode(node));
        fprintf(outFile, " %s", strnode(node_file)->_str);
        fprintf(outFile, "\n");

        write_make_compiler(outFile, var_compiler);
        write_make_flags(outFile, var_flags);
        write_make_flags(outFile, var_obj_flags);
        fprintf(outFile, " -o");
        fprintf(outFile, " %s", strnode(node_out)->_str);
        fprintf(outFile, " %s", strnode(node_file)->_str);
        write_make_flags(outFile, var_debug);
        fprintf(outFile, "\n\n");
        node = node->next;
        node_file = node_file->next;
        node_out = node_out->next;
    }

    fprintf(outFile, "clean:\n\trm %s%s*%s", var_obj_out->data->_str, PATH_SEPERATOR, extension._str);
    fprintf(outFile, " %s%s*\n", var_prog_out->data->_str, PATH_SEPERATOR);

    fclose(outFile);

    config_destroy(cfg);
    
    return (0);
}