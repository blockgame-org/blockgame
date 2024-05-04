#include "str.h"
#include <Basic/panic.h>

size_t str_findNext(char c, char *stream) {
    size_t ind = 0;
    while (stream[ind] && stream[ind] != c)
        ind++;
    return ind;
}

char *str_nextLine(char *stream) {
    stream = &stream[str_findNext('\n', stream)];
    if (!stream[0])
        return stream;
    return &stream[1];
}

char *str_skip(char c, char *stream) {
    while (stream && stream[0] == c)
        stream++;
    return stream;
}

char *str_after(char c, char *stream) {
    return str_skip(c, &stream[str_findNext(c, stream)]);
}

int str_startsWith(char *str, char *substr) {
    while (str[0] && substr[0] && str[0] == substr[0]) {
        str++;
        substr++;
    }
    return !substr[0];
}

/// @brief skips the initial "op" at the begginning of a line
/// @return a progressed pointer
char *skip_op_(char *line, size_t len) {
    char *start = line;

    line = str_after(' ', line);
    if (!line[0])
        bg_panic("An error occured while parsing .obj file: %.*s", (int)len,
                 start);
    return line;
}