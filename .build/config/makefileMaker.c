/*
 * Converts the generated files from 'glassCompile' and turns them into a makefile
*/

#include <stdio.h>
#include "../std/path.h"
#include "../std/bool.h"
#include "../std/string.h"
#include "../std/fileIO.h"
#include "../std/list.h"

#define REQUIRED_ARGS 2
#define ARG_PROG_FILES 2
#define ARG_REQ_FILES 1

#define EXIT_ERROR 1
#define SPACE_FOR_PERIOD 1
#define MIN_LEN_FOR_EXTENSION 2
#define DEFAULT_BUFFER_LEN 128
#define EXTRA_CHARS 9

void write_string_list(FILE* outFile, list_t list);
string_t concat_string_list(list_t list);

int main(int argc, char **argv)
{
    // Check Arguments
    if (argc < REQUIRED_ARGS + 1)
    {
        printf("USAGE: \t  ./thisProgram   \"source Files\"  \"program Files\"\n");
        exit(EXIT_ERROR);
    }

    // Making input args more usable
    string_t outFilePath = string_from_cstring("makefile");
    string_t requiredFilesPath = string_from_cstring(argv[ARG_REQ_FILES]);
    string_t programFilesPath = string_from_cstring(argv[ARG_PROG_FILES]);

    FILE *requiredFiles = fileio_open_safe(requiredFilesPath.str, TRUE);
    string_t buffer = new_string(DEFAULT_BUFFER_LEN);

    list_t valid_files = new_list(sizeof(string_t));

    while (fileio_next_line(requiredFiles, &buffer))
    {
        if (buffer.len > 0)
        {
            string_t valid_string = string_copy(buffer);
            list_append(&valid_files, &valid_string);
        }
    }

    fileio_close(requiredFiles);

    string_t compiler = string_from_cstring("gcc");
    string_t flags = string_from_cstring("-Wall -c -o");
    string_t progFlags = string_from_cstring("-Wall -o");
    string_t linker_flags = string_from_cstring("-g -lm");
    string_t objectExtension = string_from_cstring(".o");
    string_t outDir = string_from_cstring("objects/");

    list_t objectNames = new_list(sizeof(string_t));
    list_t objMakeLines = new_list(sizeof(string_t));
    list_t objectPaths = new_list(sizeof(string_t));
    list_t execInstructions = new_list(sizeof(string_t));
    list_t execNames = new_list(sizeof(string_t));
    list_t execBuildInstructions = new_list(sizeof(string_t));

    // Outputting all files found
    list_node_t *node = valid_files.first_node;
    while (node)
    {
        string_t *source = (string_t*)node->data;

        // Removing the .c and replacing it with .o
        string_t objName = getFileName(*source);
        objName.str = removeFileExtension(objName.str);
        objName.len = strlen(objName.str);
        string_write(&objName, objectExtension);
        
        // Getting the object.o name
        string_t fileOutPath = string_new_concat(outDir, objName);

        string_t finalised = new_string(objName.len + source->len + compiler.len + flags.len + fileOutPath.len + source->len + linker_flags.len + EXTRA_CHARS);
        string_write(&finalised, objName);
        string_write(&finalised, string_from_cstring(": "));
        string_write(&finalised, *source);
        string_write(&finalised, string_from_cstring("\n\t"));
        string_write(&finalised, compiler);
        string_write(&finalised, string_from_cstring(" "));
        string_write(&finalised, flags);
        string_write(&finalised, string_from_cstring(" "));
        string_write(&finalised, fileOutPath);
        string_write(&finalised, string_from_cstring(" "));
        string_write(&finalised, *source);
        string_write(&finalised, string_from_cstring(" "));
        string_write(&finalised, linker_flags);
        string_write(&finalised, string_from_cstring("\n"));

        list_append(&objectPaths, &fileOutPath);
        list_append(&objMakeLines, &finalised);
        list_append(&objectNames, &objName);

        node = node->next;
    }

    string_t allObjectPaths = concat_string_list(objectPaths);    
    string_t allObjects = concat_string_list(objectNames);

    // Creating strings for the make instructions for each executable
    FILE *programFiles = fileio_open_safe(programFilesPath.str, TRUE);
    while (fileio_next_line(programFiles, &buffer))
    {
        string_t *source = &buffer;

        // Removing the .c and replacing it with .o
        string_t objName = getFileName(*source);
        objName.str = removeFileExtension(objName.str);
        objName.len = strlen(objName.str);

        string_t progName = string_copy(objName);

        string_write(&objName, objectExtension);

        // Getting the object.o name
        string_t fileOutPath = string_new_concat(outDir, objName);

        string_t finalised = new_string(progName.len + objName.len + allObjects.len + allObjectPaths.len + compiler.len + progFlags.len + progName.len + linker_flags.len + EXTRA_CHARS + 3);
        string_write(&finalised, progName);
        string_write(&finalised, string_from_cstring(": "));
        string_write(&finalised, objName);
        string_write(&finalised, string_from_cstring(" "));
        string_write(&finalised, allObjects);
        string_write(&finalised, string_from_cstring("\n\t"));
        string_write(&finalised, compiler);
        string_write(&finalised, string_from_cstring(" "));
        string_write(&finalised, progFlags);
        string_write(&finalised, string_from_cstring(" "));
        string_write(&finalised, progName);
        string_write(&finalised, string_from_cstring(" "));
        string_write(&finalised, fileOutPath);
        string_write(&finalised, string_from_cstring(" "));
        string_write(&finalised, allObjectPaths);
        string_write(&finalised, string_from_cstring(" "));
        string_write(&finalised, linker_flags);
        string_write(&finalised, string_from_cstring("\n"));
        
        string_t buildInstructions = new_string(objName.len + source->len + compiler.len + flags.len + fileOutPath.len + source->len + linker_flags.len + EXTRA_CHARS);
        string_write(&buildInstructions, objName);
        string_write(&buildInstructions, string_from_cstring(": "));
        string_write(&buildInstructions, *source);
        string_write(&buildInstructions, string_from_cstring("\n\t"));
        string_write(&buildInstructions, compiler);
        string_write(&buildInstructions, string_from_cstring(" "));
        string_write(&buildInstructions, flags);
        string_write(&buildInstructions, string_from_cstring(" "));
        string_write(&buildInstructions, fileOutPath);
        string_write(&buildInstructions, string_from_cstring(" "));
        string_write(&buildInstructions, *source);
        string_write(&buildInstructions, string_from_cstring(" "));
        string_write(&buildInstructions, linker_flags);
        string_write(&buildInstructions, string_from_cstring("\n"));

        list_append(&execNames, &progName);
        list_append(&execBuildInstructions, &buildInstructions);
        list_append(&execInstructions, &finalised);

        string_destroy(&objName);
        string_destroy(&fileOutPath);
    }

    fileio_close(programFiles);

    string_t allCmd = new_string(strlen("all: "));
    string_t names = concat_string_list(execNames);
    string_write(&allCmd, string_from_cstring("all: "));
    string_write(&allCmd, names);
    string_write(&allCmd, string_from_cstring("\n"));
    string_destroy(&names);
    
    FILE *outFile = fileio_open_safe(outFilePath.str, FALSE);

    fprintf(outFile, "%s\n", allCmd.str);


    // Print the object creation code
    write_string_list(outFile, execInstructions);
    write_string_list(outFile, execBuildInstructions);
    write_string_list(outFile, objMakeLines);

    string_destroy(&allCmd);
    string_destroy(&allObjectPaths);
    string_destroy(&allObjects);
    string_destroy(&buffer);
    list_destroy(&execBuildInstructions, void_string_destroy);
    list_destroy(&execInstructions, void_string_destroy);
    list_destroy(&execNames, void_string_destroy);
    list_destroy(&valid_files, void_string_destroy);
    list_destroy(&objectPaths, void_string_destroy);
    list_destroy(&objectNames, void_string_destroy);
    list_destroy(&objMakeLines, void_string_destroy);
    fileio_close(outFile);

    return (0);
}

string_t concat_string_list(list_t list)
{
    list_node_t *node = list.first_node;
    unsigned int totalLen = 0;
    while (node)
    {
        totalLen += ((string_t*)node->data)->len;
        ++totalLen;
        node = node->next;
    }

    string_t finalised = new_string(totalLen);

    node = list.first_node;
    while (node)
    {
        string_write(&finalised, *((string_t*)node->data));
        string_write(&finalised, string_from_cstring(" "));
        node = node->next;
    }

    return finalised;
}

void write_string_list(FILE* outFile, list_t list)
{
    // Print the object creation code
    list_node_t *node = list.first_node;
    while (node)
    {
        fprintf(outFile, "%s\n",((string_t*)node->data)->str);
        node = node->next;
    } 
}