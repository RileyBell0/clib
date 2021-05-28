#include "../std/dir/directory.h"
#include "../std/string.h"
#include "../std/fileIO.h"

#define HAS_FILENAME_ARG 2
#define OUTPUT_FILENAME_ARG 1
#define DEFAULT_OUT_FILENAME "all_files.txt"

int main(int argc, char** argv) {
    string_t output_name = new_string(0);
    string_t path = string_make(".");

    if (argc >= HAS_FILENAME_ARG) {
        string_write_c(&output_name, argv[OUTPUT_FILENAME_ARG]);
    }
    else{
        string_write_c(&output_name, DEFAULT_OUT_FILENAME);
    }

    alist_t all_files = dir_all_files_recur(&path);

    FILE* outfile = fileio_open_safe(cstr(&output_name), FALSE);

    alist_iterator_t it = new_alist_iterator(&all_files, TRUE);
    for (string_t *element = it.first(&it); !it.done(&it);
         element = it.next(&it)) {
      fprintf(outfile, "%s\n", cstr(element));
    }

    fclose(outfile);

    return 0;
}