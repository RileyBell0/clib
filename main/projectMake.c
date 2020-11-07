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
#define EXTRA_CHARS 9
#define DEFAULT_STR_LEN_PROJMAKE 16

#define OBJECT_OUT_DIR "objects/"
#define COMPILER "gcc"
#define OBJ_FLAGS "-Wall -c -o"
#define PROG_FLAGS "-Wall -o"
#define DEBUG_FLAG "-g"
#define OBJECT_EXTENSION ".o"

#define MAKEFILE_NAME "Makefile"

typedef struct make_instructions_t
{
    /*
     * The name of the compiler to use, Confirmed to work with "gcc"
    */
    string_t compiler;

    /*
     * Flags to compile programs intended to be objects
    */
    string_t flags;

    /*
     * Flags to compile programs with 'int main()'
    */
    string_t progFlags;

    /*
     * Flags required if debugging, reccomended "-g"
    */
    string_t debug_flags;

    /*
     * The extension to give the compiled objects. ".o" is
     * reccomended
    */
    string_t objectExtension;

    /*
     * Where to put the compiled objects
    */
    string_t outDir;
} make_instructions_t;

void write_string_list(FILE *outFile, list_t list);
string_t concat_string_list(list_t list);
void add_object_make_instructions(make_instructions_t *makeInstructions,
                                  list_t *files,
                                  list_t *objectPaths,
                                  list_t *objectMakeLines,
                                  list_t *objectNames);

void add_program_make_instructions(make_instructions_t *makeInstructions,
                                   list_t *mainFiles,
                                   list_t *execNames,
                                   list_t *execInstructions,
                                   string_t *allObjects,
                                   string_t *allObjectPaths);

// overcommented to heck and back
void add_object_make_instructions(make_instructions_t *makeInstructions,
                                  list_t *files,
                                  list_t *objectPaths,
                                  list_t *objectMakeLines,
                                  list_t *objectNames)
{
    /*
     * For all files in the list
    */
    list_node_t *node = files->first_node;
    while (node)
    {
        string_t *source = (string_t *)node->data;

        /*
         * Removing the extension and replacing it with the required object
         * extension. This will be the name of the compiled file
        */
        string_t objName = getFileName(*source);
        objName.str = removeFileExtension(objName.str);
        objName.len = strlen(objName.str);
        string_write(&objName, makeInstructions->objectExtension);

        /*
         * Creating a string to store the path to where to store the compiled file. eg "objects/my_file.o"
        */
        string_t fileOutPath = string_new_concat(makeInstructions->outDir, objName);

        /*
         * Concatenating all information into a makefile instruction for the given object
         * Allocates just enough space to avoid reallocing at any point
        */
        string_t finalised = new_string(objName.len +
                                        source->len +
                                        makeInstructions->compiler.len +
                                        makeInstructions->flags.len +
                                        fileOutPath.len +
                                        source->len +
                                        makeInstructions->debug_flags.len +
                                        EXTRA_CHARS);

        /*
         * Writing the name of the instruction
         * 
         * 1) myobject.o: 
        */
        string_write(&finalised, objName);
        string_write_c(&finalised, ": ");

        /*
         * Adding the path to the source file
         * 
         * 1) myobject.o: src/myobject.c 
         * 2)     
        */
        string_write(&finalised, *source);
        string_write_c(&finalised, "\n\t");

        /*
         * Adding the compiler and the flags
         * 
         * 1) myobject.o: src/myobject.c
         * 2)     gcc -Wall -c -o
        */
        string_write(&finalised, makeInstructions->compiler);
        string_write_c(&finalised, " ");
        string_write(&finalised, makeInstructions->flags);
        string_write_c(&finalised, " ");

        /*
         * Adding the name of and where to store the output file
         * 
         * 1) myobject.o: src/myobject.c
         * 2)     gcc -Wall -c -o objects/myobject.o
        */
        string_write(&finalised, fileOutPath);
        string_write_c(&finalised, " ");

        /*
         * Adding the source file's location
         * 
         * 1) myobject.o: src/myobject.c
         * 2)     gcc -Wall -c -o objects/myobject.o src/myobject.c
        */
        string_write(&finalised, *source);
        string_write_c(&finalised, " ");

        /*
         * Adds all debug flags (if hte debug flags string is empty, nothing will be appended)
         * 
         * 1) myobject.o: src/myobject.c
         * 2)     gcc -Wall -c -o objects/myobject.o src/myobject.c -g
         * 3) 
        */
        string_write(&finalised, makeInstructions->debug_flags);
        string_write_c(&finalised, "\n");

        /*
         * Adding the path to the object we've just created the instructions for. Required
         * in linking the file to overall programs
        */
        if (objectPaths)
        {
            list_append(objectPaths, &fileOutPath);
        }
        else
        {
            string_destroy(&fileOutPath);
        }

        /*
         * Saving the make instructions we just created through a bunch of concats
        */
        if (objectMakeLines)
        {
            list_append(objectMakeLines, &finalised);
        }
        else
        {
            string_destroy(&finalised);
        }

        /*
         * Saving the name of the object (the name of the instruction we just created)
        */
        if (objectNames)
        {
            list_append(objectNames, &objName);
        }
        else
        {
            string_destroy(&objName);
        }

        node = node->next;
    }
}

// overcommented to heck and back
void add_program_make_instructions(make_instructions_t *makeInstructions,
                                   list_t *mainFiles,
                                   list_t *execNames,
                                   list_t *execInstructions,
                                   string_t *allObjects,
                                   string_t *allObjectPaths)
{
    // Creating strings for the make instructions for each executable
    list_node_t *node = mainFiles->first_node;
    while (node)
    {
        string_t *source = (string_t *)node->data;

        /*
         * Removing the extension and replacing it with the required object
         * extension. This will be the name of the compiled file
        */
        string_t objName = getFileName(*source);
        objName.str = removeFileExtension(objName.str);
        objName.len = strlen(objName.str);

        /*
         * Saving the name of the program. This is what will be called when compiling the program
        */
        string_t progName = string_copy(objName);

        /*
         * Adding the .o, this is the name of the object file and the make instruction
        */
        string_write(&objName, makeInstructions->objectExtension);

        string_t progObjName = string_copy(objName);
        /*
         * Creating a string to store the path to where to store the compiled file. eg "objects/my_file.o"
        */
        string_t fileOutPath = string_new_concat(makeInstructions->outDir, objName);

        /*
         * Allocating just enough space to store the new string. This means no realloc
         * calls should be required
        */
        string_t finalised = new_string(progName.len +
                                        objName.len +
                                        allObjects->len +
                                        allObjectPaths->len +
                                        makeInstructions->compiler.len +
                                        makeInstructions->progFlags.len +
                                        progName.len +
                                        makeInstructions->debug_flags.len +
                                        EXTRA_CHARS +
                                        1 +
                                        1);

        /*
         * Writing the name of the instruction
         * 
         * 1) myprog:
        */
        string_write(&finalised, progName);
        string_write_c(&finalised, ": ");

        /*
         * Adding the call to make the object file for the current program
         * 
         * 1) myprog: myprog.o 
        */
        string_write(&finalised, progObjName);
        string_write_c(&finalised, " ");

        /*
         * Calling the creation of all other object files for the program
         * 
         * Note that the names of all programs are also in this string, just at the end of it
         * 1) myprog: myobj_1.o myobj_2.o ... myobj_n.o   myprog_1.o 
         * 2)     
        */
        string_write(&finalised, *allObjects);
        string_write_c(&finalised, "\n\t");

        /*
         * Adding the compiler
         * 
         * 1) myprog:myobj_1.o myobj_2.o ... myobj_n.o   myprog_1.o myprog_2.o ... myprog_n.o   
         * 2)     gcc 
        */
        string_write(&finalised, makeInstructions->compiler);
        string_write_c(&finalised, " ");

        /*
         * Adding the flags for making a program
         * 
         * 1) myprog: myobj_1.o myobj_2.o ... myobj_n.o   myprog_1.o myprog_2.o ... myprog_n.o   
         * 2)     gcc -Wall -o 
        */
        string_write(&finalised, makeInstructions->progFlags);
        string_write_c(&finalised, " ");

        /*
         * Adding the output name of the program
         * 
         * 1) myprog: myobj_1.o myobj_2.o ... myobj_n.o   myprog_1.o myprog_2.o ... myprog_n.o   
         * 2)     gcc -Wall -o build/myprog 
        */
        string_write(&finalised, progName);
        string_write_c(&finalised, " ");

        /*
         * Adding the path to the program main file
         * 
         * 1) myprog: myobj_1.o myobj_2.o ... myobj_n.o   myprog_1.o myprog_2.o ... myprog_n.o   
         * 2)     gcc -Wall -o build/myprog objects/myprog.o
        */
        string_write(&finalised, fileOutPath);
        string_write_c(&finalised, " ");

        /*
         * Adding the paths to all the objects being linked
         * 
         * 1) myprog: myobj_1.o myobj_2.o ... myobj_n.o   myprog_1.o myprog_2.o ... myprog_n.o   
         * 2)     gcc -Wall -o build/myprog objects/myprog.o objects/myobj_1.o objects/myobj_2.o ... objects/myobj_n.o   objects/myprog_1.o
        */
        string_write(&finalised, *allObjectPaths);
        string_write_c(&finalised, " ");

        /*
         * Adding the paths to all the objects being linked
         * 
         * 1) myprog: myobj_1.o myobj_2.o ... myobj_n.o   myprog_1.o myprog_2.o ... myprog_n.o   
         * 2)     gcc -Wall -o build/myprog objects/myobj_1.o objects/myobj_2.o ... objects/myobj_n.o   objects/myprog_1.o objects/myprog_2.o ... objects/myprog_n.o   -g
        */
        string_write(&finalised, makeInstructions->debug_flags);
        string_write_c(&finalised, "\n");

        /*
         * Saving the name of the executable's make instruction to add to the 'all:' make instruction
         * eg.
         * all: myprog myprog1 myprog2 ... myprogn
        */
        if (execNames)
        {
            list_append(execNames, &progName);
        }
        else
        {
            string_destroy(&progName);
        }

        if (execInstructions)
        {
            list_append(execInstructions, &finalised);
        }
        else
        {
            string_destroy(&finalised);
        }

        string_destroy(&objName);
        string_destroy(&fileOutPath);
        string_destroy(&progObjName);

        node = node->next;
    }
}

int main(int argc, char **argv)
{
    // Check Arguments
    if (argc < REQUIRED_ARGS + 1)
    {
        printf("USAGE: \t  ./thisProgram   \"source Files\"  \"program Files\"\n");
        exit(EXIT_ERROR);
    }

    /*
     * Converting input args to string_t's
    */
    string_t outFilePath = string_from_cstring(MAKEFILE_NAME);
    string_t requiredFilesPath = string_from_cstring(argv[ARG_REQ_FILES]);
    string_t programFilesPath = string_from_cstring(argv[ARG_PROG_FILES]);

    /*
     * Setting up the make instructions for the makefile
    */
    make_instructions_t instructions = {0};
    instructions.compiler = string_from_cstring(COMPILER);
    instructions.flags = string_from_cstring(OBJ_FLAGS);
    instructions.progFlags = string_from_cstring(PROG_FLAGS);
    instructions.debug_flags = string_from_cstring(DEBUG_FLAG);
    instructions.objectExtension = string_from_cstring(OBJECT_EXTENSION);
    instructions.outDir = string_from_cstring(OBJECT_OUT_DIR);

    /*
     * Lists responsible for storing object information
     * Object Names
     *      myobj.o
     * 
     * Object Paths
     *      objects/myobj.o
     * 
     * Object Make Lines:
     *      myobj.o: src/myobj.c
     *          gcc -c -o objects/myobj.o src/myobj.c -g
    */
    list_t objectNames = new_list(sizeof(string_t));
    list_t objectPaths = new_list(sizeof(string_t));
    list_t objMakeLines = new_list(sizeof(string_t));

    /*
     * Lists responsible for storing object information
     * Exec Names
     *     [myprog1, myprog2, myprog3, ...]
     * 
     * exec Paths
     *      [objects/myprog1.o, objects/myprog2.o, objects/myprog3.o, ...]
     * 
     * prog Make Lines:
     *      myprog.o: src/myprog.c
     *          gcc -c -o objects/myobj.o src/myobj.c -g
    */
    list_t execNames = new_list(sizeof(string_t));
    list_t execInstructions = new_list(sizeof(string_t));
    list_t progMakeLines = new_list(sizeof(string_t));
    
    /*
     * Constructing make file instructions for the non-main files
    */
    list_t objectFiles = fileio_read_all_lines_list(requiredFilesPath.str);
    add_object_make_instructions(&instructions, &objectFiles, &objectPaths, &objMakeLines, &objectNames);
    list_destroy(&objectFiles, void_string_destroy);

    /*
     * Constructing make instructions for creating objects from the main files
    */
    list_t mainFiles = fileio_read_all_lines_list(programFilesPath.str);
    add_object_make_instructions(&instructions, &mainFiles, NULL, &progMakeLines, NULL);

    /*
     * Concating the object names and paths into singular strings with ' ' between each entry
    */
    string_t allObjectPaths = concat_string_list(objectPaths);
    list_destroy(&objectPaths, void_string_destroy);
    string_t allObjects = concat_string_list(objectNames);
    list_destroy(&objectNames, void_string_destroy);
    /*
     * Adding the make instructions for each program
    */
    add_program_make_instructions(&instructions, &mainFiles, &execNames, &execInstructions, &allObjects, &allObjectPaths);
    string_destroy(&allObjectPaths);
    string_destroy(&allObjects);
    /*
     * Done using the lines from the mainFiles file, destroying...
    */
    list_destroy(&mainFiles, void_string_destroy);

    /*
     * Writing the 'all' instruction for the makefile
    */
    string_t allCmd = new_string(DEFAULT_STR_LEN_PROJMAKE);
    string_t names = concat_string_list(execNames);
    list_destroy(&execNames, void_string_destroy);
    string_write(&allCmd, string_from_cstring("all: "));
    string_write(&allCmd, names);
    string_write(&allCmd, string_from_cstring("\n"));
    string_destroy(&names);

    //////////////////////////////////////////////////////////////
    /*
     * Writing the makefile
    */
    //////////////////////////////////////////////////////////////
    FILE *outFile = fileio_open_safe(outFilePath.str, FALSE);

    fprintf(outFile, "%s\n", allCmd.str);
    string_destroy(&allCmd);

    // Print the object creation code
    write_string_list(outFile, execInstructions);
    list_destroy(&execInstructions, void_string_destroy);

    write_string_list(outFile, progMakeLines);
    list_destroy(&progMakeLines, void_string_destroy);

    write_string_list(outFile, objMakeLines);
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
        totalLen += ((string_t *)node->data)->len;
        ++totalLen;
        node = node->next;
    }

    string_t finalised = new_string(totalLen);

    node = list.first_node;
    while (node)
    {
        string_write(&finalised, *((string_t *)node->data));
        string_write(&finalised, string_from_cstring(" "));
        node = node->next;
    }

    return finalised;
}

void write_string_list(FILE *outFile, list_t list)
{
    // Print the object creation code
    list_node_t *node = list.first_node;
    while (node)
    {
        fprintf(outFile, "%s\n", ((string_t *)node->data)->str);
        node = node->next;
    }
}