#ifndef CLIB_STD_STRING_H
#define CLIB_STD_STRING_H

#include "./memory.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CSTR_EQUAL 0
#define SPACE_FOR_NULL 1
#define REALLOC_MULTIPLIER 1.5
#define EXTENSION_ASSURANCE 1
#define SPACE_FOR_LOCAL_BOOL 1
#define SHORT_STR_BUF \
  sizeof(size_t) * 8 - sizeof(void *) - (2 * sizeof(int)) - SPACE_FOR_LOCAL_BOOL
#define SHORT_STR_LEN SHORT_STR_BUF - SPACE_FOR_NULL

/*
 * All strings need string_destroy called on them when you're done with them
 */
typedef struct string_t
{
  char *_str;
  size_t len;
  size_t max_len;
  bool local;
  char small[SHORT_STR_BUF];
} string_t;

/*
 * Creates a new string with value 'input'. if input is null, the string
 * will be empty
 */
string_t string_new(char *input);

/*
 * Returns a new empty string
 */
string_t empty_string();

/*
 * Resets a string to empty_string (also frees the string memory)
 */
string_t *string_clear(string_t *to_clear);

/*
 * limits the given string to the new length.
 */
string_t *string_limit(string_t *source, size_t new_len);

/*
 * strcmp() called on both cstring components of the strings given
 * if return value < 0 -> str1 < str2
 * if return value > 0 -> str1 > str2
 * if return value == 0 -> str1 == str2
 */
int string_compare(string_t *str1, string_t *str2);

bool string_equals(string_t *str1, string_t *str2);

// Writes the given char to the end of the string
string_t *string_append_char(string_t *base, char to_add);

/*
 * Appends the cstring to the string 'base'
 */
string_t *string_append_c(string_t *base, char *source);

/*
 * Appends the given cstrings to the string 'base'
 * Last argument must be null
 */
string_t *string_append_c_multi(string_t *base, char *source, ...);

/*
 * Writes the given string source of length 'len' from source to base. If len
 * is greater than strlen(source), only copies strlen(source) chars
 */
string_t *string_append_c_len(string_t *base, char *source, size_t len);

/*
 * Appends the given string 'source' to the end of 'base'
 * Extends the string where necessary
 */
string_t *string_append(string_t *base, string_t *source);

/*
 * Appends to 'base'. Last parameter must be NULL
 */
string_t *string_append_multi(string_t *base, string_t *source, ...);

/*
 * Copies 'source' and returns it
 */
string_t string_copy(string_t *source);

/*
 * returns the concatenation of 'base' and 'extension' without
 * altering either strings
 */
string_t string_concat(string_t *base, string_t *extension);

/*
 * concats the given args into a new string.
 * args must be set to the number of input args
 * Allocates memory
 */
string_t string_concat_multi(string_t *base, string_t *extension, ...);

/*
 * Trim whitespaces and tabs from the ends of the string
 */
string_t *string_trim(string_t *base);

/*
 * Returns a pointer to the start of the cstring component
 * MUST BE USED IN ALL INSTANCES WHERE A CSTR IS REQUIRED
 */
char *cstr(string_t *str);

// murders a string in cold blood
void string_destroy(string_t *str);

/*
 * Same as string_destroy but doesnt throw errors
 * when using for stuff like list_t which takes
 * data_destruction functions with void* as a parameter
 */
void void_string_destroy(void *str);

#endif