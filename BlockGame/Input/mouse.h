#ifndef BLOCKGAME_MOUSE_H
#define BLOCKGAME_MOUSE_H

#include <Basic/mathtypes.h>
#include <Graphics/window.h>

#include <SDL2/SDL.h>

typedef struct {
    bgVec2i oldPosition;
    bgVec2i position;
    bgVec2i positionDelta;

    bgVec2i oldScroll;
    bgVec2i scroll;
    bgVec2i scrollDelta;

    int leftButton;
    int middleButton;
    int rightButton;
} bgMouse;

extern bgMouse *BG_MOUSE;

void bg_initMouse(void);
int bgMouse_update(SDL_Event *e);
void bgMouse_setRelativeMode(int enabled);
void bgMouse_getRelativePosition(bgVec2i out);

#endif // BLOCKGAME_MOUSE_H