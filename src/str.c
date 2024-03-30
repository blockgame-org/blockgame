#include "str.h"
#include <blockgame/panic.h>

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

int
str_starts_with(char *str, char *substr)
{
    while (str[0] && substr[0] && str[0] == substr[0])
    {
        str++;
        substr++;
    }
    return !substr[0];
}


/// @brief skips the initial "op" at the begginning of a line
/// @return a progressed pointer
char *
skip_op_(char *line, size_t len)
{
    char *initial_line = line;

    line = str_after(' ', line);
    if (!line[0])
        bg_panic("An error occured while parsing .obj file: %.*s",
                 (int) len, initial_line);
    return line;
}