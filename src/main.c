#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define END_SUCCESS 0
#define MULTIPLE 1

int code(int argc, char** argv){
    
    
    return END_SUCCESS;
}























// IGNORE ME

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

int main(int argc, char** argv){
    bar("Program Start");
    
    int errorCode = code(argc, argv);
    
    bar("Program End");
    return errorCode;
}


