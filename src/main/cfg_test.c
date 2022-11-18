#include <stdio.h>
#include "../std/adv/config.h"

int main(int argc, char **argv)
{
  string_t buffer = empty_string();
  string_t *buf = &buffer;
  while (true)
  {
    printf("Options\n1: Read file\n2: Exit\n");
    fileio_next_line(stdin, buf);
    int choice = atoi(cstr(buf));
    switch (choice)
    {
    case 1:
      printf("Enter path to cfg file: ");
      fileio_next_line(stdin, buf);
      dict_t res = config_read(cstr(buf));
      for (size_t i = 0; i < res.data.len; i++)
      {
        dict_node_t *node = (dict_node_t *)vector_get(&res.data, i);
        printf("{%s: %s}", cstr(&node->key), cstr((string_t *)node->value));
        if (i != res.data.len - 1)
        {
          printf(" > ");
        }
      }
      printf("\n");
      dict_destroy(&res);
      break;
    case 2:
      break;
      break;
    }
  }

  return 0;
}