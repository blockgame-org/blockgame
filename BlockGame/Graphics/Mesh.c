#include <epoxy/gl.h>

#include <Graphics/Mesh.h>

#include <stdlib.h>
#include <string.h>

#include <Basic/utility.h>

struct bgMesh {
    unsigned int vao;
    unsigned int vbo;
    unsigned int ibo;
};

void bg_mesh(bgMesh **out, bgVertex const *vertices, size_t verticesCount,
             uint32_t const *indices, size_t indicesCount) {
    unsigned int vao = 0;
    unsigned int vbo = 0;
    unsigned int ibo = 0;

    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);
    glCreateBuffers(1, &ibo);

    bgVertex_configureVaoVbo(vao, vbo, 0);

    glNamedBufferStorage(vbo, verticesCount * sizeof(bgVertex), vertices,
                         GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(ibo, indicesCount * sizeof(uint32_t), indices,
                         GL_DYNAMIC_STORAGE_BIT);

    *out = bg_calloc(1, sizeof(bgMesh));
}

void bgMesh_bind(bgMesh const *mesh) {
    glBindVertexArray(mesh->vao);
    // TODO: Use glBindVertexBuffer?
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
}

void bgMesh_free(bgMesh *mesh) {
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->ibo);
    free(mesh);
}