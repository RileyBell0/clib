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
  list_destroy(list, delete_func);
}

void options()
{
  printf("OPTIONS:\n------\n1: append\n2: delete\n3: delete at\n4: exit\n5: destroy\n6: pop\n");
}

void pop(list_t *list)
{
  string_t buffer = empty_string();
  printf("Enter an index: ");
  fileio_next_line(stdin, &buffer);
  int ind = atoi(cstr(&buffer));
  int dest = -1;
  list_pop(list, &dest, ind);
  printf("Popped %d\n", dest);
}

void deleteone(list_t *list, string_t *buf)
{
  printf("Enter a number: ");
  fileio_next_line(stdin, buf);
  int del = atoi(cstr(buf));
  list_remove(list, &del);
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
  int add = atoi(cstr(buf));
  list_append(list, &add);
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
    printf("%d", *(int *)&node[1]);
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
    printf("%d", *(int *)&node[1]);
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
  string_t str = string_new(DEFAULT_BUFFER_LEN);
  string_t *buf = &str;
  list_t list = list_new(sizeof(int));

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
        list_destroy(&list, NULL);
        return 0;
        break;
      case 5:
        string_destroy(buf);
        dest(&list);
        break;
      case 6:
        pop(&list);
      default:
        break;
      }
    }
  }
  return 0;
}
