#include "../string.h"

static void string_lengthen(string_t *toExtend, unsigned int len);

string_t new_string(unsigned int len)
{
    string_t str;

    str.len = 0;
    str.max_len = len;

    str.str = safe_malloc(len * sizeof(char) + SPACE_FOR_NULL);
    str.str[0] = '\0';

    return str;
}

string_t string_from_cstring(char *source)
{
    string_t newString;

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

int string_equals(string_t str1, string_t str2)
{
    if (str1.len != str2.len)
    {
        return FALSE;
    }
    
    return cstring_equals(str1.str, str2.str);
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

unsigned int string_count_occurances(string_t source, char delim)
{
    unsigned int occurances = 0;
    for (int i = 0; i < source.len; i++)
    {
        if (source.str[i] == delim)
        {
            ++occurances;
        }
    }
    return occurances;
}

// array_t string_split(string_t source, char delim)
// {
//     unsigned int segments = string_count_occurances(source, delim) + 1;
//     array_t split = new_array(segments, sizeof(string_t));

//     for (int i = 0; i < source.len; i++)
//     {
//     }

//     return split;
// }

void string_null_terminate(string_t* str)
{
    if (!str || !str->str)
    {
        return;
    }

    str->str[str->len] = '\0';
}

void string_write_char(string_t *base, char toAdd)
{
    if (!base || !base->str)
    {
        return;
    }
    // Extend the string if too small
    if (base->max_len <= base->len)
    {
        string_extend(base);
    }
    
    // Write the char
    base->str[base->len] = toAdd;
    base->len += 1;
}

void string_write_c(string_t *base, char* source)
{
    if (!base || !base->str || !source)
    {
        return;
    }
    string_t extension = string_from_cstring(source);
    string_write(base, extension);
}

void string_write(string_t *base, string_t source)
{
    // If there isnt enough room to fit the extension
    if (base->max_len < base->len + source.len)
    {
        int newLen = base->max_len * REALLOC_MULTIPLIER;

        // If the new string is going to need more space than a normal extension
        if (newLen < base->len + source.len)
        {
            string_lengthen(base, base->len + source.len - base->max_len);
        }
        else
        {
            string_extend(base);
        }
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

void string_extend(string_t *toExtend)
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

static void string_lengthen(string_t *toExtend, unsigned int len)
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
string_t string_new_concat(string_t base, string_t extension)
{
    string_t combined = new_string(base.len + extension.len);

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

string_t string_copy(string_t source)
{
    string_t newString;

    // Updating string length field
    newString.len = source.len;
    newString.max_len = source.len;

    // Allocating space for the string
    newString.str = (char *)safe_malloc(sizeof(char) * source.len + SPACE_FOR_NULL);

    // copying 'source' onto the new string
    for (int i = 0; i < source.len; i++)
    {
        newString.str[i] = source.str[i];
    }

    // null terminating
    newString.str[newString.len] = '\0';

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
    destroy(toDestroy->str);
}

void void_string_destroy(void *toDestroy)
{
    destroy(((string_t *)toDestroy)->str);
}
