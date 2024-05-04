#include <Input/mouse.h>

#include <Basic/math.h>
#include <Basic/utility.h>

#include <string.h>

bgMouse *BG_MOUSE = NULL;

void destroyMouse_(void) {
    free(BG_MOUSE);
    BG_MOUSE = NULL;
}

void bg_initMouse(void) {
    BG_MOUSE = bg_calloc(1, sizeof(bgMouse));
    atexit(destroyMouse_);
}

int bgMouse_update(SDL_Event *e) {
    switch (e->type) {
    case SDL_MOUSEMOTION:
        memcpy(BG_MOUSE->oldPosition, BG_MOUSE->position, sizeof(bgVec2i));

        BG_MOUSE->position[0] = e->motion.x;
        BG_MOUSE->position[1] = e->motion.y;

        bgVec2i_sub(BG_MOUSE->positionDelta, BG_MOUSE->oldPosition,
                    BG_MOUSE->position);
        return 1;
    case SDL_MOUSEBUTTONDOWN:
        if (e->button.button == SDL_BUTTON_LEFT)
            BG_MOUSE->leftButton = 1;
        if (e->button.button == SDL_BUTTON_MIDDLE)
            BG_MOUSE->middleButton = 1;
        if (e->button.button == SDL_BUTTON_RIGHT)
            BG_MOUSE->rightButton = 1;
        return 1;
    case SDL_MOUSEBUTTONUP:
        if (e->button.button == SDL_BUTTON_LEFT)
            BG_MOUSE->leftButton = 0;
        if (e->button.button == SDL_BUTTON_MIDDLE)
            BG_MOUSE->middleButton = 0;
        if (e->button.button == SDL_BUTTON_RIGHT)
            BG_MOUSE->rightButton = 0;
        return 1;
    case SDL_MOUSEWHEEL:
        memcpy(BG_MOUSE->oldScroll, BG_MOUSE->scroll, sizeof(bgVec2i));
        BG_MOUSE->scroll[0] = e->wheel.x;
        BG_MOUSE->scroll[1] = e->wheel.y;
        bgVec2i_sub(BG_MOUSE->scrollDelta, BG_MOUSE->oldScroll,
                    BG_MOUSE->scroll);
        return 1;
    default:
        return 0;
    }
}

void bgMouse_setRelativeMode(int enabled) {
    SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP,
                            enabled ? "1" : "0", SDL_HINT_OVERRIDE);
    SDL_SetRelativeMouseMode(enabled);
}

void bgMouse_getRelativePosition(bgVec2i out) {
    SDL_GetRelativeMouseState(&out[0], &out[1]);
}