#include "parsing.h"
#include "str.h"
#include "string.h"
#include <blockgame/panic.h>

void parsing_parseName(char **str_out, size_t *len_out, char *line,
                       size_t line_len) {
    char *initial_line = line;
    line = skip_op_(line, line_len);

    if (line_len - (line - initial_line) == 0)
        bg_panic("Name cannot be empty:\n  %.*s", (int)line_len, initial_line);

    *len_out = line_len - (line - initial_line);
    *str_out = line;
}

void parsing_parseFloat(float *out, char *line, size_t len) {
    line = skip_op_(line, len);
    *out = strtof(line, NULL);
}

void parsing_parseInt(int *out, char *line, size_t len) {
    line = skip_op_(line, len);
    *out = strtod(line, NULL);
}

void parsing_parseFloatArray(float *out, int count, char *line, size_t len) {
    char *initial_line = line;
    line = skip_op_(line, len);

    for (int i = 0; i < count; i++) {
        if (!line[0] || line[0] == '\n')
            bg_panic(
                "float[%i] element does not contain enough values:\n  %.*s",
                count, (int)len, initial_line);
        out[i] =
            strtof(line, NULL); // TODO: check if it stops on an invalid char
        line = str_after(' ', line);
    }
}