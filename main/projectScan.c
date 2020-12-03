/*
 * Gets all 'c' files that are to be compiled
*/

#include <stdio.h>
#include "../std/path.h"
#include "../std/list.h"
#include "../std/string.h"
#include "../std/directory.h"
#include "../std/fileIO.h"
#include "../std/configLoader.h"

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

config_var_t* safe_cfg_get_var(config_t* config, char* name);
void add_src_files_from_dir(FILE* outFile, string_t basePath, string_t pathSeperator, string_t extension);

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

void add_src_files_from_dir(FILE* outFile, string_t basePath, string_t pathSeperator, string_t extension)
{
    DIR* main = opendir(basePath.str);
    if (main == NULL)
    {
        printf("DIR could not be opened\n");
        return;
    }

    printf("Reached the first section\n");
    list_t files = getFilesWithExtensionRecursive(main, basePath, pathSeperator, extension);
    printf("Got all the files\n");

    char* yes = malloc(sizeof(char)*52);

    // Print all files in the subdirectory
    list_node_t *node = files.first_node;
    while (node)
    {
        string_t* current = ((string_t*)node->data);
        printf("\tCurrent String: %s\n", current->str);
        // fprintf(outFile, "%s\n", current->str);

        node = node->next;
    }

    list_destroy(&files, void_string_destroy);

    closedir(main);
}

int main(int argc, char **argv)
{
    printf("Size: %d\n", sizeof(struct dirent));
    printf("Size: %d\n", sizeof(struct dirent));
    printf("Size: %d\n", sizeof(struct dirent));
    printf("Size: %d\n", sizeof(struct dirent));
    printf("Size: %d\n", sizeof(struct dirent));
    // Check Arguments
    if (argc < REQUIRED_ARGS + 1)
    {
        printf("USAGE: %s  Path_To_Config\n", argv[0]);
        exit(EXIT_ERROR);
    }

    config_t cfg = read_config_file(argv[ARG_CONFIG]);

    // ------------ SETTING UP VARAIBLES
    config_var_t* var_src_dirs = safe_cfg_get_var(&cfg, VAR_SRC_DIRS);
    config_var_t* var_main_dirs = safe_cfg_get_var(&cfg, VAR_MAIN_DIRS);
    config_var_t* var_extension = safe_cfg_get_var(&cfg, VAR_EXTENSION);
    config_var_t* var_component_out = safe_cfg_get_var(&cfg, VAR_COMPONENT_OUT);
    config_var_t* var_main_out = safe_cfg_get_var(&cfg, VAR_MAIN_OUT);
    config_var_t* var_config_dir = safe_cfg_get_var(&cfg, VAR_CONFIG_DIR);


    // Making input args more usable
    string_t extension = *var_extension->data;
    string_t pathSeperator = string_from_cstring(PATH_SEPERATOR);
    string_t component_out = string_new_concat_multi(*var_config_dir->data, &pathSeperator, var_component_out->data, NULL);
    string_t main_out = string_new_concat_multi(*var_config_dir->data, &pathSeperator, var_main_out->data, NULL);
    
    // Finding and recording all program files
    // FILE *mainOut = fileio_open_safe(main_out.str, FALSE);
    for (unsigned int i = 0; i < var_main_dirs->len; i++)
    {
        printf("%d OI THE LOOP HAS BEEN ENTERED\n",i);
        printf("dir = %s\n\n", var_main_dirs->data[i]);
        string_t mainDirPath = var_main_dirs->data[i];
        add_src_files_from_dir(NULL, mainDirPath, pathSeperator, extension);
    }
    // fclose(mainOut);

    // // Finding and recording all component files
    // printf("COMPONENT START\n");
    // FILE *componentOut = fileio_open_safe(component_out.str, FALSE);
    // for (unsigned int i = 0; i < var_src_dirs->len; i++)
    // {
    //     string_t componentDirPath = var_src_dirs->data[i];
    //     add_src_files_from_dir(mainOut, componentDirPath, pathSeperator, extension);
    // }
    // fclose(componentOut);
    // printf("Finished with the Component files\n\n");

    // string_destroy(&component_out);
    // string_destroy(&main_out);

    // config_destroy(cfg);

    return (0);
}