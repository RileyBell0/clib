// /*
//  * Gets all 'c' files that are to be compiled
// */

// // #include "../std/directory.h"
// // #include "../std/configLoader.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include "../std/configLoader.h"
// #include "../std/directory.h"

// #define REQUIRED_ARGS 1
// #define ARG_CONFIG 1

// #define VAR_SRC_DIRS "SRC_DIRS"
// #define VAR_MAIN_DIRS "MAIN_DIRS"
// #define VAR_EXTENSION "PROGRAM_TYPE"
// #define VAR_COMPONENT_OUT "COMPONENT_OUT"
// #define VAR_MAIN_OUT "MAIN_OUT"
// #define VAR_CONFIG_DIR "CONFIG_DIR"

// #define EXIT_ERROR 1
// #define SPACE_FOR_PERIOD 1
// #define MIN_LEN_FOR_EXTENSION 2

// config_var_t* safe_cfg_get_var(config_t* config, char* name);
// void add_src_files_from_dir(FILE* outFile, string_t basePath, string_t pathSeperator, string_t extension);

// config_var_t* safe_cfg_get_var(config_t* config, char* name)
// {
//     config_var_t* returnedVar = config_get_var(config, name);
//     if (!returnedVar)
//     {
//         printf("ERROR - %s could not be found in config file\n", name);
//         exit(EXIT_ERROR);
//     }
//     return returnedVar;
// }

// void add_src_files_from_dir(FILE* outFile, string_t *basePath, string_t *pathSeperator, string_t *extension)
// {
//     printf("Reached the first section\n");
//     list_t files = dir_files_with_extension_recur(basePath, extension);
//     printf("Got all the files\n");


//     // Print all files in the subdirectory
//     list_node_t *node = files.first_node;
//     while (node)
//     {
//         string_t* current = ((string_t*)node->data);
//         printf("\tCurrent String: %s\n", cstr(current));
//         // fprintf(outFile, "%s\n", current->str);

//         node = node->next;
//     }

//     list_destroy(&files, void_string_destroy);
// }

// int main(int argc, char **argv)
// {
//     // Check Arguments
//     if (argc < REQUIRED_ARGS + 1)
//     {
//         printf("USAGE: %s  Path_To_Config\n", argv[0]);
//         exit(EXIT_ERROR);
//     }

//     config_t cfg = read_config_file(argv[ARG_CONFIG]);

//     // ------------ SETTING UP VARAIBLES
//     config_var_t* var_src_dirs = safe_cfg_get_var(&cfg, VAR_SRC_DIRS);
//     config_var_t* var_main_dirs = safe_cfg_get_var(&cfg, VAR_MAIN_DIRS);
//     config_var_t* var_extension = safe_cfg_get_var(&cfg, VAR_EXTENSION);
//     config_var_t* var_component_out = safe_cfg_get_var(&cfg, VAR_COMPONENT_OUT);
//     config_var_t* var_main_out = safe_cfg_get_var(&cfg, VAR_MAIN_OUT);
//     config_var_t* var_config_dir = safe_cfg_get_var(&cfg, VAR_CONFIG_DIR);


//     // Making input args more usable
//     string_t extension = *var_extension->data;
//     string_t pathSeperator = string_make(PATH_SEPERATOR);
//     string_t component_out = string_new_concat_multi(var_config_dir->data, &pathSeperator, var_component_out->data, NULL);
//     string_t main_out = string_new_concat_multi(var_config_dir->data, &pathSeperator, var_main_out->data, NULL);
    
//     printf("\t- Trying first directory.h function, dir_all_entries_list\n");
//     list_t entries = dir_all_entries_list( string_make("std") );
//     printf("\t- Completed function\n\t-\n");

//     list_node_t* node = entries.first_node;
//     printf("entries: %d\n",entries.size);
//     while (node)
//     {
//         printf("\t%s\n",((struct dirent*)node->data)->d_name);
//         node = node->next;
//     }

//     // Finding and recording all program files
//     FILE *mainOut = fileio_open_safe(cstr(&main_out), FALSE);
//     for (unsigned int i = 0; i < var_main_dirs->len; i++)
//     {
//         printf("%d OI THE LOOP HAS BEEN ENTERED\n",i);
//         printf("dir = %s\n\n", cstr(&var_main_dirs->data[i]));
//         string_t mainDirPath = var_main_dirs->data[i];
//         add_src_files_from_dir(NULL, mainDirPath, pathSeperator, extension);
//     }
//     fclose(mainOut);

//     // Finding and recording all component files
//     printf("COMPONENT START\n");
//     FILE *componentOut = fileio_open_safe(cstr(&component_out), FALSE);
//     for (unsigned int i = 0; i < var_src_dirs->len; i++)
//     {
//         string_t componentDirPath = var_src_dirs->data[i];
//         add_src_files_from_dir(mainOut, componentDirPath, pathSeperator, extension);
//     }
//     fclose(componentOut);
//     printf("Finished with the Component files\n\n");

//     string_destroy(&component_out);
//     string_destroy(&main_out);

//     config_destroy(cfg);

//     return (0);
// }

int main()
{
  return 0;
}