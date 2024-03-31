#ifndef BLOCKGAME_UTILITY_H
#define BLOCKGAME_UTILITY_H

#include <stddef.h>

void *bg_calloc(size_t count, size_t size);

void *bg_realloc(void *p, size_t size);

#endif // BLOCKGAME_UTILITY_H