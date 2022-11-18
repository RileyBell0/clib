#include "../std/fileIO.h"
#include "../std/struc/dict.h"
#include "../std/string.h"
#include <stdio.h>

void options()
{
  printf("1: Set item\n2: Contains item\n3: Get\n4: Exit\n");
}

void print(dict_t *dict)
{
  printf("Contents:\n");

  for (size_t i = 0; i < dict->data.len; i++)
  {
    dict_node_t *node = (dict_node_t *)vector_get(&dict->data, i);
    string_t *key = &node->key;
    string_t *value = (string_t *)node->value;

    printf("{\"%s\": \"%s\"}", cstr(key), cstr(value));
    if (i != dict->data.len - 1)
    {
      printf("  >  ");
    }
  }
  printf("\n");

  printf("-------\n");
}

void set(dict_t *dict)
{
  string_t buffer = empty_string();
  string_t *buf = &buffer;
  printf("Enter a key: ");
  fileio_next_line(stdin, buf);
  string_t key = string_copy(buf);
  printf("Enter a value: ");
  fileio_next_line(stdin, buf);
  string_t value = string_copy(buf);

  dict_set(dict, &key, &value);

  string_destroy(&key);
  string_destroy(buf);
}
void contains(dict_t *dict)
{
  string_t buffer = empty_string();
  string_t *buf = &buffer;
  printf("Enter a key: ");
  fileio_next_line(stdin, buf);

  if (dict_contains(dict, buf))
  {
    printf("TRUE - CONTAINS %s\n", cstr(buf));
  }
  else
  {
    printf("FALSE - does not contain %s\n", cstr(buf));
  }

  string_destroy(buf);
}
void get(dict_t *dict)
{
  string_t buffer = empty_string();
  string_t *buf = &buffer;
  printf("Enter a key: ");
  fileio_next_line(stdin, buf);
  string_t *res = (string_t *)dict_get(dict, buf);
  printf("RESULT: {\"%s\": \"%s\"}\n", cstr(buf), cstr(res));

  string_destroy(buf);
}

void loop(dict_t *dict, string_t *buf)
{
  while (true)
  {
    print(dict);
    options();
    if (fileio_next_line(stdin, buf))
    {
      int res = atoi(cstr(buf));
      switch (res)
      {
      case 1:
        set(dict);
        break;
      case 2:
        contains(dict);
        break;
      case 3:
        get(dict);
        break;
      case 4:
        return;
      }
    }
  }
}

int main(int argc, char **argv)
{
  string_t buffer = empty_string();
  dict_t dict = dict_new(sizeof(string_t), void_string_destroy);

  loop(&dict, &buffer);

  dict_destroy(&dict);
  string_destroy(&buffer);
  return 0;
}