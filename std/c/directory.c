#include "../directory.h"

unsigned char get_file_type(char *path)
{
    struct stat sb;
    if (stat(path, &sb) == -1)
    {
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
            string_t entryName = string_from_cstring(entry->d_name);
            string_t filePath = string_new_concat(&path, &entryName);
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
            string_t entryName = string_from_cstring(entry->d_name);
            string_t filePath = string_new_concat(&path, &entryName);
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
    string_t filePath = new_string(path.len + SMALL_BUFFER_LEN);
    string_write_single(&filePath, &path);
    while ((entry = readdir(d)))
    {
        if (entry->d_type == DT_UNKNOWN)
        {
            // Try harder to get the file's type
            filePath.len = path.len;
            filePath.str[filePath.len] = '\0';

            string_t entryName = string_from_cstring(entry->d_name);
            string_write_single(&filePath, &entryName);
            entry->d_type = get_file_type(filePath.str);
        }
        ordered_dirent_insert(&ordered, entry);
    }
    string_destroy(&filePath);

    closedir(d);
    return ordered;
}

//TODO
/*
 * This could do with some nice changes
 * like for instance we store the starting path of each string related to each matching file path
 * this is wasted space, and thus wasted performance.
 * 
 * instead we could create our own tree-like structure to act as a directory which we can filter
 * to only have stuff like files with the matching directory, then we can make an iterator
 * which could keep within it the current node
 * so im thinking we make a tree structure, each node has the name of itself (the directory)
 * and it has a list of files, and these only have thier file names in them
 * 
 * then the iterator can keep track of the list it's in and the path up to that point.
 * so it would start at the root, list all the files in that directory (while also hvaing it's path attached to the file if requested)
 * and then when you call next it'll go to the next node in the tree, put its name in the plae of the last file and return that string
 * 
 * this means that its going to be somehting like this
 * 
 * directorytreenode
 * {
 *      string_t dirName
 *      list_t files (list of strings)
 *      list_t subDirs
 * }
 * 
 * idk, i want a function which remembers its state (through using a iterator struct) which can be used to progressively
 * traverse a tree at each call. This might not be easy, but it shouldn be too hard
 * 
 * itll need to know where it is in the tree, what it's looking at and how to get to the next element from what it's looking at
 * and from there it should be a piece of cake
 * 
 * the only problem is that this iterator might have to allocate memory
 * so we're going to have to see how much we can get away with without allocating memory because fuck that
 * and idk what im doing but im sure i can figure it out
 * 
 * This function takes a path to a 
*/
list_t dir_files_with_extension_recur(string_t path, string_t extension)
{
    // The return type is a list containing the paths to all matching files
    list_t matchingFiles = new_list(sizeof(string_t));

    // Converting the system-specific path seperator into a string
    string_t path_seperator = string_from_cstring(PATH_SEPERATOR);

    // Getting all the directory entries in the current directory
    ordered_dirent_t entries = dir_all_entries_categorised(path);

    // Add all files with the matching extension
    list_node_t *node = entries.regular.first_node;

    // THE PART DEALING WITH THE CURRENT DIRECTORY

    // Storing the current directory's path into an editable buffer string
    string_t filePath = new_string(DEFAULT_BUFFER_LEN);
    string_write_single(&filePath, &path);
    while (node)
    {
        // Resetting the current file's path back to the path
        // to the current directory
        // (after last loop /usr/dir/myfile.txt becomes /usr/dir)
        filePath.str[path.len] = '\0';
        filePath.len = path.len;

        // Converting the file's name into a string for easier use
        string_t fileName = string_from_cstring(ldirentnode(node)->d_name);

        // If the extension matches the requested one
        if (cstring_equals(extension.str, getFileExtension(fileName)))
        {
            // Add the file's name to the end of the string
            // eg. /usr/dir -> /usr/dir/myfile.extension
            string_write_single(&filePath, &path_seperator);
            string_write_single(&filePath, &fileName);

            // Make a copy of this and save it to the list
            string_t matching = string_copy(&filePath);
            list_append(&matchingFiles, &matching);
        }
        node = node->next;
    }
    string_destroy(&filePath);

    // THE RECURSION PART

    // Go through the directories
    node = entries.directory.first_node;
    string_t subDirPath = string_copy(&path);
    while (node)
    {
        // restrict the path back to the current directory (the path recieved by this function)
        subDirPath.str[path.len] = '\0';

        string_t subDirName = string_from_cstring(ldirentnode(node)->d_name);

        if (is_normal_dir(subDirName))
        {
            // Make the new path's string
            string_write_single(&subDirPath, &path_seperator);
            string_write_single(&subDirPath, &subDirName);
            // Look through the directory
            list_t subDirFiles = dir_files_with_extension_recur(subDirPath, extension);
            list_combine(&matchingFiles, &subDirFiles);
        }
        node = node->next;
    }
    string_destroy(&subDirPath);

    return matchingFiles;
}

int is_normal_dir(string_t dirName)
{
    if (dirName.len <= 2)
    {
        if (cstring_equals(dirName.str, ".") || cstring_equals(dirName.str, ".."))
        {
            return FALSE;
        }
    }
    return TRUE;
}

struct dirent* ldirentnode(list_node_t *node)
{
    return (struct dirent*)node->data;
}

string_t *lstrnode(list_node_t *node)
{
    return (string_t *)node->data;
}