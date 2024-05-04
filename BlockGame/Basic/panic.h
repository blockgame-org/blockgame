#ifndef BLOCKGAME_PANIC_H
#define BLOCKGAME_PANIC_H

#include <Basic/macro.h>

#include <stdio.h>
#include <stdlib.h>

#define bg_panic(...)                                                          \
    do {                                                                       \
        fprintf(stderr, "Oh uh, I panicked in (" __FILE__                      \
                        ":" BG_STRINGIFY(__LINE__) "): ");                     \
        fprintf(stderr, __VA_ARGS__);                                          \
        fprintf(stderr, "\n");                                                 \
        abort();                                                               \
    } while (0);

#endif // BLOCKGAME_PANIC_H