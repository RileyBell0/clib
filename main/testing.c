/*
 * There is no purpose of this file other than to test the functionality of
 * the clib files
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../std/configLoader.h"

#define END_SUCCESS 0
#define MULTIPLE 1

int code(int argc, char** argv);
void bar(char* toPrint);

// Write your code here
int code(int argc, char** argv){
    config_t config = read_config_file("/usr/include/clib/configTest.txt");


    string_t buffer = new_string(DEFAULT_BUFFER_LEN);

    while (fileio_next_line(stdin, &buffer))
    {
        
        config_var_t* vary = config_get_var(&config, buffer.str);

        if (vary)
        {
            printf("Found the requested var\n");
            printf("\nname: %s\n",vary->varName.str);

            for (int i = 0; i < vary->len; i++)
            {
                string_t element = vary->data[i];
                printf("\t- %s\n",element.str);
            }
        }
        else
        {
            printf("Couldn't find the requested var\n");
        }
    }

    config_destroy(config);
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


