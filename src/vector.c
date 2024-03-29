#include <blockgame/vector.h>
#include <blockgame/utility.h>
#include <string.h>
#include <assert.h>

void
bg_vector(struct bg_vector *out, size_t element)
{
    out->raw = NULL;
    out->element = element;
    out->length = 0;
    out->capacity = 0;
}

void
bg_vector_grow(struct bg_vector *vec, size_t count)
{
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
        vec->raw = bg_realloc(vec->raw, (vec->capacity * 2  + 1)* vec->element);
}

void
bg_vector_append(struct bg_vector *vec, void *elements, size_t count)
{
    bg_vector_grow(vec, count);
    memcpy((char *)vec->raw + (vec->length * vec->element), elements, count * vec->element);
    vec->length += count;
}

void
bg_vector_push(struct bg_vector *vec, void *element)
{
    bg_vector_append(vec, element, 1);
}

void
bg_vector_free(struct bg_vector *vec)
{
    free(vec->raw);
    memset(vec, 0, sizeof(struct bg_vector));
}