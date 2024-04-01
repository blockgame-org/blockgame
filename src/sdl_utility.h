#ifndef BG_SDL_UTILITY_H
#define BG_SDL_UTILITY_H

#include <blockgame/log.h>

#include <SDL2/SDL.h>

#define assertSDL_(message, x)                                                 \
    do {                                                                       \
        if (!(x)) {                                                            \
            bg_error(message ": %s", SDL_GetError());                          \
            return -1;                                                         \
        }                                                                      \
    } while (0)

#endif // BG_SDL_UTILITY_H