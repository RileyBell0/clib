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
