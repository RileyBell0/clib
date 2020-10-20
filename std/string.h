
/*
 * The speed or, well, efficiency of these functinos may be improvable
 * through the replacement of the usage of 'string' with a pointer to a string
 * as there is passing of more data than needed. For instance, only the pointer to
 * any string field would be required, as this takes only the sizeof(a pointer) rather
 * than a pointer and two ints, effectively halving the ammount of data. however this would
 * require dealing with pointers and im unsure how dereferencing pointers would act and whether
 * any tangible performance benefit could arise as a result
*/

#ifndef CLIB_STD_STRING_H
#define CLIB_STD_STRING_H

#include "general.h"
#include <string.h>

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
typedef struct string {
    char* str;
    unsigned int len;
    unsigned int max_len;
} string;

/*
 * returns a new empty string with the max length of 'len'
*/
string new_string(unsigned int len);

/*
 * Copies 'source' and returns it
*/
string string_copy(string source);

/*
 * 
*/
void string_write(string* base, string source);

/*
 * Takes a cstring as a source and converts it
 * into a string struct
 * 
 * Does not allocate memory
*/
string string_from_cstring(char* source);

/*
 * Copies 'source' and returns it
*/
char *cstring_copy(const char *source);

/*
 * returns the concatenation of 'base' and 'extension' without
 * altering either strings
*/
string string_new_concat(string base, string extension);


// murders a string in cold blood
void string_destroy(string *toDestroy);
void void_string_destroy(void *toDestroy);

#endif