#include "../directory.h"

/*
 * if (extension == NULL || extension[0] == NULL) then there is no extension
 * 
 * Does not allocate memory
*/
char *getFileExtension(char *fileName)
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

list_t getAllDirectoryEntries(DIR *d)
{
    list_t entries = new_list(sizeof(struct dirent));

    struct dirent *entry;

    while ((entry = readdir(d)) != NULL)
    {
        // Copy the contents of 'entry'
        list_append(&entries, entry);
    }

    return entries;
}

// Allocates memory, returns a list of (char*)
list_t getAllDirectoryEntryNames(DIR *d)
{
    list_t entries = new_list(sizeof(char *));

    struct dirent *entry;

    while ((entry = readdir(d)) != NULL)
    {
        // Copy the contents of 'entry'
        char *name = cstring_copy(entry->d_name);
        list_append(&entries, &name);
    }

    return entries;
}

list_t getAllFiles(DIR *d)
{
    list_t files = new_list(sizeof(struct dirent));

    struct dirent *entry;

    while ((entry = readdir(d)) != NULL)
    {
        if (entry->d_type == DT_REG)
        { /* If the entry is a regular file */
            list_append(&files, &entry);
        }
    }

    return files;
}

list_t getFoldersInDir(DIR *d)
{
    struct dirent *entry;
    list_t folders = new_list(sizeof(struct dirent));

    list_t allEntries = getAllDirectoryEntries(d);

    list_node_t *node = allEntries.first_node;
    while (node)
    {
        entry = (struct dirent *)node->data;
        if (entry->d_type == DT_DIR)
        {
            list_append(&folders, entry);
        }
        node = node->next;
    }

    list_destroy(&allEntries, NULL);

    return folders;
}

list_t getFilesWithExtension(DIR *d, string_t extension)
{
    struct dirent *file;
    list_t matchingFiles = new_list(sizeof(struct dirent));

    // Get all files in the directory
    list_t allFiles = getAllFiles(d);

    list_node_t *node = allFiles.first_node;
    while (node)
    {
        // Get the next file
        file = (struct dirent *)node->data;

        string_t fileExtension = string_from_cstring(getFileExtension(file->d_name));

        // Extension matches the requested extension
        if (((extension.str == NULL || extension.len == 0) && fileExtension.len == 0) ||
            (fileExtension.len != 0 && strcmp(fileExtension.str, extension.str) == 0))
        {
            list_append(&matchingFiles, file);
        }
        node = node->next;
    }

    list_destroy(&allFiles, NULL);

    return matchingFiles;
}

string_t getSubDirectory(string_t basePath, string_t pathSeperator, string_t dirName)
{
    string_t sourceFiles = new_string(basePath.len + pathSeperator.len + dirName.len);

    string_write(&sourceFiles, basePath);
    string_write(&sourceFiles, pathSeperator);
    string_write(&sourceFiles, dirName);

    return sourceFiles;
}

list_t getFilesWithExtensionRecursive(DIR *d, string_t path, string_t pathSeperator, string_t extension)
{
    list_t filesWithExtension = new_list(sizeof(string_t));

    if (d)
    {
        // Get all files wihtin the current directory
        list_t allFiles = getAllDirectoryEntryNames(d);

        list_node_t *node = allFiles.first_node;

        // Go through the list of directory entires
        while (node)
        {
            DIR *sub = NULL;

            // Save the location of the directory name for easier access
            string_t entryName = string_from_cstring(*(char **)node->data);

            // Construct a string for the path to the given entry
            string_t entryPath = getSubDirectory(path, pathSeperator, entryName);

            // If it is a sub directory
            if (strcmp(entryName.str, ".") != 0 && strcmp(entryName.str, "..") != 0)
            {
                sub = opendir(entryPath.str);
            }

            // Gets a reference to the extension on the current directory
            string_t fileExtension = string_from_cstring(getFileExtension(entryName.str));

            if (sub)
            {
                // Re-call this same method
                list_t subFiles = getFilesWithExtensionRecursive(sub, entryPath, pathSeperator, extension);
                // Close the sub directory
                closedir(sub);
                // Add the results from the next folder to this one
                list_combine(&filesWithExtension, &subFiles);
                // Destroy the string to the path of the file
                string_destroy(&entryPath);
            }
            else if (((extension.str == NULL || extension.len == 0) && fileExtension.len == 0) ||
                     (fileExtension.len != 0 && strcmp(fileExtension.str, extension.str) == 0))
            {
                // Extension matches the requested extension
                // Save the string's information into the list
                list_append(&filesWithExtension, &entryPath);
            }
            else
            {
                // it was not a directory or a file wiht the extension we want - we no longer need this path
                string_destroy(&entryPath);
            }

            // Go to the next element
            node = node->next;
        }

        // Destroy the pain
        list_destroy(&allFiles, ptr_destroy);
    }
    // Return a list of all matching files in this and any sub directories
    return filesWithExtension;
}
