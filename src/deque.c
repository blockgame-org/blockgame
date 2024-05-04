#include <blockgame/deque.h>

#include <blockgame/utility.h>

#include <stdlib.h>
#include <string.h>

void bg_deque(bgDeque *out, size_t element) {
    memset(out, 0, sizeof(bgDeque));

    out->element = element;
}

void bgDeque_append(bgDeque *deque, void *elements, size_t count) {
    if (!count)
        return;

    bgDequeNode *start = deque->tail;
    bgDequeNode **tail = deque->tail ? &deque->tail->next : &deque->tail;

    for (size_t i = 0; i < count; ++i, tail = &(*tail)->next) {
        *tail = bg_calloc(1, sizeof(bgDequeNode) + deque->element);
        memcpy((*tail)->raw, (char *)elements + i * deque->element,
               deque->element);
    }

    if (!deque->head)
        deque->head = start;
    deque->tail = *tail;
}

void bgDeque_prepend(bgDeque *deque, void *elements, size_t count) {
    if (!count)
        return;

    bgDequeNode *end = deque->head;
    bgDequeNode **head = deque->head ? &deque->head->previous : &deque->head;

    for (size_t i = 0; i < count; ++i, head = &(*head)->previous) {
        *head = bg_calloc(1, sizeof(bgDequeNode) + deque->element);
        memcpy((*head)->raw,
               (char *)elements + (count - (i + 1)) * deque->element,
               deque->element);
    }

    if (!deque->tail)
        deque->tail = end;
    deque->head = *head;
}

void bgDeque_dropEnd(bgDeque *deque, size_t count) {
    bgDequeNode *tail = deque->tail;

    for (size_t i = 0; i < count; ++i) {
        bgDequeNode *temp = tail;
        tail = tail->previous;
        free(temp);
    }

    tail->next = NULL;
    deque->tail = tail;
}

void bgDeque_free(bgDeque *deque) {
    for (bgDequeNode *tail = deque->head; tail;) {
        bgDequeNode *next = tail->next;
        free(tail);
        tail = next;
    }

    memset(deque, 0, sizeof(bgDeque));
}