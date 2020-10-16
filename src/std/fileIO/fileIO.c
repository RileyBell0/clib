#include "fileIO.h"

FILE* fileio_open_safe(char* filePath, int isReading){
    FILE* newFile = NULL;

    if (isReading){
        newFile = fopen(filePath,MODE_READ);
    }
    else{
        newFile = fopen(filePath,MODE_WRITE);
    }

    assert(newFile);
    
    return newFile;
}

FILE* fileio_open(char* filePath, int isReading){
    FILE* newFile = NULL;

    if (isReading){
        newFile = fopen(filePath,MODE_READ);
    }
    else{
        newFile = fopen(filePath,MODE_WRITE);
    }
    
    return newFile;
}

void fileio_close(FILE* file){
    if (file != NULL){
        fclose(file);
    }
}

int fileio_next_line(FILE* file, string buffer){
    if (file == NULL){
        return FALSE;
    }

    char c;
    int i = 0;
    while ((c = getc(file)) != EOF && c != '\n' && c != '\r')
    {
        if (i > buffer.len)
        {

        }
        buffer.str[i++] = c;
        
    }
    buffer.str[buffer.len] = '\0';

    return TRUE;
}