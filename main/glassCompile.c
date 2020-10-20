/*
 * Gets all 'c' files that are to be compiled
*/

#include "../std/list.h"
#include "../std/string.h"
#include "../std/directory.h"
#include "../std/fileIO.h"

#include <stdio.h>

#define FILE_NAME "getFileList.c"

#define REQUIRED_ARGS 3
#define ARG_OUTFILE 3
#define ARG_EXTENSION 2
#define ARG_PATH 1

#define EXIT_ERROR 1
#define SPACE_FOR_PERIOD 1
#define MIN_LEN_FOR_EXTENSION 2


int main(int argc, char **argv);
int main(int argc, char **argv)
{
// Check Arguments
    if (argc < REQUIRED_ARGS + 1)
    {
        printf("USAGE %d: \t  ./%s   \"path\"   \"fileExtension\"   \"outFile name\"\n", argc, FILE_NAME);
        exit(EXIT_ERROR);
    }
    // Making input args more usable
    string fileName = string_from_cstring(FILE_NAME);
    string path = string_from_cstring(argv[ARG_PATH]);
    string extension = string_from_cstring(argv[ARG_EXTENSION]);
    string outFilePath = string_from_cstring(argv[ARG_OUTFILE]);
    
    int nameOnly = FALSE;
    if (argc > REQUIRED_ARGS + 1)
    {
        if (*argv[REQUIRED_ARGS + 1] == '1')
        {
            nameOnly = TRUE;
        }
    }

    // TODO this should be platform inspecific
    string pathSeperator = string_from_cstring("/");
    string source;
    DIR *main, *sourceDir;
    list files = {0};
    list filesInMain = {0};
    
    main = opendir(path.str);
    if (main)
    {
        filesInMain = list_GetFilesWithExtension(main,extension);
        list_node* node = filesInMain.first_node;
        while(node)
        {
            node = node->next;
        }
        // Take preference for searching a sub-directory over the main directory
        source = getSubDirectory(path,pathSeperator,extension);
        sourceDir = opendir(source.str);
        // if the subdirectory exists
        if (sourceDir)
        {
            files = list_GetFilesWithExtension(sourceDir, extension);
            closedir(sourceDir);
        }

        closedir(main);
    }
    else
    {
        printf("Directory: \"%s\" could not be found\n",path.str);
    }


    // Outputting all files found
    FILE* outFile = fopen(outFilePath.str, "w");
    assert(outFile);

    // Print all files in the subdirectory
    list_node *node = files.first_node;
    while (node)
    {
        string fileName = string_from_cstring(((struct dirent*)node->data)->d_name);
        if (!nameOnly)
        {
            string firstout = string_new_concat(source, pathSeperator);
            string outStr = string_new_concat(firstout, fileName);
            string_destroy(firstout);

            // Writing all required files to the output file
            fprintf(outFile, "%s\n",outStr.str);

            string_destroy(outStr);
        }
        else
        {
            fprintf(outFile, "%s\n", removeFileExtension(fileName.str));
        }
        node = node->next;
    }
    
    node = filesInMain.first_node;
    while (node)
    {
        string fileName = string_from_cstring(((struct dirent*)node->data)->d_name);
        if (!nameOnly)
        {
            string firstout = string_new_concat(path, pathSeperator);
            string outStr = string_new_concat(firstout, fileName);
            string_destroy(firstout);

            // Writing all required files to the output file
            fprintf(outFile, "%s\n",outStr.str);

            string_destroy(outStr);
        }
        else
        {
            fprintf(outFile, "%s\n", removeFileExtension(fileName.str));
        }
        node = node->next;
    }

    fclose(outFile);
    
    return (0);
}



