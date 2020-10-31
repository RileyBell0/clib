#include "fileIO.h"

FILE *fileio_open_safe_advanced(char *filePath, char *mode)
{
    FILE *newFile = NULL;

    newFile = fopen(filePath, mode);

    assert(newFile);

    return newFile;
}

FILE *fileio_open_safe(char *filePath, int isReading)
{
    FILE *newFile = NULL;

    if (isReading)
    {
        newFile = fopen(filePath, MODE_READ);
    }
    else
    {
        newFile = fopen(filePath, MODE_WRITE);
    }

    assert(newFile);

    return newFile;
}

FILE *fileio_open(char *filePath, int isReading)
{
    FILE *newFile = NULL;

    if (isReading)
    {
        newFile = fopen(filePath, MODE_READ);
    }
    else
    {
        newFile = fopen(filePath, MODE_WRITE);
    }

    return newFile;
}

FILE *fileio_open_advanced(char *filePath, char *mode)
{
    FILE *newFile = NULL;

    newFile = fopen(filePath, mode);

    return newFile;
}

void fileio_close(FILE *file)
{
    if (file != NULL)
    {
        fclose(file);
    }
}

int fileio_next_line(FILE *file, string_t *buffer)
{
    if (file == NULL || buffer == NULL)
    {
        return FALSE;
    }

    char c;
    unsigned int charsWritten = 0;
    while ((c = getc(file)) != EOF && c != '\n' && c != '\r')
    {
        // Need more room to store the next line
        if (charsWritten == buffer->len)
        {
            string_extend(buffer);
        }

        buffer->str[charsWritten++] = c;
    }
    buffer->len = charsWritten;

    buffer->str[buffer->len] = '\0';

    // Failed to read anything
    if (buffer->len == 0)
    {
        return FALSE;
    }

    // Read something from the file
    return TRUE;
}

string_t getFileName(string_t path)
{
    string_t fileName = {0};
    int found = FALSE;
    char pathSeperator = PATH_SEPERATOR_CHAR;
    for (int i = path.len; i >= 0; i--)
    {
        // Found the first directory character
        if (path.str[i] == pathSeperator)
        {
            // The file name starts at the next character
            fileName = string_from_cstring(&path.str[i + 1]);
            // Copy the string starting from the character after 'i' to a new string
            fileName = string_copy(fileName);

            found = TRUE;

            break;
        }
    }

    if (!found)
    {
        fileName = string_copy(path);
    }

    return fileName;
}

char *removeFileExtension(char *fileName)
{
    // Was a valid string recieved?
    if (!fileName)
    {
        return NULL;
    }

    // Start at the end of the string and work backwards
    // until the extension seperating char is recieved
    for (unsigned int i = strlen(fileName); i >= 0; i--)
    {
        if (fileName[i] == EXTENSION_SEPERATING_CHAR)
        {
            fileName[i] = '\0';
            return fileName;
        }
    }
    return fileName;
}