#include "str.h"

size_t
str_find_next(char c, char *stream)
{
    size_t ind = 0;
    while (stream[ind] && stream[ind]!=c)
        ind++;
    return ind;
}

char *
str_next_line(char *stream)
{
    stream = &stream[str_find_next('\n', stream)];
    if (!stream[0])
        return stream;
    return &stream[1];
}

char *
str_skip(char c, char *stream)
{
    while (stream && stream[0]==c)
        stream++;
    return stream;
}

char *
str_after(char c, char *stream)
{
    return str_skip(c, &stream[str_find_next(c, stream)]);
}