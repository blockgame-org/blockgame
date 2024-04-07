#ifndef BLOCKGAME_SHADER_H
#define BLOCKGAME_SHADER_H

#include <blockgame/mathtypes.h>

#include <stddef.h>

typedef struct {
    unsigned int id;
} bgShader;

int bg_vertexShader(bgShader *out, char const *fileName, char const *content,
                    size_t contentLength);

int bg_fragmentShader(bgShader *out, char const *fileName, char const *content,
                      size_t contentLength);

int bgShader_free(bgShader *shader);

typedef struct {
    unsigned int index;
    char const *name;
} bgVertexAttribute;

typedef struct {
    unsigned int id;
} bgProgram;

int bg_program(bgProgram *out, bgShader *shaders, size_t shadersLength,
               bgVertexAttribute *attributes, size_t attributesLength);

int bgProgram_use(bgProgram *prog);

int bgProgram_getUniformId(bgProgram *prog, char const *uniformName);

int bgProgram_setUniform1i(bgProgram *prog, int location, int i);
int bgProgram_setUniformMat4(bgProgram *prog, int location, bgMat4 m);

void bgProgram_free(bgProgram *prog);

#endif // BLOCKGAME_SHADER_H