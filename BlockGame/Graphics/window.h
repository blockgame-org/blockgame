#ifndef BLOCKGAME_WINDOW_H
#define BLOCKGAME_WINDOW_H

#include <Basic/mathtypes.h>

#include <stdint.h>

typedef struct bgWindow bgWindow;

extern bgWindow *BG_WINDOW;

int bg_initWindow(bgVec2u dimensions);

int bgWindow_isFocused(void);

void bgWindow_swap(void);

#endif // BLOCKGAME_WINDOW_H