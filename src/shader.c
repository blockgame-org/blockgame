#include <epoxy/gl.h>

#include <blockgame/shader.h>

#include <stdlib.h>
#include <string.h>

#include <blockgame/log.h>
#include <blockgame/utility.h>

// TODO: Handle GL errors better

/// @brief Compiles a shader program.
/// @return Returns the compiled program ID, otherwise 0 on failure.
unsigned int compileShader_(GLenum type, char const *fileName,
                            char const *content, size_t contentLength) {
    unsigned int id = glCreateShader(type);

    glShaderSource(id, 1, &content, &(int){contentLength});
    glCompileShader(id);

    int status = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);

    if (status)
        return id;

    // Compilation wasn't successful

    int logLen = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen);

    char *logMsg = bg_calloc(logLen + 1, sizeof(char));
    glGetShaderInfoLog(id, logLen, NULL, logMsg);

    bg_error("Failed to compile shader '%s': %s",
             fileName ? fileName : "<unknown>", logMsg);

    free(logMsg);

    return 0;
}

int bg_vertexShader(bgShader *out, char const *fileName, char const *content,
                    size_t contentLength) {
    unsigned int id =
        compileShader_(GL_VERTEX_SHADER, fileName, content, contentLength);

    if (0 == id)
        return -1;
    out->id = id;
    return 0;
}

int bg_fragmentShader(bgShader *out, char const *fileName, char const *content,
                      size_t contentLength) {
    unsigned int id =
        compileShader_(GL_FRAGMENT_SHADER, fileName, content, contentLength);

    if (0 == id)
        return -1;
    out->id = id;
    return 0;
}

int bgShader_free(bgShader *shader) {
    glDeleteShader(shader->id);
    memset(shader, 0, sizeof(bgShader));
    return 0;
}

int bg_program(bgProgram *out, bgShader *shaders, size_t shaders_length,
               bgVertexAttribute *attributes, size_t attributesLength) {
    unsigned int id = glCreateProgram();

    for (size_t i = 0; i < shaders_length; ++i)
        glAttachShader(id, shaders[i].id);

    for (size_t i = 0; i < attributesLength; ++i)
        glBindAttribLocation(id, attributes[i].index, attributes[i].name);

    glLinkProgram(id);

    out->id = id;
    return 0;
}

int bgProgram_use(bgProgram *prog) {
    glUseProgram(prog->id);
    return 0;
}

int bgProgram_getUniformId(bgProgram *prog, char const *uniform_name) {
    return glGetUniformLocation(prog->id, uniform_name);
}

int bgProgram_setUniformMat4(bgProgram *prog, int location, bgMat4 mat) {
    glUniformMatrix4fv(location, 1, GL_FALSE, (float *)mat);
    return 0;
}

void bgProgram_free(bgProgram *prog) {
    glDeleteProgram(prog->id);
    memset(prog, 0, sizeof(bgProgram));
}