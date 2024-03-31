#ifndef BLOCKGAME_VERTEX_H
#define BLOCKGAME_VERTEX_H

#include <blockgame/mathtypes.h>

#define BG_VERTEX_POSITION_LOCATION 0
#define BG_VERTEX_NORMAL_LOCATION 1
#define BG_VERTEX_UV_LOCATION 2

typedef struct {
  bgVec3f position;
  bgVec3f normal;
  bgVec2f uv;
} bgVertex;

int bgVertex_configureVaoVbo(int vao, int vbo, int bindingIx);

#endif // BLOCKGAME_VERTEX_H