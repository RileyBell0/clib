/*
 * Author:          Riley Bell
 * Creation Date:   16/10/2020
 */

/*
 * im obsessed with optimisation ok
 *
 * The speed or, well, efficiency of these functinos may be improvable
 * through the replacement of the usage of 'string' with a pointer to a string
 * as there is passing of more data than needed. For instance, only the pointer
 * to any string field would be required, as this takes only the sizeof(a
 * pointer) rather than a pointer and two ints, effectively halving the ammount
 * of data. however this would require dealing with pointers and im unsure how
 * dereferencing pointers would act and whether any tangible performance benefit
 * could arise as a result
 */

/*
 * TO IMPLEMENT
 *      String split
 *          - Array
 *          - List
 *      concat string list
 *          - Array
 *          - List
 *          - Takes a string as an input for what to put inbetween each string
 *      String compare
 *      cstring compare
 *      cstring compare with len
 *      string compare with len
 *
 * need to be able to write a certain number of characters to a string
 * like given a char array write the given number of characters
 */

/*
 * Could do with a const bool
 * which we could chuck into the local bool
 * 1 means local non-const
 * 0 means non-local non-const
 * 2 means const non-local
 * 3 means const and local
 *
 * this would make destroying any string safe
 * so you could chuck const strings into a list
 * and forget about them
 * and it means that reallocing wont hpapen to thiem
 * and yeah
 * safe
 * no destroy
 * might be worth checking or looking into
 * because currently thats a huge downside is that you can chuck any old
 * cstring into here but it makes issues happen when youre not suppoed to edit
 * it so the easiest thing to do would be to remove that option entirely and
 * have cstrings be forced to be copied into here but idk
 *
 * also yeah more method ideads
 */

#ifndef CLIB_STD_STRING_H
#define CLIB_STD_STRING_H

#include "array.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define CSTR_EQUAL 0
#define SPACE_FOR_NULL 1
#define REALLOC_MULTIPLIER 1.5
#define EXTENSION_ASSURANCE 1
#define SPACE_FOR_LOCAL_BOOL 1
#define SHORT_STR_BUF                                                          \
  sizeof(size_t) * 8 - sizeof(void *) - (2 * sizeof(int)) - SPACE_FOR_LOCAL_BOOL
#define SHORT_STR_LEN SHORT_STR_BUF - SPACE_FOR_NULL

typedef struct string_t {
  char *_str;
  unsigned int len;
  unsigned int max_len; // number of characaters able to be stored - 1 (does not
                        // count left for null char)
  bool local;

  char small[SHORT_STR_BUF];
} string_t;

void string_replace(string_t *str, char *pattern, char *replacement);

void string_set_max_len(string_t *str, unsigned int max_len);

/*
 * Removes all data from the string, making its length zero
 */
void string_clear(string_t *to_clear);

/*
 * Returns a new empty string
 */
string_t empty_string();

/*
 * If the capacity of the string is not newLen or greater, extends it to be of
 * said length
 */
void string_allocate(string_t *str, unsigned int newLen);

/*
 * Takes a C string as an input and converts it into a string_t
 * Copies the cstring into the string_t
 * Memory may be allocated when copying
 */
string_t string_make(char *src);

/*
 * takes a const cstring as an input
 * If the given cstring is not dynamically allocated DO NOT FREE
 * THE STRING_T THIS FUNCTION RETURNS
 */
string_t cstring_wrap(char *src);

/*
 * returns a new empty string with the max length of 'len'
 * May allocate memory
 * Remember to destroy with string_destroy or void_string_destroy
 */
string_t string_new(unsigned int len);

/*
 * strcmp() called on both cstring components of the strings given
 * if return value < 0 -> str1 < str2
 * if return value > 0 -> str1 > str2
 * if return value == 0 -> str1 == str2
 */
int string_compare(string_t *str1, string_t *str2);

/*
 * same as string_compare but makes comparing string_t that are
 * seen as void work without causing errors
 */
int string_void_compare(const void *str1, const void *str2);

/*
 * Returns a pointer to the start of the cstring component
 * MUST BE USED IN ALL INSTANCES WHERE A CSTR IS REQUIRED
 */
char *cstr(string_t *str);

/*
 * Copies 'source' and returns it
 */
string_t string_copy(string_t *source);

/*
 * limits the given string to the new length.
 */
void string_limit(string_t *source, unsigned int new_len);

/*
 * Adds 'len' capacity to the string
 */
void string_lengthen(string_t *str, unsigned int len);

/*
 * Returns TRUE if the given strings are equal within the given
 * range, otherwise returns FALSE
 *
 * If you want to compare the strings just use strcmp
 */
bool cstring_equals_range(char *str1, char *str2, int compare_range);
bool cstring_equals(char *str1, char *str2);
bool string_equals(string_t *str1, string_t *str2);

// Null terminates the string
void string_null_terminate(string_t *str);

// Writes the given char to the end of the string
void string_write_char(string_t *base, char to_add);

// /*
//  * Splits a string based on the given delimiting character
//  * returns an array of all split segments (there can be segments
//  * of length 0)
// */
// array_t string_split(string_t source, char delim);

/*
 * Counts the number of times a given character appears within
 * a string
 */
unsigned int string_count_occurances(string_t *source, char delim);

/*
 * Appends the given cstrings to the string 'base'
 * Last argument must be null
 */
string_t *string_write_c_multi(string_t *base, char *source, ...);

/*
 * Writes the given string source of length 'len' from source to base
 * len should be the same as strlen(len) (num_chars not including null
 * termination)
 */
string_t *string_write_c_len(string_t *base, char *source, unsigned int len);

/*
 * Appends the cstring to the string 'base'
 */
string_t *string_write_c(string_t *base, char *source);

/*
 * Appends to 'base'. Last parameter must be NULL
 */
string_t *string_write_multi(string_t *base, string_t *source, ...);
/*
 * Appends the given string 'source' to the end of 'base'
 * Extends the string where necessary
 */
string_t *string_write(string_t *base, string_t *source);

/*
 * Copies the recieved cstring and returns it
 */
char *cstring_copy(const char *source);

/*
 * concats the given args into a new string.
 * args must be set to the number of input args
 * Allocates memory
 */
string_t string_new_concat_multi(string_t *base, string_t *extension, ...);

/*
 * returns the concatenation of 'base' and 'extension' without
 * altering either strings
 */
string_t string_new_concat(string_t *base, string_t *extension);

/*
 * Concatenates the given strings
 * args must be set to the number of input args
 */
string_t *string_concat_multi(string_t *base, string_t *extension, ...);

string_t *string_concat(string_t *base, string_t *extension);

// Realloc's the given string, updating its length
void string_extend(string_t *str);

/*
 * If the string is longer than the given length, limit it
 * to the requried length
 */
void string_limit(string_t *str, unsigned int len);

// murders a string in cold blood
void string_destroy(string_t *str);

/*
 * Same as string_destroy but doesnt throw errors
 * when using for stuff like list_t which takes
 * data_destruction functions with void* as a parameter
 *
 * Primarily, this just quietens compiler warnings
 */
void void_string_destroy(void *str);

#endif