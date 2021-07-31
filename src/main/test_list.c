#include "../std/alist.h"
#include "../std/fileIO.h"
#include "../std/list.h"
#include "../std/string.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPTION_EXIT "0"
#define OPTION_APPEND "1"
#define OPTION_VIEW "2"
#define OPTION_REMOVE_AT "3"
#define OPTION_REMOVE "4"
#define OPTION_POP "5"
#define OPTION_EXPLORE "6"
#define OPTION_SWITCH "7"
#define OPTION_COMBINE "8"

void print_alist(alist_t* list);

void print_options();

void add_to_alist(alist_t* list, char* str) {
  string_t to_add = string_make(str);
  alist_append(list, &to_add);
}

int main(int argc, char **argv) {
  string_t buffer_str = string_new(DEFAULT_BUFFER_LEN);
  string_t messages = string_new(DEFAULT_BUFFER_LEN);
  string_t *msgs = &messages;
  string_t *buff = &buffer_str;

  alist_t list = alist_new(sizeof(string_t));
  list.destroy = void_string_destroy;
  list.compare = string_void_compare;
  alist_t list2 = alist_new(sizeof(string_t));
  list2.destroy = void_string_destroy;
  list2.compare = string_void_compare;

  // Add Dummy numbers to alist
  add_to_alist(&list, "0");
  add_to_alist(&list, "1");
  add_to_alist(&list, "2");
  add_to_alist(&list, "3");
  add_to_alist(&list, "4");
  add_to_alist(&list, "5");
  add_to_alist(&list, "6");
  add_to_alist(&list, "7");
  add_to_alist(&list, "8");
  add_to_alist(&list, "9");

  add_to_alist(&list2, "a");
  add_to_alist(&list2, "b");
  add_to_alist(&list2, "c");
  add_to_alist(&list2, "d");
  add_to_alist(&list2, "e");
  add_to_alist(&list2, "f");
  add_to_alist(&list2, "g");
  add_to_alist(&list2, "h");
  add_to_alist(&list2, "i");
  add_to_alist(&list2, "j");

  alist_t* curr_list = &list;
  alist_t* other_list = &list2;

  char* curr_list_val = "1";
  char* other_list_val = "2";

  while (true) {
    printf("\n\n------------\n%s", cstr(msgs));
    string_clear(msgs);
    printf("------------\nSELECTED LIST: %s\n", curr_list_val);
    printf("List Contents: ");
    alist_iterator_t it = alist_iterator_new(curr_list, true);
    for (it.first(&it); !it.done(&it); it.next(&it)) {
      string_t* str = (string_t*)it.element;
      printf("%s ", cstr(str));
    }
    printf("\n");
    printf("List Size: %d    List Capacity: %d\n", curr_list->size, curr_list->capacity);

    print_options();
    printf("Enter your selection: ");
    if (!fileio_next_line(stdin, buff)) {
      string_write_c(msgs, "Error - You must choose something...\n");
      continue;
    }

    char* buffstr = cstr(buff);

    if (cstring_equals(buffstr, OPTION_EXIT)) {
      printf("Exiting...\n");
      string_destroy(buff);
      alist_destroy(curr_list);
      alist_destroy(other_list);
      exit(EXIT_SUCCESS);
    }
    else if (cstring_equals(buffstr, OPTION_APPEND)) {
      printf("What would you like to append? ");
      if (fileio_next_line(stdin, buff)) {
        string_t to_append = string_copy(buff);
        alist_append(curr_list, &to_append);
        string_write_c(msgs, "Appended \"");
        string_write(msgs, &to_append);
        string_write_c(msgs, "\"!\n");
      }
      else {
        string_write_c(msgs, "Error - Nothing appended, no input recieved\n");
      }
    }
    else if (cstring_equals(buffstr, OPTION_VIEW)) {
      print_alist(curr_list);
    }
    else if (cstring_equals(buffstr, OPTION_REMOVE_AT)) {
      printf("Remove at which index? ");
      if (fileio_next_line(stdin, buff)) {
        char* str = cstr(buff);
        int index = atoi(str);
        alist_remove_at(curr_list, index);
        string_write_c(msgs, "Removed node at index\n");
      }
      else {
        string_write_c(msgs, "Nothing was entered, removing nothing...\n");
      }
    }
    else if (cstring_equals(buffstr, OPTION_REMOVE)) {
      printf("What to remove? ");
      if (fileio_next_line(stdin, buff)) {
        if (alist_remove(curr_list, buff)) {
          string_write_c(msgs, "Sucessfully removed element!\n");
        }
        else {
          string_write_c(msgs, "Could not find element to remove...\n");
        }
      }
      else {
        string_write_c(msgs, "Did not enter anything, not removing...\n");
      }
    }
    else if (cstring_equals(buffstr, OPTION_SWITCH)) {
      char* temp_val = curr_list_val;
      curr_list_val = other_list_val;
      other_list_val = temp_val;
      alist_t* temp = curr_list;
      curr_list = other_list;
      other_list = temp;
      string_write_c(msgs, "Switch lists!\n");
    }
    else if (cstring_equals(buffstr, OPTION_EXPLORE)) {
      string_write_c(msgs, "Not implemented\n");
    }
    else if (cstring_equals(buffstr, OPTION_POP)) {
      printf("What index? ");
      if (fileio_next_line(stdin, buff)) {
        char* str = cstr(buff);
        int index = atoi(str);
        string_write_c(msgs, "Popping at index\n");

        string_t *elem = (string_t *)alist_pop(curr_list, index);
        
        string_write_c(msgs, "Recieved elem \"");
        string_write(msgs, elem);
        string_write_c(msgs, "\" from list\n");

        string_destroy(elem);
      }
    }
    else if (cstring_equals(buffstr, OPTION_COMBINE)) {
      string_write_c(msgs, "Combining lists!\n");
      alist_combine(curr_list, other_list);
    }
  
  }

  return EXIT_SUCCESS;
}



void print_options() {
  printf("%s) Exit\n", OPTION_EXIT);
  printf("%s) Append\n", OPTION_APPEND);
  printf("%s) View\n", OPTION_VIEW);
  printf("%s) Remove At\n", OPTION_REMOVE_AT);
  printf("%s) Remove\n", OPTION_REMOVE);
  printf("%s) Pop\n", OPTION_POP);
  printf("%s) Explore\n", OPTION_EXPLORE);
  printf("%s) switch lists\n", OPTION_SWITCH);
  printf("%s) Combine onto current list\n", OPTION_COMBINE);
}

void print_alist(alist_t* list) {
  printf("\n");

  for (int i = 0; i < list->size; i++) {
    alist_node_t *node =
        array_generic_get(list->list_start, i, list->block_size);

    char* str = cstr((string_t*)&node[ALIST_ELEMENT]);
    printf("%d) %s prev %d next %d\n", i, str, node->prev, node->next);
  }
}