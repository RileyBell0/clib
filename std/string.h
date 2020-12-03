/*
 * Author:          Riley Bell
 * Creation Date:   16/10/2020
*/

/*
 * im obsessed with optimisation ok
 * 
 * The speed or, well, efficiency of these functinos may be improvable
 * through the replacement of the usage of 'string' with a pointer to a string
 * as there is passing of more data than needed. For instance, only the pointer to
 * any string field would be required, as this takes only the sizeof(a pointer) rather
 * than a pointer and two ints, effectively halving the ammount of data. however this would
 * require dealing with pointers and im unsure how dereferencing pointers would act and whether
 * any tangible performance benefit could arise as a result
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
*/

#ifndef CLIB_STD_STRING_H
#define CLIB_STD_STRING_H

#include <string.h>
#include <stdarg.h>
#include "general.h"
#include "array.h"

#define SPACE_FOR_NULL 1
#define REALLOC_MULTIPLIER 1.5
#define EXTENSION_ASSURANCE 1

/*
 * Dealing with strings, do the same thing youd normally do, pass a pointer
 * if you want to keep track of len and max_len
 * 
 * note that you can always re-affirm the length of the string but not the max
 * length, so if in a pinch, duplicate the string and free the old one. the
 * max length will now be the length of the string
 * 
 * just keep in mind that all below methods ASSUME that len is correct
 * if it isnt you need to update it using the inbuilt strlen function
*/
typedef struct string_t
{
    char *str;
    unsigned int len;
    unsigned int max_len;
} string_t;

/*
 * returns a new empty string with the max length of 'len'
*/
string_t new_string(unsigned int len);

/*
 * Copies 'source' and returns it
*/
string_t string_copy(string_t source);

/*
 * Returns TRUE if the given strings are equal within the given
 * range, otherwise returns FALSE
 * 
 * If you want to compare the strings just use strcmp
*/
int cstring_equals_range(char* str1, char* str2, int compareRange);
int cstring_equals(char* str1, char* str2);
int string_equals(string_t str1, string_t str2);

// Null terminates the string
void string_null_terminate(string_t* str);

// Writes the given char to the end of the string
void string_write_char(string_t *base, char toAdd);

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
unsigned int string_count_occurances(string_t source, char delim);

/*
 * Appends the given cstrings to the string 'base'
 * Last argument must be null
*/
string_t* string_write_c(string_t *base, char* source, ...);
/*
 * Appends the cstring to the string 'base'
*/
string_t* string_write_c_single(string_t *base, char* source);

/*
 * Appends to 'base'. Last parameter must be NULL
*/
string_t* string_write(string_t *base, string_t* source, ...);
/*
 * Appends the given string 'source' to the end of 'base'
 * Extends the string where necessary
*/
string_t *string_write_single(string_t *base, string_t source);

/*
 * Takes a cstring as a source and converts it
 * into a string struct
 * 
 * Does not allocate memory
*/
string_t string_from_cstring(char *source);

/*
 * Copies the recieved cstring and returns it
*/
char *cstring_copy(const char *source);

/*
 * concats the given args into a new string.
 * args must be set to the number of input args
 * Allocates memory
*/
string_t string_new_concat_multi(string_t base, string_t *extension, ...);

/*
 * returns the concatenation of 'base' and 'extension' without
 * altering either strings
*/
string_t string_new_concat(string_t base, string_t extension);

/*
 * Concatenates the given strings
 * args must be set to the number of input args
*/
string_t string_concat_multi(string_t base, string_t *extension, ...);

string_t string_concat(string_t base, string_t extension);

// Realloc's the given string, updating its length
void string_extend(string_t *toExtend);

// murders a string in cold blood
void string_destroy(string_t *toDestroy);

/*
 * Same as string_destroy but doesnt throw errors
 * when using for stuff like list_t which takes
 * data_destruction functions with void* as a parameter
 * 
 * Primarily, this just quietens compiler warnings
*/
void void_string_destroy(void *toDestroy);


#endif