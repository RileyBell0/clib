/*
 * Gets all 'c' files that are to be compiled
*/

#include "std/list.h"
#include "std/string.h"
#include "std/directory.h"

#include <string.h>
#include <stdio.h>

#define FILE_NAME "getFileList.c"

#define REQUIRED_ARGS 2
#define ARG_EXTENSION 2
#define ARG_PATH 1
#define EXIT_ERROR 1
#define SPACE_FOR_PERIOD 1
#define MIN_LEN_FOR_EXTENSION 2

int main(int argc, char **argv)
{
    // Check Arguments
    if (argc != REQUIRED_ARGS + 1)
    {
        printf("USAGE: \t  ./%s   \"path\"   \"extension\"\n", FILE_NAME);
        exit(EXIT_ERROR);
    }

    DIR *main, *sourceDir;

    // Making input args more usable
    string fileName = string_from_cstring(FILE_NAME);
    string path = string_from_cstring(argv[ARG_PATH]);
    string extension = string_from_cstring(argv[ARG_EXTENSION]);
    string pathSeperator = string_from_cstring("/");

    dynamicArray files;

    main = opendir(path.str);
    if (main)
    {
        printf("Directory: \"%s\" was found\n",path.str);

        string source = getSubDirectory(path,pathSeperator,extension);
        sourceDir = opendir(source.str);

        if (sourceDir)
        {
            printf("Directory: \"%s\" was found\n",source.str);
            files = getFilesWithExtension(sourceDir, extension);
            closedir(sourceDir);
        }
        else
        {
            printf("Directory: \"%s\" could not be found\n",source.str);
        }

        closedir(main);
    }
    else
    {
        printf("Directory: \"%s\" could not be found\n",path.str);
    }
    
    // Outputting all files found
    printf("Files Found:\n");
    for (unsigned int i = 0; i < files.len; i++)
    {
        printf("\t%s\n", ((struct dirent*)files.dat)[i].d_name);
    }
    
    printf("\nFinished Printing\n");
    return (0);
}