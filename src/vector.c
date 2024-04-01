#include <blockgame/vector.h>

#include <blockgame/utility.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void bg_vector(bgVector *out, size_t element) {
    out->raw = NULL;
    out->element = element;
    out->length = 0;
    out->capacity = 0;
}

void bgVector_dropEnd(bgVector *vec, size_t count) {
    vec->length -= (count < vec->length) ? count : vec->length;
}

/// @brief calls cleanup function for all dropped elements
/// @param vec
/// @param count
/// @param cleanup
void bgVector_dropEndCleanup(bgVector *vec, size_t count,
                             void (*cleanup)(void *)) {
    for (size_t i = vec->length - count; i < vec->length; i++)
        cleanup((char *)vec->raw + (i * vec->element));

    bgVector_dropEnd(vec, count);
}

void bgVector_grow(bgVector *vec, size_t count) {
    assert(vec);

    if (vec->capacity >= count + vec->length)
        return;

    if (!vec->capacity && count)
        vec->capacity = 1;

    while (vec->capacity < count + vec->length)
        vec->capacity *= 2;

    if (vec->raw == NULL)
        vec->raw = bg_calloc(vec->capacity, vec->element);
    else
        vec->raw = bg_realloc(vec->raw, (vec->capacity * 2 + 1) * vec->element);
}

void bgVector_shrink(bgVector *vec) {
    if (!vec->length)
        return;
    vec->raw = bg_realloc(vec->raw, (vec->length) * vec->element);
    vec->capacity = vec->length;
}

void bgVector_append(bgVector *vec, void *elements, size_t count) {
    bgVector_grow(vec, count);
    memcpy((char *)vec->raw + (vec->length * vec->element), elements,
           count * vec->element);
    vec->length += count;
}

void bgVector_push(bgVector *vec, void *element) {
    bgVector_append(vec, element, 1);
}

void bgVector_free(bgVector *vec) {
    free(vec->raw);
    memset(vec, 0, sizeof(bgVector));
}

void bgVector_cleanup(bgVector *vec, void (*cleanup)(void *)) {
    for (size_t i = 0; i < vec->length; i++)
        cleanup((char *)vec->raw + (i * vec->element));

    bgVector_free(vec);
}

void bgVector_duplicate(bgVector *out, bgVector *src) {
    out->element = src->element;
    out->length = src->length;
    out->capacity = src->capacity;
    free(out->raw);

    out->raw = bg_calloc(out->capacity, out->element);

    memcpy(out->raw, src->raw, out->element * out->capacity);
}

// copy = func (dest, src);
void bgVector_copy(bgVector *out, bgVector *src, void (*copy)(void *, void *)) {
    out->element = src->element;
    out->length = src->length;
    out->capacity = src->capacity;
    free(out->raw);

    out->raw = bg_calloc(out->capacity, out->element);

    for (size_t i = 0; i < out->length; i++)
        copy(out->raw, src->raw);
}