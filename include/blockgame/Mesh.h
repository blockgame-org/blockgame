#ifndef BLOCKGAME_MESH_H
#define BLOCKGAME_MESH_H

#include <blockgame/vertex.h>

typedef struct bgMesh bgMesh;

void bg_mesh(bgMesh **out, bgVertex const *vertices, size_t verticesCount,
             uint32_t const *indices, size_t indicesCount);
void bgMesh_bind(bgMesh const *mesh);
void bgMesh_free(bgMesh *mesh);

#endif // BLOCKGAME_MESH_H