/*
 * There is no purpose of this file other than to test the functionality of
 * the clib files
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define END_SUCCESS 0
#define MULTIPLE 1

int code(int argc, char** argv);
void bar(char* toPrint);

// Write your code here
int code(int argc, char** argv){
    

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


