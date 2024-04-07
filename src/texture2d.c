#include <blockgame/texture2d.h>

#include <blockgame/log.h>

#include <SDL2/SDL_image.h>

int bg_readTexture2D(bgTexture2D *out, char const *filePath, GLint wrapMode,
                     GLint filterMode, int generateMipmap) {
    SDL_Surface *surface = IMG_Load(filePath);
    bg_info("Loaded texture '%s' (%dx%d)", filePath, surface->w, surface->h);

    if (!surface) {
        bg_error("Failed to load image '%s': %s", filePath, IMG_GetError());
        return -1;
    }

    GLuint id;
    glGenTextures(1, &id);

    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, surface->pixels);

    if (generateMipmap)
        glGenerateMipmap(id);

    out->id = id;
    out->width = surface->w;
    out->height = surface->h;

    SDL_FreeSurface(surface);

    return 0;
}

int bgTexture2D_use(bgTexture2D *t) {
  glBindTexture(GL_TEXTURE_2D, t->id);
}

int bgTexture2d_free(bgTexture2D *t) {
    glDeleteTextures(1, &t->id);
    memset(t, 0, sizeof(bgTexture2D));
    return 0;
}