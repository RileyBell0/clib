#include "../console.h"

// RE-CHECKED 04/05/2021
// MEMORY_SAFE 04/05/2021
string_t console_bar(unsigned int len) {
  string_t bar = new_string(len);

  char* str = cstr(&bar);

  // make a bar of len 'len'
  for (unsigned int i = 0; i < len; i++) {
    str[i] = CONSOLE_BAR_CHAR;
  }
  str[len] = '\0';

  return bar;
}

// RE-CHECKED 04/05/2021
// MEMORY_SAFE 04/05/2021
void console_header(string_t* str) {
  string_t bar = console_bar(str->len);

  printf("%s\n%s\n%s\n", cstr(&bar), cstr(str), cstr(&bar));

  string_destroy(&bar);
}

// RE-CHECKED 04/05/2021
// MEMORY_SAFE 04/05/2021
void console_header_c(char* str)
{
    unsigned int len = strlen(str);
    string_t bar = console_bar(len);
    
    printf("%s\n%s\n%s\n", cstr(&bar), str, cstr(&bar));

    string_destroy(&bar);
}