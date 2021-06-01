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

int code(int argc, char** argv) {
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