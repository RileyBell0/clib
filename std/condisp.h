/*
 * Contains functions helpful for printing 
 * stuff quickly in a console window
*/

#ifndef CLIB_STD_CONDISP_H
#define CLIB_STD_CONDISP_H

#include "std/string.h"

void console_bar_c(char* str)
{
    unsigned int len = strlen(str);
    
    string_t bar = new_string(len);
    
    for (unsigned int i = 0; i < len; i++)
    {
        string_write_char(&bar, '-');
    }

    printf("%s\n%s\n%s\n", cstr(&bar), str, cstr(&bar));
}

#endif