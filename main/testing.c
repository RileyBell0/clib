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
    array_t result = read_config_file("/usr/include/clib/configTest.txt");

    for (int i = 0; i < result.len; i++)
    {
        config_var_t var = ((config_var_t*)result.dat)[i];
        printf("\nname: %s\n",var.varName.str);

        for (int i = 0; i < var.data.len; i++)
        {
            string_t element = ((string_t*)var.data.dat)[i];
            printf("\t- %s\n",element.str);
        }
    }


    config_destroy(result);
    

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


