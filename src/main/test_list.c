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
#define OPTION_INFO "7"
#define OPTION_SWITCH "SWITCH"

void print_alist(alist_t* list);

void print_options();

int main(int argc, char **argv) {
  string_t buffer_str = new_string(DEFAULT_BUFFER_LEN);
  string_t *buff = &buffer_str;

  alist_t list = new_alist(sizeof(string_t));
  list.destroy = void_string_destroy;
  list.compare = string_void_compare;
  string_t str;

  str = string_make("0");
  alist_append(&list, &str);
  str = string_make("1");
  alist_append(&list, &str);
  str = string_make("2");
  alist_append(&list, &str);
  str = string_make("3");
  alist_append(&list, &str);
  str = string_make("4");
  alist_append(&list, &str);
  str = string_make("5");
  alist_append(&list, &str);
  str = string_make("6");
  alist_append(&list, &str);
  str = string_make("7");
  alist_append(&list, &str);
  str = string_make("8");
  alist_append(&list, &str);
  str = string_make("9");
  alist_append(&list, &str);
  alist_t list2 = new_alist(sizeof(string_t));
  list2.destroy = void_string_destroy;
  list2.compare = string_void_compare;

  alist_t* curr_list = &list;
  alist_t* other_list = &list2;

  char* curr_list_val = "1";
  char* other_list_val = "2";

  while (true) {
    print_options();
    printf("Enter your selection: ");
    if (!fileio_next_line(stdin, buff)) {
      printf("Error - You must choose something..\n");
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
        printf("Appended \"%s\"!\n", cstr(&to_append));
      }
      else {
        printf("Error - Nothing appended, no input recieved\n");
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
        printf("Removed node at %d\n", index);
      }
      else {
        printf("Nothing was entered, removing nothing...\n");
      }
    }
    else if (cstring_equals(buffstr, OPTION_REMOVE)) {
      printf("What to remove? ");
      if (fileio_next_line(stdin, buff)) {
        if (alist_remove(curr_list, buff)) {
          printf("Sucessfully removed element!\n");
        }
        else {
          printf("Could not find element to remove...\n");
        }
      }
      else {
        printf("Did not enter anything, not removing...\n");
      }
    }
    else if (cstring_equals(buffstr, OPTION_SWITCH)) {
      char* temp_val = curr_list_val;
      curr_list_val = other_list_val;
      other_list_val = temp_val;
      alist_t* temp = curr_list;
      curr_list = other_list;
      other_list = temp;
      printf("Switched!\n");
    }
    else if (cstring_equals(buffstr, OPTION_EXPLORE)) {
      printf("Not implemented\n");
    }
    else if (cstring_equals(buffstr, OPTION_POP)) {
      printf("What index? ");
      if (fileio_next_line(stdin, buff)) {
        char* str = cstr(buff);
        int index = atoi(str);
        printf("Popping at %d\n", index);

        string_t *elem = (string_t *)alist_pop(curr_list, index);
        
        printf("Recieved elem \"%s\" from list\n", cstr(elem));

        string_destroy(elem);
      }
    }
    else if (cstring_equals(buffstr, OPTION_INFO)) {
      printf("List Size: %d\nList Capacity: %d\n", curr_list->size, curr_list->capacity);
    }
  
  }

  return EXIT_SUCCESS;
}



void print_options() {
  printf("0) Exit\n");
  printf("1) Append\n");
  printf("2) View\n");
  printf("3) Remove At\n");
  printf("4) Remove\n");
  printf("5) Pop\n");
  printf("6) Explore\n");
  printf("7) Info\n");
  printf("SWITCH) switch lists\n");
}

void print_alist(alist_t* list) {
  // alist_iterator_t it = new_alist_iterator(list, true);
  // printf("List:\n");
  // for (it.first(&it); !it.done(&it); it.next(&it)) {
  //   char* elem = cstr((string_t*)it.element);
  //   printf("%d)  \"%s\" prev %d  next %d\n", it.index, elem, it.curr_node->prev, it.curr_node->next);
  // }
  // printf("\n");

  // it = new_alist_iterator(list, false);
  // printf("List:\n");
  // for (it.first(&it); !it.done(&it); it.next(&it)) {
    
  //   char* elem = cstr((string_t*)it.element);
  //   printf("%d)  \"%s\" prev %d  next %d\n", it.index, elem, it.curr_node->prev, it.curr_node->next);
  // }
  printf("\n");

  for (int i = 0; i < list->size; i++) {
    alist_node_t* node = array_get_element(list->list_start, i, list->block_size);

    char* str = cstr((string_t*)&node[ALIST_ELEMENT]);
    printf("%d) %s prev %d next %d\n", i, str, node->prev, node->next);
  }
}