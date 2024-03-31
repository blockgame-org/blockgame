#include <epoxy/gl.h>

#include <blockgame/vertex.h>

int bgVertex_configureVaoVbo(int vao, int vbo, int bindingIx) {
    glVertexArrayVertexBuffer(vao, bindingIx, vbo, 0, sizeof(bgVertex));

    glEnableVertexArrayAttrib(vao, BG_VERTEX_POSITION_LOCATION);
    glEnableVertexArrayAttrib(vao, BG_VERTEX_NORMAL_LOCATION);
    glEnableVertexArrayAttrib(vao, BG_VERTEX_UV_LOCATION);

    glVertexArrayAttribFormat(vao, BG_VERTEX_POSITION_LOCATION, 3, GL_FLOAT,
                              GL_FALSE, 0);
    glVertexArrayAttribFormat(vao, BG_VERTEX_NORMAL_LOCATION, 3, GL_FLOAT,
                              GL_FALSE, offsetof(bgVertex, position));
    glVertexArrayAttribFormat(vao, BG_VERTEX_UV_LOCATION, 2, GL_FLOAT, GL_FALSE,
                              offsetof(bgVertex, normal));

    glVertexArrayAttribBinding(vao, BG_VERTEX_POSITION_LOCATION, bindingIx);
    glVertexArrayAttribBinding(vao, BG_VERTEX_NORMAL_LOCATION, bindingIx);
    glVertexArrayAttribBinding(vao, BG_VERTEX_UV_LOCATION, bindingIx);

    return 0;
}