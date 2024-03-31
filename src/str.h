#ifndef BG_STR_H
#define BG_STR_H

#include <stddef.h>

size_t str_findNext(char c, char *stream);

char *str_nextLine(char *stream);

char *str_skip(char c, char *stream);

char *str_after(char c, char *stream);

int str_startsWith(char *str, char *substr);

char *skip_op_(char *line, size_t len);

#endif // BG_STR_H