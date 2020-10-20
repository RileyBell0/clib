#include "../directory.h"

/*
 * if (extension == NULL || extension[0] == NULL) then there is no extension
*/
char* getFileExtension(char *fileName)
{
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
                extension = &fileName[i + 1];
            }
        }

        ++i;
    }

    return extension;
}

char *removeFileExtension(char *fileName)
{
    int i = 0;
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

dynamicArray getAllFiles(DIR* d)
{
    dynamicArray files = new_dynamic_array(sizeof(struct dirent));

    struct dirent * entry;

    while ((entry = readdir(d)) != NULL) {
        if (entry->d_type == DT_REG) { /* If the entry is a regular file */
            dynamic_array_append(&files, entry);
        }
    }
    
    return files;
}

/*
 * Returns an array of 'struct dirent' from <dirent.h>
*/
dynamicArray array_GetFilesWithExtension(DIR* d, string extension)
{
    struct dirent *file;
    dynamicArray matchingFiles = new_dynamic_array(sizeof(struct dirent));

    // Get all files in the directory
    dynamicArray allFiles = getAllFiles(d);
    
    for (unsigned int i = 0; i < allFiles.len; i++)
    {
        // Get the next file
        file = &((struct dirent*)allFiles.dat)[i];

        string fileExtension = string_from_cstring(getFileExtension(file->d_name));
    
        // Extension matches the requested extension
        if (((extension.str == NULL || extension.len == 0) && fileExtension.len == 0) || 
            (fileExtension.len != 0 && strcmp(fileExtension.str,extension.str) == 0))
        {
            dynamic_array_append(&matchingFiles, file);
        }
    }

    dynamic_array_destroy(allFiles);

    return matchingFiles;
}

list list_GetFilesWithExtension(DIR* d, string extension)
{
    struct dirent *file;
    list matchingFiles = new_list(sizeof(struct dirent));

    // Get all files in the directory
    dynamicArray allFiles = getAllFiles(d);

    for (unsigned int i = 0; i < allFiles.len; i++)
    {
        // Get the next file
        file = &((struct dirent*)allFiles.dat)[i];

        string fileExtension = string_from_cstring(getFileExtension(file->d_name));
    
        // Extension matches the requested extension
        if (((extension.str == NULL || extension.len == 0) && fileExtension.len == 0) || 
            (fileExtension.len != 0 && strcmp(fileExtension.str,extension.str) == 0))
        {
            list_append(&matchingFiles, file);
        }
    }

    dynamic_array_destroy(allFiles);

    return matchingFiles;
}

string getSubDirectory(string basePath, string pathSeperator, string dirName)
{
    string sourceFiles = new_string(basePath.len + pathSeperator.len + dirName.len);

    string_write(&sourceFiles, basePath);
    string_write(&sourceFiles, pathSeperator);
    string_write(&sourceFiles, dirName);

    return sourceFiles;
}