#ifndef BLOCKGAME_TEXTURE2D_H
#define BLOCKGAME_TEXTURE2D_H

#include <epoxy/gl.h>

typedef struct {
    GLuint id;
    uint32_t width;
    uint32_t height;
} bgTexture2D;

int bg_readTexture2D(bgTexture2D *out, char const *filePath, GLint wrapMode,
                     GLint filterMode, int generateMipmap);

int bgTexture2D_use(bgTexture2D *t);

int bgTexture2D_free(bgTexture2D *t);

#endif // BLOCKGAME_TEXTURE2D_H