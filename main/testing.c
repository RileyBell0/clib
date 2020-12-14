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

void strinfo(string_t info)
{
    printf("len: %d, max: %d, str: %s\n", info.len, info.max_len, cstr(&info));
}

void list_test()
{
    alist_t list = new_alist(sizeof(string_t));
    string_t str = string_make("hellow");

    printf("Time to append\n");
    alist_append(&list, &str);
    str = string_make("elemtn 2");
    alist_append(&list, &str);
    str = string_make("elemtn 3");
    alist_append(&list, &str);
    str = string_make("elemtn 4");
    alist_append(&list, &str);
    str = string_make("elemtn 5");
    alist_append(&list, &str);
    str = string_make("elemtn 6");
    alist_append(&list, &str);

    printf("Time to retrieve some elements\n");
    string_t* retrieved = alist_get_element(&list, 1);
    printf("Second Element: %s\n", cstr(retrieved));
    retrieved = alist_get_element(&list, 0);
    printf("First Element: %s\n", cstr(retrieved));
    retrieved = alist_get_element(&list, 2);
    printf("third Element: %s\n", cstr(retrieved));
    retrieved = alist_get_element(&list, 3);
    printf("forth Element: %s\n", cstr(retrieved));
}

#define STR_TEST 48
// Write your code here
int code(int argc, char** argv){
    list_test();
    return 0;
    printf("Reading config file..\n");
    config_t cfg = read_config_file(".build/config/project.cfg");
    printf("Completed\n\n");

    printf("reading all entries in dir\n");
    ordered_dirent_t entries = dir_all_entries_categorised(string_make("std"));
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


    list_t result = dir_files_with_extension_recur(string_make("std"), string_make("c"));
    node = result.first_node;
    printf("going through results:\n");
    while (node)
    {
        printf("\t- %s\n", cstr(lstrnode(node)));
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


