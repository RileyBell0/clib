#include "../std/dir/directory.h"
#include "../std/string.h"
#include "../std/fileIO.h"

#define HAS_FILENAME_ARG 2
#define HAS_FOLDER_ARG 3
#define OUTPUT_FILENAME_ARG 1
#define FOLDER_ARG 2
#define DEFAULT_OUT_FILENAME "all_files.txt"
#define DEFAULT_PATH_ARG "."

int main(int argc, char** argv) {
    
    void** my_arr = NULL;
    my_arr = safe_malloc(sizeof(void*) * 1000);
    for (int i = 0; i < 1000; i++) {
        my_arr[i] = safe_malloc(sizeof(void*));
    }
    
    return 0;
}