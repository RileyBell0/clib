/*
 * Gets all 'c' files that are to be compiled
*/

#include "../std/list.h"
#include "../std/string.h"
#include "../std/directory.h"
#include "../std/fileIO.h"

#include <stdio.h>
#include "../std/path.h"

#define REQUIRED_ARGS 3
#define ARG_OUTFILE 3
#define ARG_EXTENSION 2
#define ARG_PATH 1

#define EXIT_ERROR 1
#define SPACE_FOR_PERIOD 1
#define MIN_LEN_FOR_EXTENSION 2


int main(int argc, char **argv)
{
    // Check Arguments
    if (argc < REQUIRED_ARGS + 1)
    {
        printf("USAGE: \t  ./thisProgram   \"path\"   \"fileExtension\"   \"outFile name\"\n");
        exit(EXIT_ERROR);
    }

    // ------------ SETTING UP VARAIBLES
    int nameOnly = FALSE;
    if (argc > REQUIRED_ARGS + 1 && *argv[REQUIRED_ARGS + 1] == '1')
    {
        nameOnly = TRUE;
    }
    // Making input args more usable
    string_t path = string_from_cstring(argv[ARG_PATH]);
    string_t extension = string_from_cstring(argv[ARG_EXTENSION]);
    string_t outFilePath = string_from_cstring(argv[ARG_OUTFILE]);
    string_t pathSeperator = string_from_cstring(PATH_SEPERATOR);

    // Outputting all files found
    FILE *outFile = fopen(outFilePath.str, "w");
    assert(outFile);

    DIR *main = opendir(path.str);

    if (main) 
    {
        list_t files = getFilesWithExtensionRecursive(main, path, pathSeperator, extension);

        // Print all files in the subdirectory
        list_node_t *node = files.first_node;
        while (node)
        {
            string_t* current = ((string_t*)node->data);
            if (nameOnly)
            {
                string_t toPrint = *current;
                if (toPrint.str[toPrint.len-1] != PATH_SEPERATOR_CHAR)
                {
                    for (unsigned int i = current->len-1; i >= 0; i--)
                    {
                        if (current->str[i] == PATH_SEPERATOR_CHAR)
                        {
                            toPrint.str = cstring_copy(&current->str[i + 1]);
                            break;
                        }
                    }
                }
                toPrint.str = removeFileExtension(toPrint.str);
                fprintf(outFile, "%s\n", toPrint.str);
            }
            else
            {
                fprintf(outFile, "%s\n", current->str);
            }

            node = node->next;
        }

        list_destroy(&files, void_string_destroy);
    }

    closedir(main);

    fclose(outFile);

    return (0);
}