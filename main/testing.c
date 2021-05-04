/*
 * There is no purpose of this file other than to test the functionality of
 * the clib files
 */

#include "../std/array.h"
#include "../std/configLoader.h"
#include "../std/dir/directory.h"
#include "../std/alist.h"
#include "../std/string.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define END_SUCCESS 0
#define MULTIPLE 1
#define APPENDTEST 50000
#define TESTING_C_TIME_PROGRAM TRUE

#define EXIT '0'
#define APPEND '1'
#define REMOVE '2'
#define PRINT_ARRAY '3'
#define TRAVERSE '4'
#define LIST_INFO '5'
#define LIST_INDEX '6'
#define DISPLAY '7'

int code2(int argc, char **argv);
int code3(int argc, char** argv);
int code(int argc, char **argv);
void bar(char *toPrint);
void alist_test();
void list_test();
void strinfo(string_t info);
void test_alist_append(void *ptr, void *data);
void test_list_append(void *ptr, void *data);
void append_test(void *list, void (*append)(void *ptr, void *data));
void arr_append_test();
void clearscreen();
void print_string_alist(alist_t *list);

#define STR_TEST 48

/*
 * okay so it looks to me like an alist is about 0.5-0.75x faster than using a
 * normal list but it seems to be 1/3 slower than using just a normal dynamic
 * array
 * -> im assuming this is because of the patching
 */
// Write your code here

int code3(int argc, char** argv) {
  printf("Code 3\n");
  return 0;
}

// TODO remove
void printline(const char *line) { printf("%s\n", line); }

// For formatting program output
void bar(char *toPrint) {
  int len = strlen(toPrint);
  for (int i = 0; i < len + 1; i++) {
    printf("-");
  }
  printf("\n");
  printf("%s\n", toPrint);
  for (int i = 0; i < len + 1; i++) {
    printf("-");
  }
  printf("\n");
}

// Entrypoint of the program. Prints headers before and after entering the
// user's code
int main(int argc, char **argv) {
  bar("Program Start");
  clock_t start_t, end_t;

  if (TESTING_C_TIME_PROGRAM == TRUE) {
    start_t = clock();
    printf("Starting of the program, start_t = %ld\n", start_t);
  }

  int errorCode = code3(argc, argv);

  if (TESTING_C_TIME_PROGRAM == TRUE) {
    end_t = clock();
    printf("End time end_t = %ld\n", end_t);
    printf("Difference: %ld\n", end_t - start_t);
    double timetaken = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("Total time taken by CPU: %lf\n", timetaken);
    printf("Exiting of the program...\n");
  }

  bar("Program End");
  return errorCode;
}










/*
void clearscreen() {
  for (int i = 0; i < 3; i++) {
    printf("\n");
  }
}

void traverse_string_alist(alist_t *list) {
  if (!list->size) {
    return;
  }
  
  string_t buffer = new_string(DEFAULT_BUFFER_LEN);

  int32_t curr = list->first;
  int32_t pos = 1;

  while (TRUE) {
    alist_node_t *node =
        array_get_element(list->list_start, curr, list->block_size);

    printf("CURRENT NODE: index: %u, str: %s, node: %d\n", curr,
           cstr((string_t *)&node[ALIST_ELEMENT]), pos);
    printf("%d) \tnext: %12d\tprev %12d\n", curr, node->next, node->prev);
    fileio_next_line(stdin, &buffer);

    if (cstring_equals(cstr(&buffer), "b")) {
      if (node->prev != ALIST_NULL) {
        printf("GOING BACK %d\n", node->prev);
        curr += node->prev;
        --pos;
      } else {
        printf("AT START OF LIST\n");
      }
    } else if (cstring_equals(cstr(&buffer), "n")) {
      if (node->next != ALIST_NULL) {
        printf("GOING FORWARD %d\n", node->next);
        curr += node->next;
        ++pos;
      } else {
        printf("AT END OF LIST\n");
      }

    } else if (cstring_equals(cstr(&buffer), "exit")) {
      break;
    }
  }
}

void print_string_alist(alist_t *list) {
  if (list->first == ALIST_NULL) {
    return;
  }

  printf("First: %u\n", list->first);
  printf("\n");
  int32_t curr = list->first;

  while (curr >= 0) {
    alist_node_t *node =
        array_get_element(list->list_start, curr, list->block_size);

    printf("%d) \tnext: %12d\tprev %12d", curr, node->next, node->prev);

    strinfo(*((string_t *)(&node[ALIST_ELEMENT])));

    curr += node->next;
  }
  printf("endof list\n");
}

int code(int argc, char **argv) {
  char pause = FALSE;
  alist_t list = new_alist(sizeof(string_t));
  string_t str = string_make("1");
  alist_append(&list, &str);
  str = string_make("2");
  alist_append(&list, &str);
  str = string_make("3");
  alist_append(&list, &str);
  str = string_make("4");
  alist_append(&list, &str);
  str = string_make("5");
  alist_append(&list, &str);

  list.compare = string_void_compare;

  char input;
  string_t buffer = new_string(DEFAULT_BUFFER_LEN);

  while (TRUE) {
    clearscreen();
    printf("%c) EXIT\n"
           "%c) Append\n"
           "%c) Remove\n"
           "%c) Print Array\n"
           "%c) Traverse\n"
           "%c) List Info\n"
           "%c) Index\n"
           "%c) Display\n",
           EXIT, APPEND, REMOVE, PRINT_ARRAY, TRAVERSE, LIST_INFO, LIST_INDEX,
            DISPLAY);

    if (fileio_next_line(stdin, &buffer) && buffer.len == 1) {
      input = cstr(&buffer)[0];
      switch (input) {
      case APPEND:
        printf("Enter what you would like to append:\n");
        if (fileio_next_line(stdin, &buffer)) {
          printf("APPENDING \"%s\"\n\n", cstr(&buffer));
          string_t input = string_copy(&buffer);
          alist_append(&list, &input);
        }
        break;
      case REMOVE:
        printf("What would you like to remove?\n");
        if (fileio_next_line(stdin, &buffer)) {
          if (alist_remove(&list, &buffer)) {
            printf("REMOVED NODE\n");
          } else {
            printf("COULD NOT FIND OR REMOVE\n");
          }
        }
        break;
      case DISPLAY:
        printf(" first: %u\n Last: %u\n capacity: %u\n size: %u\n\n",
               list.first, list.last, list.capacity, list.size);
        print_string_alist(&list);
        pause = TRUE;
        break;
      case TRAVERSE:
        traverse_string_alist(&list);
        break;
      case LIST_INFO:
        printf("List Information:\n\tCAPACITY:\n\t\t%u\n\tSIZE:\n\t\t%u\n",
               list.capacity, list.size);
        print_string_alist(&list);
        pause = TRUE;
        break;
      case LIST_INDEX:
        printf("What index?\n");
        if (fileio_next_line(stdin, &buffer)) {
          int32_t index = atoi(cstr(&buffer));
          string_t *element = alist_index(&list, index);
          printf("element at index %d: %s\n", index, cstr(element));
        }
        pause = TRUE;
        break;
      case PRINT_ARRAY:
        printf("\nArray Contents:\n");
        for (int32_t i = 0; i < list.size; i++) {
          printf("%d) %s\n", i, cstr(((string_t *)alist_index(&list, i))));
        }
        pause = TRUE;
        break;
      case EXIT:
        return EXIT_SUCCESS;
      default:
        printf("INVALID INPUT\n");
        break;
      }
    } else {
      printf("\nError - invalid input\n\n");
    }

    // if (pause){
    //   printf("\nEnter anything to continue... ");
    //   fileio_next_line(stdin, &buffer);
    //   pause = FALSE;
    // }
  }

  return END_SUCCESS;
}

void strinfo(string_t info) {
  // printf("\tlen: %d\n\tmax: %d\n\tstr: %s\n", info.len, info.max_len,
  //        cstr(&info));
  printf("\tstr: %s\n", cstr(&info));
}













#define B_SELECT_LIST1 '1'
#define B_SELECT_LIST2 '2'
#define B_APPEND_LIST '4'
#define B_REMOVE_FROM_LIST '5'
#define B_COMBINE_LISTS '6'
#define B_RESET_LIST '7'

void print_options_code2(){
  printf("------------------\n");
  printf("%c) Select List 1:\n", B_SELECT_LIST1);
  printf("%c) Select List 2:\n", B_SELECT_LIST2);
  printf("%c) Print current list:\n", B_SELECT_LIST2);
  printf("%c) Append to list\n", B_APPEND_LIST);
  printf("%c) Remove from current list\n", B_REMOVE_FROM_LIST);
  printf("%c) Combine lists\n", B_COMBINE_LISTS);
  printf("%c) Reset List\n", B_RESET_LIST);
}

alist_t make_single_char_str_list(char* str_in) {
  alist_t list = new_alist(sizeof(string_t));
  string_t str = string_make(str_in);

  for (int i = 0; i < str.len; i++) {
    string_t new_element = new_string(0);
    string_write_char(&new_element, str_in[i]);
    alist_append(&list, &new_element);
  }

  string_destroy(&str);
  
  return list;
}















int code2(int argc, char **argv){
  // string_t path = string_make(".");
  // string_t extension = string_make("c");
  // // printf("Getting files with extension \"%s\" - recursive\n", cstr(&extension));
  // printf("Getting files - recursive\n");
  // alist_t entries = dir_all_files_recur(&path);
  // // alist_t entries = dir_files_with_extension_recur(&path, &extension);

  // printf("\n\n");
  // printf("RECURSIVE ENTRIES: %d\n", entries.size);
  // alist_iterator_t it = new_alist_iterator(&entries, TRUE);

  // for (string_t* element = it.first(&it); !it.done(&it); element = it.next(&it)) {
  //   printf("\t%s\n", cstr(element));
  // }  
  // printf("\n\n");
  // return 0;


  alist_t list1 = make_single_char_str_list("0123456789");
  alist_t list2 = make_single_char_str_list("abcdefghij");
  string_t nullstr = string_make("NULL");

  list1.compare = string_void_compare;
  list2.compare = string_void_compare;

  char input;
  string_t buffer = new_string(DEFAULT_BUFFER_LEN);
  alist_t* curr_list = &list1;
  int32_t list_num = 1;
  while (TRUE) {
    clearscreen();
    printf("Current List: %d\n", list_num);
    print_options_code2();

    if (fileio_next_line(stdin, &buffer) && buffer.len == 1) {
      input = cstr(&buffer)[0];
      switch (input) {
      case EXIT:
        return EXIT_SUCCESS;
      case B_SELECT_LIST1:
        curr_list = &list1;
        list_num = 1;
        break;
      case B_SELECT_LIST2:
        curr_list = &list2;
        list_num = 2;
        break;
      case PRINT_ARRAY:
        printf("List information\nsize: %d\n", curr_list->size);
        printf("\nArray Contents:\n");
        alist_iterator_t iterator = new_alist_iterator(curr_list, TRUE);
        for (string_t *element = (string_t *)iterator.first(&iterator);
             !iterator.done(&iterator); element = iterator.next(&iterator)) {
          printf("%03d| %s  ", iterator.index, cstr(element));
          printf("prev ");
          if (iterator.curr_node->prev == ALIST_NULL) {
            printf("%11s", cstr(&nullstr));
          } else {
            printf("%11d", iterator.curr_node->prev);
          }
          printf(" next ");
          if (iterator.curr_node->next == ALIST_NULL) {
            printf("%11s", cstr(&nullstr));
          }
          else{
            printf("%11d", iterator.curr_node->next);
          }
          printf("\n");
        }
        break;
      case B_APPEND_LIST:
        printf("Enter what you would like to append:\n");
        if (fileio_next_line(stdin, &buffer)) {
          printf("APPENDING \"%s\"\n\n", cstr(&buffer));
          string_t input = string_copy(&buffer);
          alist_append(curr_list, &input);
        }
        break;
      case B_REMOVE_FROM_LIST:
        printf("What would you like to remove?\n");
        if (fileio_next_line(stdin, &buffer)) {
          if (alist_remove(curr_list, &buffer)) {
            printf("REMOVED NODE\n");
          } else {
            printf("COULD NOT FIND OR REMOVE\n");
          }
        }
        break;
      case B_COMBINE_LISTS:
        alist_combine(&list1, &list2);
        break;
      case B_RESET_LIST:
        curr_list->size = 0;
        curr_list->first = ALIST_NULL;
        curr_list->last = ALIST_NULL;
        break;
      default:
        printf("INVALID INPUT\n");
        break;
      }
    } else {
      printf("\nError - invalid input\n\n");
    }

    // if (pause){
    //   printf("\nEnter anything to continue... ");
    //   fileio_next_line(stdin, &buffer);
    //   pause = FALSE;
    // }
  }

  return END_SUCCESS;
}
*/