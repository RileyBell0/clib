#include "../string.h"

/*
 * Ensures the string has a max length of at least max_len. If not, allocates
 * this space
 */
static void string_set_max_len(string_t *str, size_t max_len);

string_t string_new(char *input)
{
  string_t str;

  if (input)
  {
    str.len = strlen(input);
  }
  else
  {
    str.len = 0;
  }

  // Small string optimisation - define local str length
  if (str.len <= SHORT_STR_LEN)
  {
    str.max_len = SHORT_STR_LEN;
    str._str = str.small;
    str.local = true;
  }
  else
  {
    str.max_len = str.len;
    str._str = safe_malloc(str.len * sizeof(char) + SPACE_FOR_NULL);
    str.local = false;
  }

  // Write "input" onto the string
  if (input)
  {
    memcpy(str._str, input, str.len);
  }
  str._str[str.len] = '\0';

  return str;
}

string_t empty_string() { return string_new(NULL); }

string_t *string_clear(string_t *str)
{
  string_destroy(str);
  return str;
}

string_t *string_limit(string_t *source, size_t new_len)
{
  if (source->len > new_len)
  {
    cstr(source)[new_len] = '\0';
    source->len = new_len;
  }
  return source;
}

int string_compare(string_t *str1, string_t *str2)
{
  return strcmp(cstr(str1), cstr(str2));
}

bool string_equals(string_t *str1, string_t *str2)
{
  return strcmp(cstr(str1), cstr(str2)) == 0;
}

string_t *string_append_char(string_t *base, char to_add)
{
  if (to_add == '\0')
  {
    return base;
  }

  string_set_max_len(base, base->len + 1);

  // Append the char
  cstr(base)[base->len] = to_add;
  base->len += 1;
  cstr(base)[base->len] = '\0';

  return base;
}

string_t *string_append_c(string_t *base, char *source)
{
  size_t len = strlen(source);

  if (len > 0)
  {
    string_set_max_len(base, base->len + len);
    strncpy(&cstr(base)[base->len], source, len);
    base->len += len;
    cstr(base)[base->len] = '\0';
  }

  return base;
}

string_t *string_append_c_multi(string_t *base, char *source, ...)
{
  va_list vargs;
  va_start(vargs, source);

  char *str = source;
  while (str)
  {
    string_append_c(base, str);
    str = va_arg(vargs, char *);
  }

  va_end(vargs);
  return base;
}

string_t *string_append_c_len(string_t *base, char *source, size_t len)
{
  size_t srclen = strlen(source);
  if (len == 0 || srclen == 0)
  {
    return base;
  }

  // Constrain copy length to (at most) the length of the source string
  if (srclen < len)
  {
    len = srclen;
  }

  string_set_max_len(base, base->len + len);

  // Append the string
  strncpy(&cstr(base)[base->len], source, len);
  base->len += len;
  cstr(base)[base->len] = '\0';

  return base;
}

string_t *string_append(string_t *base, string_t *source)
{
  if (source->len == 0)
  {
    return base;
  }

  // Allocate enough space to fit the extension
  string_set_max_len(base, base->len + source->len);
  strncpy(&cstr(base)[base->len], cstr(source), source->len);
  base->len += source->len;
  cstr(base)[base->len] = '\0';

  return base;
}

string_t *string_append_multi(string_t *base, string_t *source, ...)
{
  va_list vargs;
  va_start(vargs, source);

  string_t *str = source;
  while (str)
  {
    string_append(base, str);
    str = va_arg(vargs, string_t *);
  }

  va_end(vargs);

  return base;
}

string_t string_copy(string_t *source)
{
  string_t str = string_new(NULL);
  string_set_max_len(&str, source->len);

  string_append(&str, source);

  return str;
}

string_t string_concat(string_t *base, string_t *extension)
{
  string_t combined = string_new(NULL);
  string_set_max_len(&combined, base->len + extension->len);

  string_append(&combined, base);
  string_append(&combined, extension);

  return combined;
}

string_t string_concat_multi(string_t *base, string_t *extension, ...)
{
  va_list vargs;
  va_start(vargs, extension);

  // Combine all provided strings
  string_t combined = string_copy(base);
  string_t *str = extension;
  while (str)
  {
    string_append(&combined, str);
    str = va_arg(vargs, string_t *);
  }

  va_end(vargs);
  return combined;
}

char *cstr(string_t *str)
{
  if (str->local)
  {
    return str->small;
  }
  return str->_str;
}

void string_destroy(string_t *str)
{
  // Destroy string data
  if (!str->local)
  {
    free(str->_str);
  }

  // Clear the string
  *str = empty_string();
}

void void_string_destroy(void *str)
{
  if (!((string_t *)str)->local)
  {
    destroy(((string_t *)str)->_str);
  }
}

//////////////////
// Utilities
//////////////////

static void string_set_max_len(string_t *str, size_t max_len)
{
  if (max_len > str->max_len)
  {
    if (str->local && max_len > SHORT_STR_LEN)
    {
      // Allocate memory for the string, it can no longer be local
      str->local = false;
      str->_str = safe_malloc((max_len + SPACE_FOR_NULL) * sizeof(char));
      memcpy(str->_str, str->small, (str->len + SPACE_FOR_NULL) * sizeof(char));
    }
    else
    {
      // Increase the memory block to fit the new required length
      str->_str =
          safe_realloc(str->_str, (max_len + SPACE_FOR_NULL) * sizeof(char));
    }

    str->max_len = max_len;
  }
}