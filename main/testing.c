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
    
    char* yes = " debug=\"really true \\\"george\\\" said\" #and #thats cool # yes [a bc]";
    unsigned int pos = 0;
    string_t base = string_from_cstring(yes);
    string_t dest = new_string(10);
    while (extract_field(base, &pos, &dest))
    {
        if (dest.len != 0)
        {
            printf("field: \"%s\"\n", dest.str);
        }
        else
        {
            printf("empty:\n");
        }
        
    }

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


