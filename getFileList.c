/*
 * Gets all 'c' files that are to be compiled
*/

#include "std/list.h"
#include "std/string.h"

#include <dirent.h>
#include <string.h>
#include <stdio.h>

#define REQUIRED_ARGS 2
#define FILE_NAME "getFileList.c"
#define ARG_EXTENSION 2
#define ARG_PATH 1
#define EXIT_ERROR 1

string getExtension(string file)
{
    string extension;
    extension.str = NULL;
    extension.len = 0;
    
    // if there is an extension
    if (file.len > 2 && file.str[file.len - 1] != '.')
    {
        for (int i = file.len - 1; i > 0; i--)
        {
            // Found the start of the extension
            if (file.str[i] == '.')
            {
                extension.len = file.len - i + 1;
                extension.str = &file.str[i + 1];
            }
        }
    }

    return extension;
}

int main(int argc, char **argv)
{
    if (argc != REQUIRED_ARGS + 1)
    {
        printf("ERROR in %s, Invalid number of arguments recieved.\n", FILE_NAME);
        printf("USAGE: ./%s \"path\" \"extension\"\nIf using the current directory use \".\"\n", FILE_NAME);
    }

    DIR *d;
    list files = new_list(sizeof(string));

    string fileName = string_from_cstring(FILE_NAME);
    string path = string_from_cstring(argv[ARG_PATH]);
    string extension = string_from_cstring(argv[ARG_EXTENSION]);

    if (fileName.len == 0)
    {
        printf("Error - Don't modify the file name\n");
        exit(EXIT_ERROR);
    }

    struct dirent *dir;

    d = opendir(path.str);

    printf("Directory: \"%s\"\n",path.str);

    if (d)
    {
        printf("FOUND DIRECTORY\n");
        while ((dir = readdir(d)) != NULL)
        {
            fileName = string_from_cstring(dir->d_name);

            string fileExtension = getExtension(fileName);
            
            // Extension matches the requested extension
            if (fileExtension.len != 0 && strcmp(fileExtension.str,extension.str) == 0)
            {
                printf("\tFound: \t%s\n",fileName.str);
                string validFile = string_copy(fileName);
                printf("\t\t%s\n",validFile.str);
                list_append(&files, &validFile);
            }
        }
        printf("END:\n");

        closedir(d);
    }

    list_node *node = files.first_node;

    printf("Files Found:\n");
    int i = 0;
    while (node)
    {
        i++;
        printf("%d\t%s\t\t\t%d\n",i,((string*)node->data)->str,((string*)node->data)->len );
        node = node->next;
    }
    printf("Finished Printing\n");
    return (0);
}