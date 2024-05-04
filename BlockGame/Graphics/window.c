#include <Graphics/window.h>

#include <Graphics/sdl_utility.h>
#include <Basic/utility.h>

#include <SDL2/SDL.h>

bgWindow *BG_WINDOW = NULL;

struct bgWindow {
    SDL_Window *window;
    SDL_GLContext *glContext;
};

void destroyWindow_(void) {
    SDL_GL_DeleteContext(BG_WINDOW->glContext);
    SDL_DestroyWindow(BG_WINDOW->window);
    free(BG_WINDOW);
    BG_WINDOW = NULL;
}

int bg_initWindow(bgVec2u dimensions) {
    SDL_Window *window = SDL_CreateWindow(
        "Block Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        dimensions[0], dimensions[1], SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    // TODO: Errors

    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    // TODO: Errors

    assertSDL_("Failed to set swap interval", 0 == SDL_GL_SetSwapInterval(1));

    BG_WINDOW = bg_calloc(1, sizeof(bgWindow));
    BG_WINDOW->window = window;
    BG_WINDOW->glContext = glContext;

    atexit(destroyWindow_);

    return 0;
}

int bgWindow_isFocused(void) {
    return SDL_GetWindowFlags(BG_WINDOW->window) & SDL_WINDOW_INPUT_FOCUS;
}

void bgWindow_swap(void) { SDL_GL_SwapWindow(BG_WINDOW->window); }