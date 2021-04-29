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
#include <stdint.h>
#include "array.h"

#define SPACE_FOR_NULL 1
#define REALLOC_MULTIPLIER 1.5
#define EXTENSION_ASSURANCE 1
#define SPACE_FOR_LOCAL_BOOL 1
#define SHORT_STR_BUF __WORDSIZE - sizeof(void*) - (2 * sizeof(int32_t)) - SPACE_FOR_LOCAL_BOOL
#define SHORT_STR_LEN SHORT_STR_BUF-SPACE_FOR_NULL
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
    char *_str;
    uint32_t len;
    uint32_t max_len; // number of characaters able to be stored - 1 (does not count left for null char)
    // boolean value
    char local;
    char small[SHORT_STR_BUF];
} string_t;

/*
 * Okay so the idea for short string optimisation
 * 
 * we can make sure the string pointer is always pointing at the right place by calling cstr
 * but the problem arises once we look at string_from_cstring
 * 
 * the problem is: if we make a string which is a reference to a c string then some weird schenanigans can happen
 * usually when we make a cstring itno a string we kinda just store the start of the string
 * but i say what if we just treat it like a normal string? 
 * yeah that means thers going to be memory duplicatoin but whatever am i rite?
 * so like what canwe do we just send it in to the string?
 * we make a new string? what if we redefine how strings work, we dont ahve a string form cstring which would mean that any constant values we want to store as strings have to be loaded in
 * thats not that big of a deal in terms of performmae and istnte really an issue
*/


/*
 * Removes all data from the string, making its length zero
*/
void string_clear(string_t* to_clear);

/*
 * Returns a new empty string
*/
string_t empty_string();

/*
 * If the capacity of the string is not newLen or greater, extends it to be of said length
*/
void string_allocate(string_t* str, uint32_t newLen);

/*
 * Takes a C string as an input and converts it into a string_t
 * Copies the cstring into the string_t
 * Memory may be allocated when copying
*/
string_t string_make(char* src);

/*
 * takes a const cstring as an input
 * If the given cstring is not dynamically allocated DO NOT FREE
 * THE STRING_T THIS FUNCTION RETURNS
*/
string_t cstring_wrap(char* src);

/*
 * Makes the string point at a preexisting C string, make sure the string it points to is mutable and not
 * referenced elsewhere as it will now be managed by string_t
*/
string_t* string_set(string_t *str, char* src);

/*
 * returns a new empty string with the max length of 'len'
*/
string_t new_string(uint32_t len);

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
int string_void_compare(const void* str1, const void* str2);

/*
 * Returns a pointer to the start of the cstring component
 * MUST BE USED IN ALL INSTANCES WHERE A CSTR IS REQUIRED
*/
char* cstr(string_t* str);

/*
 * Copies 'source' and returns it
*/
string_t string_copy(string_t *source);

/*
 * Shrinks the given string to the new length.
*/
void string_shrink(string_t* source, uint32_t new_len);

/*
 * Adds 'len' capacity to the string
*/
void string_lengthen(string_t *str, uint32_t len);

/*
 * Returns TRUE if the given strings are equal within the given
 * range, otherwise returns FALSE
 * 
 * If you want to compare the strings just use strcmp
*/
int cstring_equals_range(char* str1, char* str2, int compare_range);
int cstring_equals(char* str1, char* str2);
int string_equals(string_t *str1, string_t *str2);

// Null terminates the string
void string_null_terminate(string_t* str);

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
uint32_t string_count_occurances(string_t* source, char delim);

/*
 * Appends the given cstrings to the string 'base'
 * Last argument must be null
*/
string_t* string_write_c_multi(string_t *base, char* source, ...);
/*
 * Appends the cstring to the string 'base'
*/
string_t* string_write_c(string_t *base, char* source);

/*
 * Appends to 'base'. Last parameter must be NULL
*/
string_t* string_write_multi(string_t *base, string_t* source, ...);
/*
 * Appends the given string 'source' to the end of 'base'
 * Extends the string where necessary
*/
string_t *string_write(string_t *base, string_t* source);

/*
 * Copies the recieved cstring and returns it
*/
char *cstring_copy(const char *source);

/*
 * concats the given args into a new string.
 * args must be set to the number of input args
 * Allocates memory
*/
string_t string_new_concat_multi(string_t* base, string_t *extension, ...);

/*
 * returns the concatenation of 'base' and 'extension' without
 * altering either strings
*/
string_t string_new_concat(string_t *base, string_t *extension);

/*
 * Concatenates the given strings
 * args must be set to the number of input args
*/
string_t* string_concat_multi(string_t *base, string_t *extension, ...);

string_t* string_concat(string_t *base, string_t *extension);

// Realloc's the given string, updating its length
void string_extend(string_t *str);

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