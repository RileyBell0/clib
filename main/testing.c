/*
 * There is no purpose of this file other than to test the functionality of
 * the clib files
 */

#include "../std/array.h"
#include "../std/configLoader.h"
#include "../std/directory.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define END_SUCCESS 0
#define MULTIPLE 1
#define APPENDTEST 50000
#define TESTING_C_TIME_PROGRAM TRUE

#define EXIT 'e'
#define APPEND 'a'
#define REMOVE 'r'
#define DISPLAY 'd'
#define TRAVERSE 't'
#define LIST_INFO 'o'
#define LIST_INDEX 'i'
#define LEDGER_INFO 'l'
#define PRINT_ARRAY 'p'

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

// TODO remove
void printline(const char *line) { printf("%s\n", line); }

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
  for (int x = 0; x < list->size; x++) {
    printf("%d ", list->ledger[x]);
  }
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
    // printf("%c) EXIT\n"
    //        "%c) Append\n"
    //        "%c) Remove\n"
    //        "%c) Display\n"
    //        "%c) Traverse\n"
    //        "%c) List Info\n"
    //        "%c) Index\n"
    //        "%c) Ledger Info\n"
    //        "%c) Print Array\n",
    //        EXIT, APPEND, REMOVE, DISPLAY, TRAVERSE, LIST_INFO, LIST_INDEX,
    //        LEDGER_INFO, PRINT_ARRAY);

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
      case LEDGER_INFO:
        // TODO remove
        printf("Ledger Information:\nLedger:\n");
        for (int32_t i = 0; i < list.size; i++) {
          printf("%d ", list.ledger[i]);
        }
        printf("\nComplement:\n");
        for (int32_t i = 0; i < list.size; i++) {
          printf("%d ", list.ledger_complement[i]);
        }
        printf("\n");
        break;
      case PRINT_ARRAY:
        printf("\nArray Contents:\n");
        for (int32_t i = 0; i < list.size; i++){
          printf("%d) %s\n", i, cstr(((string_t*)alist_index(&list, i))));
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

  int errorCode = code(argc, argv);

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
