#include <stdio.h>
#include "../std/struc/list.h"
#include "../std/fileIO.h"
#include "../std/string.h"

/*
 * TODO delete index 0 with size 1 => break
 * this is due to it.done() triggering when it shouldnt
 */

void options();
void print_list(list_t list);
void addone(list_t *list, string_t *buf);
void deleteat(list_t *list, string_t *buf);
void deleteone(list_t *list, string_t *buf);
void dest(list_t *list);

void delete_func(void *elem)
{
}

void dest(list_t *list)
{
  list_destroy(list);
}

void options()
{
  printf("OPTIONS:\n------\n1: append\n2: delete\n3: delete at\n4: exit\n5: destroy\n6: pop\n7: get");
}

void pop(list_t *list)
{
  string_t buffer = empty_string();

  printf("Enter an index: ");
  fileio_next_line(stdin, &buffer);

  int ind = atoi(cstr(&buffer));
  string_t dest = empty_string();
  list_pop(list, &dest, ind);

  printf("Popped %s\n", cstr(&dest));

  string_destroy(&dest);
  string_destroy(&buffer);
}

int void_string_compare(void *str1, void *str2)
{
  return string_compare((string_t *)str1, (string_t *)str2);
}

void deleteone(list_t *list, string_t *buf)
{
  printf("Enter a number: ");
  fileio_next_line(stdin, buf);
  list_remove(list, (void *)buf, void_string_compare);
}
void deleteat(list_t *list, string_t *buf)
{
  printf("Enter an index: ");
  fileio_next_line(stdin, buf);
  int del = atoi(cstr(buf));
  list_remove_at(list, del);
}

void addone(list_t *list, string_t *buf)
{
  printf("Enter a number: ");
  fileio_next_line(stdin, buf);
  string_t add = string_copy(buf);
  list_append(list, &add);
}

void get(list_t *list)
{
  printf("Enter an index: ");
  string_t buffer = empty_string();
  fileio_next_line(stdin, &buffer);
  int del = atoi(cstr(&buffer));
  string_t *elem = (string_t *)list_get(list, del);
  printf("Got '%s'\n", cstr(elem));
  string_destroy(&buffer);
}

void print_list(list_t list)
{
  printf("LIST:\n-------\n");
  int first = -1;
  int last = -1;
  if (list.first_node)
  {
    first = *(int *)(&list.first_node[1]);
  }
  if (list.last_node)
  {
    last = *(int *)(&list.last_node[1]);
  }
  printf("Size: %d | First: %d  Last %d\n\n", list.size, first, last);

  list_node_t *node = list.first_node;
  while (node)
  {
    printf("%s", cstr((string_t *)&node[1]));
    if (node->next)
    {
      printf(" > ");
    }
    node = node->next;
  }
  node = list.last_node;
  printf("\n");
  while (node)
  {
    printf("%s", cstr((string_t *)&node[1]));
    if (node->prev)
    {
      printf(" < ");
    }
    node = node->prev;
  }
  printf("\n");

  printf("\n");
}

int main(int argc, char **argv)
{
  string_t str = empty_string();
  string_t *buf = &str;
  list_t list = list_new(sizeof(string_t), void_string_destroy);

  while (true)
  {
    print_list(list);
    options();
    if (fileio_next_line(stdin, buf))
    {
      int choice = atoi(cstr(buf));
      switch (choice)
      {
      case 1:
        addone(&list, buf);
        break;
      case 2:
        deleteone(&list, buf);
        break;
      case 3:
        deleteat(&list, buf);
        break;
      case 4:
        string_destroy(buf);
        list_destroy(&list);
        return 0;
        break;
      case 5:
        string_destroy(buf);
        dest(&list);
        break;
      case 6:
        pop(&list);
        break;
      case 7:
        get(&list);
        break;
      default:
        break;
      }
    }
  }
  return 0;
}
