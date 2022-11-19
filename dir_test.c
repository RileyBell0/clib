#include <stdio.h>
#include "../std/sys/directory.h"
#include "../std/struc/list.h"
#include "../std/fileIO.h"
#include "../std/string.h"
#include "../std/struc/array.h"

/*
 * TODO delete index 0 with size 1 => break
 * this is due to it.done() triggering when it shouldnt
 */

void options();

void options()
{
  printf("OPTIONS:\n------\n1: files\n2: recur\n");
}

void disp_results(array_t res)
{
  printf("Res.len %zu\n", res.len);
  for (size_t i = 0; i < res.len; i++)
  {
    struct dirent *entry = (struct dirent *)array_get(&res, i);
    printf("%zu) \"%s\"\n", i, entry->d_name);
  }
}
void disp_results2(array_t res)
{
  printf("Res.len %zu\n", res.len);
  for (size_t i = 0; i < res.len; i++)
  {
    string_t *entry = (string_t *)array_get(&res, i);
    printf("%zu) \"%s\"\n", i, cstr(entry));
  }
}

void op1()
{
  printf("Enter dir: ");
  string_t buf = empty_string();
  fileio_next_line(stdin, &buf);
  array_t entries = dir_all_entries(cstr(&buf));
  disp_results(entries);
  array_destroy(&entries);
  string_destroy(&buf);
}
void op2()
{
  printf("Enter dir: ");
  string_t buf = empty_string();
  fileio_next_line(stdin, &buf);
  array_t entries = dir_all_files_recur(cstr(&buf));
  printf("outta there\n");
  disp_results2(entries);
  array_destroy(&entries);
  string_destroy(&buf);
}

int main(int argc, char **argv)
{
  string_t str = empty_string();
  string_t *buf = &str;

  while (true)
  {
    options();
    if (fileio_next_line(stdin, buf))
    {
      int choice = atoi(cstr(buf));
      switch (choice)
      {
      case 1:
        op1();
        break;
      case 2:
        op2();
        break;
      default:
        break;
      }
    }
  }
  return 0;
}
