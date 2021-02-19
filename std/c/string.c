#include "../string.h"


void string_shrink(string_t* source, unsigned int new_len)
{
    cstr(source)[new_len] = '\0';
    source->len = new_len;
}

// okay what if we have a short/long bit?
    // how can we do this?
    // this means that we have to fuck with the max/len 
    // okay what if instead we have a short/long char
    // not as fast i know but its a work in progress, we can always update it later
    // so there is going to be a short/long char

    /*
     * this has alot of implicatoins
     * like what if instead of short/long we have an allocated char
     * well hers the thing do we want to deal iwth constant strings?
     * what if we just copy them in?
     * thats easier right?
     * but is if twaster?
     * no
     * what if we 
     * idk'
     * we cnat have references wto other strings
     * ALL DYNAMICALLY ALOCATED STRINGS SHOULD WORK JUST FINE
     * except if they're less than the small string ammout
     * in which case we need a allocated bit
     * and if athe bit is set to true we know that the string is dynamically alloated
     * if it is set to false we know that we're dealong with a small string
     * which means that we cant have cstrings 
     * which means we have to copy in any constant strings 
     * which means that all strings need to be dynamically allocatable
     * which means a string_T cant store a constant if its ever going to be modified
     * but you dont modify constants anyway
     * so that should be fine
     * it should always 
     * wait no because we have the cstring method
     * 
     * so what if we have a constant?
     * 
     * what if instaed of allocated we have 'local'
     * 
     * which means its stored locally or externally
     * which means short
     * so we need a short bit
     * or a local bit
     * i think local makes mroe sense
     * 
     * so if we have alocal bit we can use that for any string whih were fuckng with
     * but if we have a dynamically allocated or external string we just set tha tbit to false
     * and it leaves it be
     * so this means that if we have it set to false, then we know that the 
    */

/*
 * so if we're making a new string we can give it NULL if we dont want it to copy anything
 * and in this case it's going to be a small string
*/
string_t new_string(unsigned int len)
{
    string_t str;

    str.len = 0;

    // Small string optimisation
    if (len <= SHORT_STR_LEN)
    {
        str.max_len = SHORT_STR_LEN;
        str._str = str.small;
        str.local = TRUE;
    }
    else
    {
        str.max_len = len;
        str._str = safe_malloc(len * sizeof(char) + SPACE_FOR_NULL);
        str.local = FALSE;
    }

    str._str[0] = '\0';

    return str;
}

string_t string_make(char* src)
{
    string_t str;

    str._str = src;
    str.local = FALSE;

    if (src)
    {
        str.len = strlen(src); 
    }
    else
    {
        str.len = 0;
    }

    str.max_len = str.len;

    return str;
}

string_t* string_set(string_t* str, char* src)
{
    str->_str = src;
    str->local = FALSE;

    if (src)
    {
        str->len = strlen(src); 
    }
    else
    {
        str->len = 0;
    }
    str->max_len = str->len;

    return str;
}

int string_void_compare(const void* str1, const void* str2)
{
    return string_compare((string_t*)str1, (string_t*)str2);
}

int string_compare(string_t *str1, string_t *str2)
{
    return strcmp(cstr(str1), cstr(str2));
}

int string_equals(string_t *str1, string_t *str2)
{
    if (str1->len != str2->len)
    {
        return FALSE;
    }
    
    return cstring_equals(cstr(str1), cstr(str2));
}

int cstring_equals(char* str1, char* str2)
{
    int i = 0;
    while (TRUE)
    {
        if (str1[i] != str2[i])
        {
            return FALSE;
        }
        else if (str1[i] == str2[i] && str1[i] == '\0')
        {
            return TRUE;
        }

        ++i;
    }
    return TRUE;
}

int cstring_equals_range(char* str1, char* str2, int compareRange)
{
    for (int i = 0; i < compareRange; i++)
    {
        if (str1[i] != str2[i] || str1[i] == '\0')
        {
            return FALSE;
        }
    }

    return TRUE;
}

unsigned int string_count_occurances(string_t* source, char delim)
{
    unsigned int occurances = 0;
    char* strstart = cstr(source);
    for (int i = 0; i < source->len; i++)
    {
        if (strstart[i] == delim)
        {
            ++occurances;
        }
    }
    return occurances;
}

void string_null_terminate(string_t* str)
{
    if (!str || !str->_str)
    {
        return;
    }

    cstr(str)[str->len] = '\0';
}

void string_write_char(string_t *base, char toAdd)
{
    if (!base || !base->_str)
    {
        return;
    }
    // Extend the string if too small
    if (base->max_len <= base->len)
    {
        string_extend(base);
    }
    
    // Write the char
    cstr(base)[base->len] = toAdd;
    base->len += 1;
}

string_t* string_write_c_multi(string_t *base, char* source, ...)
{
    va_list vargs;
    va_start(vargs, source);

    char *str = source;

    while (str)
    {
        string_write_c(base, str);
        str = va_arg(vargs, char*);
    }

    va_end(vargs);

    return base;
}

string_t* string_write_c(string_t *base, char* source)
{
    if (!base || !base->_str || !source)
    {
        return base;
    }
    /*
     * Goes over all chars in 'source'
     * then goes over all chars in 'source' adding it to the str
     * 
     * or 
     * goes over all chars in 'source', extending the str where necessary
     * TODO implement this method, will make the strings faster
    */
    string_t extension = string_make(source);
    
    string_write(base, &extension);

    return base;
}

string_t* string_write_multi(string_t *base, string_t* source, ...)
{
    va_list vargs;
    va_start(vargs, source);

    string_t *str = source;
    while (str)
    {
        string_write(base, str);
        str = va_arg(vargs, string_t*);
    }

    va_end(vargs);

    return base;
}

string_t* string_write(string_t *base, string_t *source)
{
    // If there isnt enough room to fit the extension
    if (base->max_len < base->len + source->len)
    {
        int newLen = base->max_len * REALLOC_MULTIPLIER;
        // If the new string is going to need more space than a normal extension
        if (newLen < base->len + source->len)
        {
            string_lengthen(base, base->len + source->len - base->max_len);
        }
        else
        {
            string_extend(base);
        }
    }

    int sourcePos = 0;
    unsigned int startLen = base->len;
    unsigned int finalLen = startLen + source->len;
    char* basestart = cstr(base);
    char* srcstart = cstr(source);
    for (unsigned int i = startLen; i < finalLen; i++)
    {
        basestart[i] = srcstart[sourcePos++];
        base->len += 1;
    }
    basestart[base->len] = '\0';

    return base;
}

void string_extend(string_t *toExtend)
{
    int newLen = REALLOC_MULTIPLIER * (toExtend->max_len + SPACE_FOR_NULL);

    // If the current string is a short string
    if (toExtend->local)
    {
        // Allocate space for the extension and copy over the
        // short-string
        toExtend->local = FALSE;
        toExtend->_str = safe_malloc(newLen);
        memcpy(toExtend->_str, toExtend->small, SHORT_STR_BUF);
    }
    else
    {
        // Increasing the length of the string and allocating space
        // Still need space for null termination
        toExtend->_str = safe_realloc(toExtend->_str, newLen * sizeof(char));
    }

    // Updating the length of the string
    // it's -1 due to needing to leave space for the null char
    toExtend->max_len = newLen-SPACE_FOR_NULL;
}

void string_allocate(string_t* str, unsigned int newLen)
{
    if (newLen > str->max_len)
    {
        string_lengthen(str, newLen-str->max_len);
    }
}

void string_lengthen(string_t *toExtend, unsigned int len)
{
    int newLen = toExtend->max_len + len + SPACE_FOR_NULL;

    // If the current string is a short string
    if (toExtend->local)
    {
        // Allocate space for the extension and copy over the
        // short-string
        toExtend->local = FALSE;
        toExtend->_str = safe_malloc(newLen);
        memcpy(toExtend->_str, toExtend->small, SHORT_STR_BUF);
    }
    else
    {
        // Increasing the length of the string and allocating space
        // Still need space for null termination
        toExtend->_str = safe_realloc(toExtend->_str, newLen * sizeof(char));
    }

    // Updating the length of the string
    toExtend->max_len = newLen - 1;
}

string_t string_new_concat_multi(string_t* base, string_t *extension, ...)
{
    va_list vargs;
    va_start(vargs, extension);

    string_t copy = string_copy(base);

    string_t* str = extension;

    while (str)
    {
        string_write(&copy, str);
        str = va_arg(vargs, string_t*);
    }
    /*
     * If were going to move strings at any point they cant be in complex data structures
     * so what if we just malloc the strings
     * the idea is that we dont have to put thenm on the heap each time adn tehy're not
     * like off in some random spot in memory and that must be wy faster except the problem is that this type of string
     * doesnt work with anything
     * okay so what if we make it check and update each time? 
     * what if every method checks the max length of the string, and if it's equal to the constant value it makes the str point to the positoin
     * as we cant use strings without the string methods anyway this hsould be fine
     * unless
     * okay
     * unless
     * okay
     * so we need a cstr method
     * this method will just return a pointer to the start of the string
    */

    va_end(vargs);

    return copy;
}

string_t string_new_concat(string_t* base, string_t* extension)
{
    string_t combined = new_string(base->len + extension->len);
    
    string_write_multi(&combined, base, extension);

    return combined;
}

char* cstr(string_t* str)
{
    if (str->local)
    {
        return str->small;
    }
    return str->_str;
}

string_t string_copy(string_t* source)
{
    string_t newString = new_string(source->len);
    
    string_write(&newString, source);

    return newString;
}

char *cstring_copy(const char *source)
{
    int sLen = strlen(source);
    char *newString = safe_malloc(sLen * sizeof(char) + SPACE_FOR_NULL);

    // copying 'source' onto the new string
    for (int i = 0; i < sLen; i++)
    {
        newString[i] = source[i];
    }
    // null terminating
    newString[sLen] = '\0';

    return newString;
}

// Wrapper that destroys the given string
void string_destroy(string_t *toDestroy)
{
    if (!toDestroy->local)
    {
        destroy(toDestroy->_str);
    }
}

void void_string_destroy(void *toDestroy)
{
    if (!((string_t *)toDestroy)->local)
    {
        destroy(((string_t *)toDestroy)->_str);
    }
}
