#include "../std/dir/directory.h"
#include "../std/fileIO.h"
#include "../std/string.h"

#define HAS_FILENAME_ARG 2
#define OUTPUT_FILENAME_ARG 1
#define DEFAULT_OUT_FILENAME "all_files.txt"
#define SHOULD_PRINT false

int main(int argc, char **argv) {
  string_t output_name = new_string(0);
  string_t path = new_string(DEFAULT_BUFFER_LEN);

  printf("What path: ");
  fileio_next_line(stdin, &path);
  printf("Path: %s\n", cstr(&path));

  if (argc >= HAS_FILENAME_ARG) {
    string_write_c(&output_name, argv[OUTPUT_FILENAME_ARG]);
  } else {
    string_write_c(&output_name, DEFAULT_OUT_FILENAME);
  }


  FILE *outfile = fileio_open_safe(cstr(&output_name), false);
  alist_iterator_t it;

  alist_t all_files = dir_all_file_names(&path, NULL, NULL);
  fprintf(outfile, "\n\n----------All Files\n");
  fprintf(outfile, "Found %d Files!\n", all_files.size);
  if (SHOULD_PRINT) {
    fprintf(stdout, "\n\n----------All Files\n");
    fprintf(stdout, "Found %d Files!\n", all_files.size);
  }
  it = new_alist_iterator(&all_files, true);
  for (string_t *element = it.first(&it); !it.done(&it);
       element = it.next(&it)) {
    if (SHOULD_PRINT) {
      fprintf(stdout, "%s\n", cstr(element));
    }
    fprintf(outfile, "%s\n", cstr(element));
  }
  if (SHOULD_PRINT) {
    fprintf(stdout, "\n");
  }
  fprintf(outfile, "\n");


  alist_t all_entries = dir_all_entry_names(&path, NULL, NULL);
  fprintf(outfile, "\n\n----------All Entries\n");
  fprintf(outfile, "Found %d Entries!\n", all_entries.size);
  if (SHOULD_PRINT) {
    fprintf(stdout, "\n\n----------All Entries\n");
    fprintf(stdout, "Found %d Entries!\n", all_entries.size);
  }
  it = new_alist_iterator(&all_entries, true);
  for (string_t *element = it.first(&it); !it.done(&it);
       element = it.next(&it)) {
    fprintf(outfile, "%s\n", cstr(element));
    if (SHOULD_PRINT) {
      fprintf(stdout, "%s\n", cstr(element));
    }
  }
  fprintf(outfile, "\n");
  if (SHOULD_PRINT) {
    fprintf(stdout, "\n");
  }


  alist_t all_files_recur = dir_all_files_recur(&path, NULL, NULL, false);
  fprintf(outfile, "\n\n----------All Files Recur\n");
  fprintf(outfile, "Found %d Files Recursively!\n", all_files_recur.size);
  if (SHOULD_PRINT) {
    fprintf(stdout, "\n\n----------All Files Recur\n");
    fprintf(stdout, "Found %d Files Recursively!\n", all_files_recur.size);
  }
  it = new_alist_iterator(&all_files_recur, true);
  for (string_t *element = it.first(&it); !it.done(&it);
       element = it.next(&it)) {
    fprintf(outfile, "%s\n", cstr(element));
    if (SHOULD_PRINT) {
      fprintf(stdout, "%s\n", cstr(element));
    }
  }
  if (SHOULD_PRINT) {
    fprintf(stdout, "\n");
  }
  fprintf(outfile, "\n");

  fclose(outfile);

  return 0;
}