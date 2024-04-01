#ifndef BLOCKGAME_VECTOR_H
#define BLOCKGAME_VECTOR_H

#include <stddef.h>

typedef struct {
    void *raw;
    size_t element;
    size_t length;
    size_t capacity;
} bgVector;

// TODO: Add an assert for idx
#define bgVector_at(type, vec, idx) (&((type *)(vec)->raw)[idx])

void bg_vector(bgVector *out, size_t element_size);

void bgVector_dropEnd(bgVector *vec, size_t count);

void bgVector_dropEndCleanup(bgVector *vec, size_t count,
                             void (*cleanup)(void *));

void bgVector_grow(bgVector *vec, size_t count);

void bgVector_shrink(bgVector *vec);

void bgVector_append(bgVector *vec, void *elements, size_t count);

void bgVector_free(bgVector *vec);

void bgVector_cleanup(bgVector *vec, void (*cleanup)(void *));

void bgVector_duplicate(bgVector *out, bgVector *src);

#endif // BLOCKGAME_VECTOR_H