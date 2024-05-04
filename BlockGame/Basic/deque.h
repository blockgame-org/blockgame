#ifndef BLOCKGAME_DEQUE_H
#define BLOCKGAME_DEQUE_H

#include <stddef.h>

typedef struct bgDequeNode bgDequeNode;
struct bgDequeNode {
    bgDequeNode *previous;
    bgDequeNode *next;
    char raw[];
};

typedef struct {
    size_t element;
    size_t length;
    bgDequeNode *head;
    bgDequeNode *tail;
} bgDeque;

void bg_deque(bgDeque *out, size_t element);
void bgDeque_append(bgDeque *deque, void *elements, size_t count);
void bgDeque_prepend(bgDeque *deque, void *elements, size_t count);
void bgDeque_dropEnd(bgDeque *deque, size_t count);
void bgDeque_free(bgDeque *deque);

#endif // BLOCKGAME_DEQUE_H