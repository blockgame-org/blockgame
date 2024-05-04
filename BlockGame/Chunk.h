#ifndef BLOCKGAME_CHUNK_H
#define BLOCKGAME_CHUNK_H

#include <Block.h>
#include <Graphics/Mesh.h>
#include <Basic/mathtypes.h>

#include <stdint.h>

#define BG_CHUNK_SIZE 8
#define BG_CHUNK_VOLUME (BG_CHUNK_SIZE * BG_CHUNK_SIZE * BG_CHUNK_SIZE)

#define bgChunk_at(chunk, v3)                                                  \
    (&((chunk)->blocks[(v3)[0] + (v3)[1] * BG_CHUNK_SIZE +                     \
                       (v3)[2] * BG_CHUNK_VOLUME]))

typedef struct {
    bgVec3i position;
    bgBlockId blocks[BG_CHUNK_VOLUME];
} bgChunk;

void bgChunk_generateMesh(bgChunk const *chunk);

#endif // BLOCKGAME_CHUNK_H