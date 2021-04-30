#include "../string.h"

// RE-CHECKED 30/04/2021
void string_shrink(string_t *source, unsigned int new_len) {
  cstr(source)[new_len] = '\0';
  source->len = new_len;
}

// RE-CHECKED 30/04/2021
// Function has no real purpose other than making code slightly
// cleaner by removing the magic 0 function argument
// can make code using this clearer
string_t empty_string() { return new_string(0); }

// RE-CHECKED 30/04/2021
string_t new_string(unsigned int len) {
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

// RE-CHECKED 30/04/2021
// VERIFIED
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

// RE-CHECKED 30/04/2021
// VERIFIED
string_t string_make(char *src) {
  // No need to pre-allocate space as string_write_c will check
  // the length of the input str and allocate space in the new string
  // accordingly
  string_t str = new_string(0);

  string_write_c(&str, src);

  return str;
}

// RE-CHECKED 30/04/2021
// VERIFIED
int string_void_compare(const void *str1, const void *str2) {
  return string_compare((string_t *)str1, (string_t *)str2);
}

// RE-CHECKED 30/04/2021
// Idk how to implement this with null values
// null should be lower than any other string right?
// so add the check from string_equals once you know what youre doing
int string_compare(string_t *str1, string_t *str2) {
  return strcmp(cstr(str1), cstr(str2));
}

// TODO this should be a compare method not an equals method
// RE-CHECKED 30/04/2021
// 
// not yet verified as either string could have a null
// component
// so if they're both null then TRUE
// or if either one of them is null then FALSE
int string_equals(string_t *str1, string_t *str2) {
  if (str1->len != str2->len) {
    return FALSE;
  }

  char* string_1 = cstr(str1);
  char* string_2 = cstr(str2);

  // Do a null string comparison
  if (string_1 == NULL && string_2 == NULL) {
    return TRUE;
  } else if (string_1 == NULL || string_2 == NULL) {
    return FALSE;
  }

  return strcmp(string_1, string_2) == 0;
}

// TODO this should be a compare method not an equals method
// RE-CHECKED 30/04/2021
// VERIFIED
int cstring_equals(char *str1, char *str2) {
  return strcmp(str1, str2) == 0;
}

// TODO this should be a compare method not an equals method
// make it cstring_compare_range instead
// RE-CHECKED 30/04/2021
// VERIFIED
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
// VERIFIED
unsigned int string_count_occurances(string_t *source, char delim) {
  unsigned int occurances = 0;

  char *src_start = cstr(source);

  // Count the number of times delim appears in the str
  for (unsigned int index = 0; index < source->len; index++) {
    if (src_start[index] == delim) {
      ++occurances;
    }
  }

  return occurances;
}

// TODO this function shouldnt really ever be called...
// or should it in the case of writing a bunch of single characters?
// thats the only real case i can think of right now
// yeah i dont think we need it but i can keep it for harmless sake
// RE-CHECKED 29/04/2021
// VERIFIED
void string_null_terminate(string_t *str) {
  char* str_component = cstr(str);
  if (str_component) {
    cstr(str)[str->len] = '\0';
  }
}

// RE-CHECKED 29/04/2021
// VERIFIED
void string_write_char(string_t *base, char to_add) {
  if (to_add == '\0') {
    return;
  }
  
  // + 1 due to adding a singular char
  string_set_max_len(base, base->len + 1);

  // Write the char
  cstr(base)[base->len] = to_add;
  base->len += 1;

  // Null terminate the string
  cstr(base)[base->len] = '\0';
}

// RE-CHECKED 29/04/2021
// VERIFIED
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
// 
// RE-CHECKED 29/04/2021
// VERIFIED
string_t *string_write_c(string_t *base, char *source) {
  // Allocate space to store the extension
  unsigned int len = strlen(source);

  if (len == 0) {
    return base;
  }
  
  string_set_max_len(base, base->len + len);

  unsigned int source_pos = 0;
  unsigned int final_len = base->len + len;
  char *base_start = cstr(base);

  // Copy the data from source across
  for (unsigned int i = base->len; i < final_len; i++) {
    base_start[i] = source[source_pos++];
  }

  // Update the string's length
  base->len += len;

  // Null terminate
  // base's string component can no longer be null at this point
  base_start[base->len] = '\0';

  return base;
}

// RE-CHECKED 29/04/2021
// VALIDATED
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
// VALIDATED
string_t *string_write(string_t *base, string_t *source) {
  char *src_start = cstr(source);

  if (!src_start) {
    // Do nothing on a Null source string
    return base;
  }

  char *base_start = cstr(base);
  unsigned int source_pos = 0;
  unsigned int final_len = base->len + source->len;

  // Allocate enough space to fit the extension
  string_set_max_len(base, base->len + source->len);


  // Write from source onto base
  for (unsigned int index = base->len; index < final_len; index++) {
    base_start[index] = src_start[source_pos++];
  }

  // Update length
  base->len += source->len;

  // Null terminate 
  // Making sure the string is not a null string
  if (base_start) {
    base_start[base->len] = '\0';
  }

  return base;
}

// RE-CONFIRMED 29/04/2021
// VERIFIED
void string_set_max_len(string_t* str, unsigned int max_len)
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

// RE-CHECKED 30/04/2021
// VERIFIED
void string_extend(string_t *str) {
  unsigned int new_len = REALLOC_MULTIPLIER * (str->max_len + SPACE_FOR_NULL);

  /*
   * For cases when a string has a max length of 0 or 1
   * Can only occur if a cstring is wrapped into a string and the cstring
   * has a length of 0 or 1
  */
  if (str->max_len < SHORT_STR_LEN) {
    new_len = SHORT_STR_LEN + SPACE_FOR_NULL;
  }

  string_set_max_len(str, new_len);
}

// RE-CHECKED 30/04/2021
// VERIFIED
void string_lengthen(string_t *str, unsigned int len) {
  string_set_max_len(str, str->len + len);
}

// RE-CHECKED 30/04/2021
// VERIFIED
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

// RE-CHECKED 30/04/2021
// VERIFIED
string_t string_new_concat(string_t *base, string_t *extension) {
  string_t combined = new_string(base->len + extension->len);

  string_write(&combined, base);
  string_write(&combined, extension);

  return combined;
}

// RE-CHECKED 30/04/2021
// VERIFIED
char *cstr(string_t *str) {
  if (str->local) {
    return str->small;
  }
  return str->_str;
}

// RE-CHECKED 30/04/2021
// VERIFIED
void string_clear(string_t *str) {
  if (cstr(str)) {
    // Null terminate to the start of the string
    cstr(str)[0] = '\0';

    // Convince the string it has no length
    str->len = 0;
  }
}

// RE-CHECKED 30/04/2021
// VERIFIED
string_t string_copy(string_t *source) {
  string_t str = new_string(source->len);

  string_write(&str, source);

  return str;
}

// RE-CHECKED
// VERIFIED
char *cstring_copy(const char *source) {
  unsigned int len = strlen(source);
  char *str = safe_malloc((len + SPACE_FOR_NULL) * sizeof(char));

  // copying 'source' onto the new string
  for (unsigned int index = 0; index < len; index++) {
    str[index] = source[index];
  }
  // null terminating
  str[len] = '\0';

  return str;
}

// RE-CHECKED 30/04/2021
// VERIFIED
void string_destroy(string_t *str) {
  if (!str->local) {
    destroy(str->_str);
  }
}

// RE-CHECKED 30/04/2021
// VERIFIED
void void_string_destroy(void *str) {
  if (!((string_t *)str)->local) {
    destroy(((string_t *)str)->_str);
  }
}
