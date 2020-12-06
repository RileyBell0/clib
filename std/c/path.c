#include "../path.h"

string_t path_enter_directory(string_t basePath, string_t dirName)
{
    string_t pathSeperator = string_from_cstring(PATH_SEPERATOR);
    string_t sourceFiles = new_string(basePath.len + pathSeperator.len + dirName.len);

    string_write(&sourceFiles, &basePath, &pathSeperator, &dirName, NULL);

    return sourceFiles;
}