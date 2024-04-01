#ifndef BG_PARSING_H
#define BG_PARSING_H

#include "stdlib.h"
#include <blockgame/mathtypes.h>

void parsing_parseName(char **str_out, size_t *len_out, char *line, size_t len);

void parsing_parseInt(int *out, char *line, size_t len);

void parsing_parseFloat(float *out, char *line, size_t len);

void parsing_parseFloatArray(float *out, int count, char *line, size_t len);

#endif // BG_PARSING_H