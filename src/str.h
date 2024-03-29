#ifndef BG_STR_H
#define BG_STR_H

#include <stddef.h>

size_t
str_find_next(char c, char *stream);

char *
str_next_line(char *stream);

char *
str_skip(char c, char *stream);

char *
str_after(char c, char *stream);

#endif // BG_STR_H