#include "../string.h"

void string_shrink(string_t *source, uint32_t new_len) {
  cstr(source)[new_len] = '\0';
  source->len = new_len;
}

string_t empty_string() { return new_string(0); }

string_t new_string(uint32_t len) {
  string_t str;

  str.len = 0;

  // Small string optimisation
  if (len <= SHORT_STR_LEN) {
    str.max_len = SHORT_STR_LEN;
    str._str = str.small;
    str.local = TRUE;
  } else {
    str.max_len = len;
    str._str = safe_malloc(len * sizeof(char) + SPACE_FOR_NULL);
    str.local = FALSE;
  }

  str._str[0] = '\0';

  return str;
}

string_t cstring_wrap(char *src) {
  string_t str;

  str._str = src;
  str.local = FALSE;

  if (src) {
    str.len = strlen(src);
  } else {
    str.len = 0;
  }

  str.max_len = str.len;

  return str;
}

string_t string_make(char *src) {
  string_t str = new_string(0);

  string_write_c(&str, src);

  return str;
}

string_t *string_set(string_t *str, char *src) {
  str->_str = src;
  str->local = FALSE;

  if (src) {
    str->len = strlen(src);
  } else {
    str->len = 0;
  }
  str->max_len = str->len;

  return str;
}

int string_void_compare(const void *str1, const void *str2) {
  return string_compare((string_t *)str1, (string_t *)str2);
}


int string_compare(string_t *str1, string_t *str2) {
  return strcmp(cstr(str1), cstr(str2));
}

// TODO this should be a compare method not an equals method
int string_equals(string_t *str1, string_t *str2) {
  if (str1->len != str2->len) {
    return FALSE;
  }

  return cstring_equals(cstr(str1), cstr(str2));
}

// TODO this should be a compare method not an equals method
int cstring_equals(char *str1, char *str2) {
  int i = 0;
  while (TRUE) {
    if (str1[i] != str2[i]) {
      return FALSE;
    } else if (str1[i] == str2[i] && str1[i] == '\0') {
      return TRUE;
    }

    ++i;
  }
  return TRUE;
}

// TODO this should be a compare method not an equals method
int cstring_equals_range(char *str_1, char *str_2, int compare_range) {
  // Compare the strings within the given range.
  for (int index = 0; index < compare_range; index++) {
    if (str_1[index] != str_2[index] || str_1[index] == '\0') {
      return FALSE;
    }
  }

  return TRUE;
}

// RE-CHECKED 29/04/2021
uint32_t string_count_occurances(string_t *source, char delim) {
  uint32_t occurances = 0;

  char *src_start = cstr(source);

  // Count the number of times delim appears in the str
  for (uint32_t index = 0; index < source->len; index++) {
    if (src_start[index] == delim) {
      ++occurances;
    }
  }

  return occurances;
}

// TODO this function shouldnt really ever be called...
// RE-CHECKED 29/04/2021
void string_null_terminate(string_t *str) {
  cstr(str)[str->len] = '\0';
}

// RE-CHECKED 29/04/2021
void string_write_char(string_t *base, char to_add) {
  // + 1 due to adding a singular char
  string_set_max_len(base, base->len + 1 * sizeof(char));

  // Write the char
  cstr(base)[base->len] = to_add;
  base->len += 1;

  // Null terminate the string
  cstr(base)[base->len] = '\0';
}

// RE-CHECKED 29/04/2021
string_t *string_write_c_multi(string_t *base, char *source, ...) {
  va_list vargs;
  va_start(vargs, source);

  char *str = source;

  while (str) {
    string_write_c(base, str);
    str = va_arg(vargs, char *);
  }

  va_end(vargs);

  return base;
}

// TODO this will work fine but is this really any faster than just
// calling cstring_wrap then string_write?
// RE-CHECKED 29/04/2021
string_t *string_write_c(string_t *base, char *source) {
  // Allocate space to store the extension
  uint32_t len = strlen(source);
  string_set_max_len(base, base->len + len);

  uint32_t source_pos = 0;
  uint32_t final_len = base->len + len;
  char *base_start = cstr(base);
  char *src_start = cstr(source);

  // Copy the data from source across
  for (uint32_t i = base->len; i < final_len; i++) {
    base_start[i] = src_start[source_pos++];
  }

  // Update the string's length
  base->len += len;

  // Null terminate
  base_start[base->len] = '\0';

  return base;
}

// RE-CHECKED 29/04/2021
string_t *string_write_multi(string_t *base, string_t *source, ...) {
  va_list vargs;
  va_start(vargs, source);

  string_t *str = source;
  while (str) {
    string_write(base, str);
    str = va_arg(vargs, string_t *);
  }

  va_end(vargs);

  return base;
}

// RE-CHECKED 29/04/2021
string_t *string_write(string_t *base, string_t *source) {
  // Allocate enough space to fit the extension
  string_set_max_len(base, base->len + source->len);

  uint32_t source_pos = 0;
  uint32_t final_len = base->len + source->len;
  char *base_start = cstr(base);
  char *src_start = cstr(source);

  // Write from source onto base
  for (uint32_t index = base->len; index < final_len; index++) {
    base_start[index] = src_start[source_pos++];
  }

  // Update length
  base->len += source->len;

  // Null terminate
  base_start[base->len] = '\0';

  return base;
}

// RE-CONFIRMED 29/04/2021
void string_set_max_len(string_t* str, uint32_t max_len)
{
  if (max_len > str->max_len) {
    // If the current string is a short string
    if (str->local && max_len > SHORT_STR_LEN) {
      // String can no longer be local
      str->local = FALSE;

      // Allocate space to store the string from now on
      str->_str = safe_malloc((max_len + SPACE_FOR_NULL) * sizeof(char));

      // Copy over the string to the new memory
      memcpy(str->_str, str->small, (str->len + SPACE_FOR_NULL) * sizeof(char));
    } else {
      // Increase the memory block to fit the new required length
      str->_str = safe_realloc(str->_str, (max_len + SPACE_FOR_NULL) * sizeof(char));
    }

    str->max_len = max_len;
  }
}

// TODO see in function
void string_extend(string_t *str) {
  uint32_t new_len = REALLOC_MULTIPLIER * (str->max_len + SPACE_FOR_NULL) * sizeof(char);

  // TODO does this need the +1 check for extending hte string to make sure a str of length 1 doesnt keep
  // just being length 1?

  string_set_max_len(str, new_len);
}

void string_lengthen(string_t *str, uint32_t len) {
  string_set_max_len(str, str->len + len);
}

string_t string_new_concat_multi(string_t *base, string_t *extension, ...) {
  va_list vargs;
  va_start(vargs, extension);

  string_t copy = string_copy(base);

  string_t *str = extension;

  while (str) {
    string_write(&copy, str);
    str = va_arg(vargs, string_t *);
  }

  va_end(vargs);

  return copy;
}

string_t string_new_concat(string_t *base, string_t *extension) {
  string_t combined = new_string(base->len + extension->len);

  string_write(&combined, base);
  string_write(&combined, extension);

  return combined;
}

char *cstr(string_t *str) {
  if (str->local) {
    return str->small;
  }
  return str->_str;
}

void string_clear(string_t *str) {
  // Null terminate to the start of the string
  cstr(str)[0] = '\0';

  // Convince the string it has no length
  str->len = 0;
}

string_t string_copy(string_t *source) {
  string_t str = new_string(source->len);

  string_write(&str, source);

  return str;
}

char *cstring_copy(const char *source) {
  int len = strlen(source);
  char *str = safe_malloc(len * sizeof(char) + SPACE_FOR_NULL);

  // copying 'source' onto the new string
  for (int i = 0; i < len; i++) {
    str[i] = source[i];
  }
  // null terminating
  str[len] = '\0';

  return str;
}

// Wrapper that destroys the given string
void string_destroy(string_t *str) {
  if (!str->local) {
    destroy(str->_str);
  }
}

void void_string_destroy(void *str) {
  if (!((string_t *)str)->local) {
    destroy(((string_t *)str)->_str);
  }
}
