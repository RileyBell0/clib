/*
 * Contains functions helpful for printing
 * stuff quickly in a console window
 */

#ifndef CLIB_STD_CONDISP_H
#define CLIB_STD_CONDISP_H

#include "../string.h"
#include <stdio.h>

#define CONSOLE_BAR_CHAR '-'

/*
 * Returns a string of length 'len' containing only CONSOLE_BAR_CHAR
 * returned string may have memory allocated and should be destroyed with
 * string_destroy once its use is completed
 */
string_t console_bar(unsigned int len);

/*
 * Given a string, prints inbetween two bars made of 'CONSOLE_BAR_CHAR' to
 * the standard output
 */
void console_header(string_t *str);

/*
 * Given a c-string, prints inbetween two bars made of 'CONSOLE_BAR_CHAR' to
 * the standard output
 */
void console_header_c(char *str);

#endif