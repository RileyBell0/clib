#include "../fileIO.h"

char *path_file_extension(char *fileName)
{
    if (!fileName)
    {
        return NULL;
    }
    char *extension = NULL;

    int i = 0;
    while (fileName[i] != '\0')
    {
        if (fileName[i] == EXTENSION_SEPERATING_CHAR)
        {
            // We havent reached the end of the string
            if (fileName[i + 1] != '\0')
            {
                // We have the start of what could be a valid extension
                // The last valid potential extension start-point is returned
                extension = &fileName[i + 1];
            }
        }

        ++i;
    }

    return extension;
}

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
    char* bufferstr = cstr(buffer);
    unsigned int charsWritten = 0;
    while ((c = getc(file)) != EOF && c != '\n' && c != '\r')
    {
        // Need more room to store the next line
        if (charsWritten == buffer->max_len)
        {
            string_extend(buffer);
        }

        bufferstr[charsWritten++] = c;
    }
    buffer->len = charsWritten;

    bufferstr[buffer->len] = '\0';

    // Failed to read anything
    if (c == EOF && charsWritten == 0)
    {
        return FALSE;
    }

    // Read something from the file
    return TRUE;
}

string_t getFileName(string_t *path)
{
    string_t fileName = new_string(0);
    int found = FALSE;
    char pathSeperator = PATH_SEPERATOR_CHAR;
    char* pathstr = cstr(path);
    for (int i = path->len; i >= 0; i--)
    {
        // Found the first directory character
        if (pathstr[i] == pathSeperator)
        {
            // The file name starts at the next character
            fileName = string_make(&pathstr[i + 1]);
            // Copy the string starting from the character after 'i' to a new string
            fileName = string_copy(&fileName);

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

void remove_file_extensions(list_t files)
{
    // Removing the extension from the program names
    list_node_t *node = files.first_node;

    while (node)
    {
        removeFileExtension(cstr(((string_t *)node->data)));

        // Update the length of the string
        ((string_t *)node->data)->len = strlen(cstr(((string_t *)node->data)));
        node = node->next;
    }
}

char* getFileExtension(string_t *fileName)
{
    // Start at the end of the string and work backwards
    // until the extension seperating char is recieved
    char* filestr = cstr(fileName);
    for (unsigned int i = fileName->len - 1; i >= 0; i--)
    {
        if (filestr[i] == EXTENSION_SEPERATING_CHAR)
        {
            // Return a pointer to the char after the extension
            return &filestr[i+1];
        }
    }
    return NULL;
}

list_t get_file_names(list_t files)
{
    list_t prog_names = new_list(sizeof(string_t));

    list_node_t *node = files.first_node;
    while (node)
    {
        // Get the path to the current file from the node
        string_t *file_path = (string_t *)node->data;

        // Extract the file's name from the path and remove its extension
        string_t file_name = getFileName(file_path);

        // Save the file's name
        list_append(&prog_names, &file_name);

        node = node->next;
    }

    return prog_names;
}

list_t fileio_read_all_lines_list(char *fileName)
{
    list_t lines = new_list(sizeof(string_t));

    // Open the given file
    FILE *toRead = fopen(fileName, MODE_READ);

    // If the file exists and could be opened
    if (toRead)
    {
        /*
         * Creating the buffer, will be resized automatically to fit lines if
         * not large enough
        */
        string_t buffer = new_string(DEFAULT_BUFFER_LEN);

        /*
         * Reads all the lines of the file and appends them to the given list
        */
        while (fileio_next_line(toRead, &buffer))
        {
            if (buffer.len > 0)
            {
                string_t valid_string = string_copy(&buffer);
                list_append(&lines, &valid_string);
            }
        }

        // Done with the file, close it
        fileio_close(toRead);

        string_destroy(&buffer);
    }

    return lines;
}