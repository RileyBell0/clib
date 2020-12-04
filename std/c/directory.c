#include "../directory.h"

unsigned char get_file_type(char *path)
{
    struct stat sb;
    if (stat(path, &sb) == -1)
    {
        printf("stat error\n");
        exit(-1);
    }
    switch (sb.st_mode & S_IFMT)
    {
    case S_IFBLK:
        return DT_BLK;
    case S_IFCHR:
        return DT_CHR;
    case S_IFDIR:
        return DT_DIR;
    case S_IFIFO:
        return DT_FIFO;
    case S_IFLNK:
        return DT_LNK;
    case S_IFREG:
        return DT_REG;
    case S_IFSOCK:
        return DT_SOCK;
    default:
        return DT_UNKNOWN;
    }
}

list_t dir_all_entries_list(string_t path)
{
    DIR *d = opendir(path.str);
    list_t entries = new_list(sizeof(struct dirent));
    if (!d)
    {
        return entries;
    }

    struct dirent *entry;

    while ((entry = readdir(d)))
    {
        if (entry->d_type == DT_UNKNOWN)
        {
            // Try harder to get the file's type
            string_t filePath = string_new_concat(path, string_from_cstring(entry->d_name));
            entry->d_type = get_file_type(filePath.str);
            string_destroy(&filePath);
        }
        // Copy the contents of 'entry'
        list_append(&entries, entry);
    }

    return entries;
}

list_t dir_all_entries_of_type(string_t path, unsigned char type)
{
    /*
     * This is essentially the code above (dir_all_entries_list) but
     * with a check for a given type 
    */
    DIR *d = opendir(path.str);
    list_t entries = new_list(sizeof(struct dirent));
    if (!d)
    {
        return entries;
    }

    struct dirent *entry;

    while ((entry = readdir(d)))
    {
        if (entry->d_type == DT_UNKNOWN)
        {
            // Try harder to get the file's type
            string_t filePath = string_new_concat(path, string_from_cstring(entry->d_name));
            entry->d_type = get_file_type(filePath.str);
            string_destroy(&filePath);
        }
        if (entry->d_type == type)
        {
            // Copy the contents of 'entry'
            list_append(&entries, entry);
        }
    }

    return entries;
}

ordered_dirent_t new_ordered_dirent_t()
{

    ordered_dirent_t ordered;
    ordered.block = new_list(sizeof(struct dirent));
    ordered.character = new_list(sizeof(struct dirent));
    ordered.directory = new_list(sizeof(struct dirent));
    ordered.fifo_pipe = new_list(sizeof(struct dirent));
    ordered.link = new_list(sizeof(struct dirent));
    ordered.regular = new_list(sizeof(struct dirent));
    ordered.socket = new_list(sizeof(struct dirent));
    ordered.unknown = new_list(sizeof(struct dirent));
    return ordered;
}

void ordered_dirent_insert(ordered_dirent_t *ordered, struct dirent *entry)
{
    switch (entry->d_type)
    {
    case DT_BLK:
        list_append(&ordered->block, entry);
        break;
    case DT_CHR:
        list_append(&ordered->character, entry);
        break;
    case DT_DIR:
        list_append(&ordered->directory, entry);
        break;
    case DT_FIFO:
        list_append(&ordered->fifo_pipe, entry);
        break;
    case DT_LNK:
        list_append(&ordered->link, entry);
        break;
    case DT_REG:
        list_append(&ordered->regular, entry);
        break;
    case DT_SOCK:
        list_append(&ordered->socket, entry);
        break;
    default:
        list_append(&ordered->unknown, entry);
        break;
    }
}

void ordered_dirent_destroy(ordered_dirent_t* ordered)
{
    list_destroy(&ordered->block, NULL);
    list_destroy(&ordered->character, NULL);
    list_destroy(&ordered->directory, NULL);
    list_destroy(&ordered->fifo_pipe, NULL);
    list_destroy(&ordered->link, NULL);
    list_destroy(&ordered->regular, NULL);
    list_destroy(&ordered->socket, NULL);
    list_destroy(&ordered->unknown, NULL);
}

ordered_dirent_t dir_all_entries_categorised(string_t path)
{
    ordered_dirent_t ordered = new_ordered_dirent_t();

    DIR *d = opendir(path.str);
    if (!d)
    {
        return ordered;
    }

    struct dirent *entry;

    while ((entry = readdir(d)))
    {
        if (entry->d_type == DT_UNKNOWN)
        {
            // Try harder to get the file's type
            string_t filePath = string_new_concat(path, string_from_cstring(entry->d_name));
            entry->d_type = get_file_type(filePath.str);
            string_destroy(&filePath);
        }
        ordered_dirent_insert(&ordered, entry);
    }

    return ordered;
}

list_t dir_files_with_extension_recur(string_t path, string_t extension)
{
    // struct dirent *file;
    list_t matchingFiles = new_list(sizeof(struct dirent));

    // ordered_dirent_t entries = dir_all_entries_categorised(path);

    // list_node_t *node = entries.regular.first_node;
    // while (node)
    // {
    //     if (strcmp(extension.str, getFileExtension(string_from_cstring(ldirentnode(node)->d_name))) == 0)
    //     {
    //         list_append(&matchingFiles, node->data);
    //     }
    //     node = node->next;
    // }
    // node = entries.directory.first_node;
    // while (node)
    // {
    //     // list_t subDirFiles = dir_files_with_extension_recur(, extension);
    //     // list_combine(&matchingFiles, &subDirFiles);
    //     node = node->next;
    // }

    return matchingFiles;
}

string_t getSubDirectory(string_t basePath, string_t pathSeperator, string_t dirName)
{
    string_t sourceFiles = new_string(basePath.len + pathSeperator.len + dirName.len);

    string_write(&sourceFiles, &basePath, &pathSeperator, &dirName, NULL);

    return sourceFiles;
}

list_t getFilesWithExtensionRecursive(DIR *d, string_t path, string_t pathSeperator, string_t extension)
{
    list_t filesWithExtension = new_list(sizeof(string_t));
    if (!d)
    {
        return filesWithExtension;
    }

    printf("Current Working Directory: %s\n", path.str);

    // int* i = malloc(sizeof(int));
    // if (d)
    // {
    //     printf("Entered a new directory %s\n", path.str);
    //     // Get all files wihtin the current directory
    //     printf("\tIterate Directory Start\n");
    //     list_t allFiles = getAllDirectoryEntryNames(d);
    //     printf("\tIterate Directory END\n");
    //     printf("Got all the file names\n");
    //     list_node_t *node = allFiles.first_node;

    //     // Go through the list of directory entires
    //     while (node)
    //     {
    //         printf("\tLooking at a new Directory Entry\n");
    //         DIR *sub = NULL;

    //         // Save the location of the directory name for easier access
    //         string_t entryName = string_from_cstring(*(char **)node->data);
    //         printf("\tEntryName: %s\n", entryName.str);

    //         // Construct a string for the path to the given entry
    //         string_t entryPath = getSubDirectory(path, pathSeperator, entryName);
    //         printf("\tEntryPath: %s\n", entryPath.str);

    //         // If it is a sub directory
    //         if (strcmp(entryName.str, ".") != 0 && strcmp(entryName.str, "..") != 0)
    //         {
    //             sub = opendir(entryPath.str);
    //         }

    //         // Gets a reference to the extension on the current directory
    //         string_t fileExtension = string_from_cstring(getFileExtension(entryName.str));

    //         printf("Tried to open %s as a subdir\n", entryPath.str);
    //         if (sub)
    //         {
    //             printf("Successfully opened %s as a subdir\n", entryPath.str);
    //             // Re-call this same method
    //             list_t subFiles = getFilesWithExtensionRecursive(sub, entryPath, pathSeperator, extension);
    //             // Close the sub directory
    //             closedir(sub);
    //             // Add the results from the next folder to this one
    //             list_combine(&filesWithExtension, &subFiles);
    //             // Destroy the string to the path of the file
    //             string_destroy(&entryPath);
    //         }
    //         else if (((extension.str == NULL || extension.len == 0) && fileExtension.len == 0) ||
    //                  (fileExtension.len != 0 && strcmp(fileExtension.str, extension.str) == 0))
    //         {
    //             // Extension matches the requested extension
    //             // Save the string's information into the list
    //             list_append(&filesWithExtension, &entryPath);
    //         }
    //         else
    //         {
    //             printf("Entry was not a subdir\n", entryPath.str);
    //             // it was not a directory or a file wiht the extension we want - we no longer need this path
    //             string_destroy(&entryPath);
    //         }

    //         // Go to the next element
    //         node = node->next;
    //     }

    //     printf("Done with the files here\n");

    //     // Destroy the pain
    //     list_destroy(&allFiles, ptr_destroy);
    // }
    // Return a list of all matching files in this and any sub directories
    return filesWithExtension;
}
