#ifndef BLOCKGAME_VECTOR_H
#define BLOCKGAME_VECTOR_H

#include <stddef.h>

struct bg_vector
{
    void *raw;
    size_t element;
    size_t length;
    size_t capacity;
};

void
bg_vector_grow(struct bg_vector *vec, size_t count);

void
bg_vector_append(struct bg_vector *vec, void *elements, size_t count);

void
bg_vector(struct bg_vector *out, size_t element_size);

void
bg_vector_free(struct bg_vector *vec);

#endif // BLOCKGAME_VECTOR_H