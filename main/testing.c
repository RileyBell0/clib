/*
 * There is no purpose of this file other than to test the functionality of
 * the clib files
*/

#include <stdlib.h>
#include <stdio.h>
#include "../std/directory.h"
#include "../std/configLoader.h"

#define END_SUCCESS 0
#define MULTIPLE 1

int code(int argc, char** argv);
void bar(char* toPrint);

// Write your code here
int code(int argc, char** argv){
    printf("making string 1\n");
    string_t small = new_string(47);
    printf("writing 1 char\n");
    string_t ext = string_from_cstring("s");
    string_write(&small, &ext);

    printf("%lu\n", SHORT_STR_LEN);
    printf("%lu\n", SHORT_STR_LEN);
    printf("%lu\n", SHORT_STR_LEN);


    return 0;


    printf("Reading config file..\n");
    config_t cfg = read_config_file(".build/config/project.cfg");
    printf("Completed\n\n");

    printf("reading all entries in dir\n");
    ordered_dirent_t entries = dir_all_entries_categorised(string_from_cstring("std"));
    printf("Tried to get the entries...\n");
    list_node_t *node;

    node = entries.regular.first_node;
    int nums = 0;
    printf("\nFiles: %d\n", entries.regular.size);
    while (node)
    {
        nums++;
        printf("\t-%d %s\n", nums, ldirentnode(node)->d_name);
        node = node->next;
    }
    printf("Directories:\n");
    node=entries.directory.first_node;
    while (node)
    {
        printf("\t- %s\n", ldirentnode(node)->d_name);
        node = node->next;
    }


    list_t result = dir_files_with_extension_recur(string_from_cstring("std"), string_from_cstring("c"));
    node = result.first_node;
    printf("going through results:\n");
    while (node)
    {
        printf("\t- %s\n", lstrnode(node)->str);
        node = node->next;
    }
    printf("Cleaning Up...\n");
    ordered_dirent_destroy(&entries);
    list_destroy(&result, void_string_destroy);
    config_destroy(cfg);
    return END_SUCCESS;
}























// For formatting program output
void bar(char* toPrint){
    int len = strlen(toPrint);
    for (int i = 0; i < len + 1; i++){
        printf("-");
    }
    printf("\n");
    printf("%s\n",toPrint);
    for (int i = 0; i < len + 1; i++){
        printf("-");
    }
    printf("\n");
}

// Entrypoint of the program. Prints headers before and after entering the user's code
int main(int argc, char** argv){
    bar("Program Start");
    
    int errorCode = code(argc, argv);
    
    bar("Program End");
    return errorCode;
}


