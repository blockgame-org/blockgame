#ifndef BLOCKGAME_ENGINE_H
#define BLOCKGAME_ENGINE_H

#include <SDL2/SDL.h>

typedef struct {
    int running;
} bgEngine;

extern bgEngine *BG_ENGINE;

/// @brief Initializes the engine.
/// @return 0 on success, -1 on error.
int bg_initEngine(void);

int bgEngine_run(void);

#endif // BLOCKGAME_ENGINE_H