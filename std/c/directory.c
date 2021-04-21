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

// TODO Clunky but will get the job done
alist_t dir_all_entries_list(string_t* path)
{
    DIR *d = opendir(cstr(path));
    alist_t entries = new_alist(sizeof(struct dirent));

    if (!d)
    {
        return entries;
    }

    struct dirent *entry;

    // there's a good chance the filepath may be longer than the small string size, so pre-allocating it
    string_t filePath = new_string(DEFAULT_BUFFER_LEN);
    string_write(&filePath, path);

    // The file name probably wont be too long so no need to allocate memory for it
    string_t entryName = new_string(0);

    while ((entry = readdir(d)))
    {
        if (entry->d_type == DT_UNKNOWN)
        {
            string_clear(&filePath);
            string_clear(&entryName);

            // Extract the file's name from the dirent struct
            string_write_c(&entryName, entry->d_name);

            // Make a new string containing the path to (and including) the file
            string_write_c(&filePath, PATH_SEPERATOR);
            string_write(&filePath, &entryName);

            // Try harder to get the file's type
            entry->d_type = get_file_type(cstr(&filePath));
        }

        // Copy the contents of 'entry'
        alist_append(&entries, entry);
    }

    // CLEANUP
    string_destroy(&filePath);
    string_destroy(&entryName);

    return entries;
}

alist_t dir_all_entries_of_type(string_t* path, unsigned char type)
{
    /*
     * This is essentially the code above (dir_all_entries_list) but
     * with a check for a given type 
    */
    DIR *d = opendir(cstr(path));
    alist_t entries = new_alist(sizeof(struct dirent));
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
            string_t entryName = string_make(entry->d_name);
            string_t filePath = string_new_concat(path, &entryName);
            entry->d_type = get_file_type(cstr(&filePath));
            string_destroy(&filePath);
        }
        if (entry->d_type == type)
        {
            // Copy the contents of 'entry'
            alist_append(&entries, entry);
        }
    }

    return entries;
}

ordered_dirent_t new_ordered_dirent_t()
{

    ordered_dirent_t ordered;
    ordered.block = new_alist(sizeof(struct dirent));
    ordered.character = new_alist(sizeof(struct dirent));
    ordered.directory = new_alist(sizeof(struct dirent));
    ordered.fifo_pipe = new_alist(sizeof(struct dirent));
    ordered.link = new_alist(sizeof(struct dirent));
    ordered.regular = new_alist(sizeof(struct dirent));
    ordered.socket = new_alist(sizeof(struct dirent));
    ordered.unknown = new_alist(sizeof(struct dirent));
    return ordered;
}

void ordered_dirent_insert(ordered_dirent_t *ordered, struct dirent *entry)
{
    switch (entry->d_type)
    {
    case DT_BLK:
        alist_append(&ordered->block, entry);
        break;
    case DT_CHR:
        alist_append(&ordered->character, entry);
        break;
    case DT_DIR:
        alist_append(&ordered->directory, entry);
        break;
    case DT_FIFO:
        alist_append(&ordered->fifo_pipe, entry);
        break;
    case DT_LNK:
        alist_append(&ordered->link, entry);
        break;
    case DT_REG:
        printf("regular\n");
        alist_append(&ordered->regular, entry);
        break;
    case DT_SOCK:
        alist_append(&ordered->socket, entry);
        break;
    default:
        alist_append(&ordered->unknown, entry);
        break;
    }
}

void ordered_dirent_destroy(ordered_dirent_t* ordered)
{
    alist_destroy(&ordered->block);
    alist_destroy(&ordered->character);
    alist_destroy(&ordered->directory);
    alist_destroy(&ordered->fifo_pipe);
    alist_destroy(&ordered->link);
    alist_destroy(&ordered->regular);
    alist_destroy(&ordered->socket);
    alist_destroy(&ordered->unknown);
}

ordered_dirent_t dir_all_entries_categorised(string_t* path)
{
    ordered_dirent_t ordered = new_ordered_dirent_t();

    DIR *d = opendir(cstr(path));
    if (!d)
    {
        return ordered;
    }

    struct dirent *entry;
    printf("path \"%s\"\n", cstr(path));
    string_t filePath = new_string(path->len + SMALL_BUFFER_LEN);
    string_write(&filePath, path);
    printf("path end \"%s\"\n", cstr(&filePath));
    while ((entry = readdir(d)))
    {
        if (entry->d_type == DT_UNKNOWN)
        {
            // Try harder to get the file's type
            filePath.len = path->len;
            cstr(&filePath)[filePath.len] = '\0';

            printf("copy d_name\n");
            string_t entryName = string_make(entry->d_name);
            printf("write path\n");
            string_write(&filePath, &entryName);
            entry->d_type = get_file_type(cstr(&filePath));
        }
            printf("dirent insert\n");
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
 *      alist_t files (list of strings)
 *      alist_t subDirs
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
alist_t dir_files_with_extension_recur(string_t *path, string_t *extension)
{
  // TODO write this function
    // The return type is a list containing the paths to all matching files
    alist_t matchingFiles = new_alist(sizeof(string_t));

    // // Converting the system-specific path seperator into a string
    // string_t path_seperator = string_make(PATH_SEPERATOR);

    // // Getting all the directory entries in the current directory
    // ordered_dirent_t entries = dir_all_entries_categorised(path);

    // // Add all files with the matching extension
    // list_node_t *node = entries.regular.first_node;

    // // THE PART DEALING WITH THE CURRENT DIRECTORY
    // // Storing the current directory's path into an editable buffer string
    // string_t filePath = new_string(DEFAULT_BUFFER_LEN);
    // string_write(&filePath, &path);
    // while (node)
    // {
    //     // Resetting the current file's path back to the path
    //     // to the current directory
    //     // (after last loop /usr/dir/myfile.txt becomes /usr/dir)
    //     cstr(&filePath)[path.len] = '\0';
    //     filePath.len = path.len;

    //     // Converting the file's name into a string for easier use
    //     string_t fileName = string_make(ldirentnode(node)->d_name);

    //     // If the extension matches the requested one
    //     if (cstring_equals(cstr(&extension), getFileExtension(&fileName)))
    //     {
    //         // Add the file's name to the end of the string
    //         // eg. /usr/dir -> /usr/dir/myfile.extension
    //         string_write(&filePath, &path_seperator);
    //         string_write(&filePath, &fileName);

    //         // Make a copy of this and save it to the list
    //         string_t matching = string_copy(&filePath);
    //         alist_append(&matchingFiles, &matching);
    //     }
    //     node = node->next;
    // }
    // string_destroy(&filePath);


    // // THE RECURSION PART

    // // Go through the directories
    // node = entries.directory.first_node;
    // string_t subDirPath = string_copy(&path);
    // while (node)
    // {
    //     // restrict the path back to the current directory (the path recieved by this function)
    //     cstr(&subDirPath)[path.len] = '\0';

    //     string_t subDirName = string_make(ldirentnode(node)->d_name);

    //     if (is_normal_dir(subDirName))
    //     {
    //         // Make the new path's string
    //         string_write(&subDirPath, &path_seperator);
    //         string_write(&subDirPath, &subDirName);
    //         // Look through the directory
    //         alist_t subDirFiles = dir_files_with_extension_recur(subDirPath, extension);
    //         list_combine(&matchingFiles, &subDirFiles);
    //     }
    //     node = node->next;
    // }
    // string_destroy(&subDirPath);

    return matchingFiles;
}

int is_normal_dir(string_t *dirName)
{
    if (dirName->len <= 2)
    {
        if (cstring_equals(cstr(dirName), ".") || cstring_equals(cstr(dirName), ".."))
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