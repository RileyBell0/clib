#include "../string.h"

static void string_lengthen(string *toExtend, unsigned int len);
static void string_extend(string *toExtend);

string new_string(unsigned int len)
{
    string str;

    str.len = 0;
    str.max_len = len;

    str.str = safe_malloc(len * sizeof(char) + SPACE_FOR_NULL);
    str.str[0] = '\0';

    return str;
}

string string_from_cstring(char* source)
{
    string newString;
    
    newString.str = source;
    if (source)
    {
        newString.len = strlen(newString.str);
    }
    else
    {
        newString.len = 0;
    }
    newString.max_len = newString.len;

    return newString;
}

#include <stdio.h>
// TODO
// not sure if this works
void string_write(string* base, string source)
{
    if (base->max_len < base->len + source.len)
    {
        string_lengthen(base, base->len + source.len - base->max_len);
    }

    int sourcePos = 0;
    unsigned int startLen = base->len;
    unsigned int finalLen = startLen + source.len;
    for (unsigned int i = startLen; i < finalLen; i++)
    {
        base->str[i] = source.str[sourcePos++];
        base->len += 1;
    }
    base->str[base->len] = '\0';
}

static void string_extend(string *toExtend)
{
    // multiplying by 1.5 then adding 1 to account for cases where strlen is 0 or 1
    int newLen = REALLOC_MULTIPLIER * toExtend->max_len;

    if (newLen < 2)
    {
        ++newLen;
    }

    // Increasing the length of the string and allocating space
    // Still need space for null termination
    toExtend->str = realloc(toExtend->str, (newLen + SPACE_FOR_NULL) * sizeof(char));

    // Ensure the string upgraded
    assert(toExtend->str);

    // Updating the length of the string
    toExtend->max_len = newLen;
}

static void string_lengthen(string *toExtend, unsigned int len)
{
    // multiplying by 1.5 then adding 1 to account for cases where strlen is 0 or 1
    int newLen = toExtend->max_len + len;

    // Increasing the length of the string and allocating space
    // Still need space for null termination
    toExtend->str = realloc(toExtend->str, (newLen + SPACE_FOR_NULL) * sizeof(char));

    // Ensure the string upgraded
    assert(toExtend->str);

    // Updating the length of the string
    toExtend->max_len = newLen;
}


// returns a new string that is the concatenation of the two inputs
string string_new_concat(string base, string extension)
{
    string combined = new_string(base.len + extension.len);

    // Writing 
    for (int i = 0; i < base.len; i++)
    {
        combined.str[combined.len++] = base.str[i];
    }
    for (int i = 0; i < extension.len; i++)
    {
        combined.str[combined.len++] = extension.str[i];
    }
    // Null terminating the new string
    combined.str[combined.len] = '\0';

    // updating string length field
    combined.len = combined.max_len;

    return combined;
}


string string_copy(string source)
{
    string newString;

    // Updating string length field
    newString.len = source.len;
    newString.max_len = source.len;

    // Allocating space for the string
    newString.str = (char*)safe_malloc(sizeof(char)*source.len + SPACE_FOR_NULL);

    // copying 'source' onto the new string
    for (int i = 0; i < source.len; i++)
    {
        newString.str[i] = source.str[i];
    }
    
    // null terminating
    newString.str[newString.len] = '\0';

    return newString;
}


char* cstring_copy(const char* source)
{
    int sLen = strlen(source);
    char *newString =  safe_malloc(sLen * sizeof(char) + SPACE_FOR_NULL);

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
void string_destroy(string *toDestroy)
{
    destroy(toDestroy->str);
}

void void_string_destroy(void *toDestroy)
{
    destroy(((string*)toDestroy)->str);
}