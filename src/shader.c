#include <epoxy/gl.h>

#include <blockgame/shader.h>

#include <string.h>
#include <stdlib.h>

#include <blockgame/log.h>
#include <blockgame/utility.h>

// TODO: Handle GL errors better

/// @brief Compiles a shader program.
/// @return Returns the compiled program ID, otherwise 0 on failure.
unsigned int 
compile_shader_(
    GLenum type, 
    char const *filename, 
    char const *content,
    size_t content_length
)
{
    unsigned int id = glCreateShader(type); 

    glShaderSource(id, 1, &content, &(int){content_length});
    glCompileShader(id);

    int status = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);

    if (status)
        return id;

    // Compilation wasn't successful

    int log_length = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);

    char *log_message = bg_calloc(log_length + 1, sizeof(char));
    glGetShaderInfoLog(id, log_length, NULL, log_message);

    bg_error(
        "Failed to compile shader '%s': %s", 
        filename ? filename : "<unknown>",
        log_message
    );

    free(log_message);

    return 0;
}

int
bg_vertex_shader(
    struct bg_shader *out,
    char const *filename,
    char const *content,
    size_t content_length
)
{
    unsigned int id = compile_shader_(
        GL_VERTEX_SHADER,
        filename,
        content,
        content_length
    );

    if (0 == id)
        return -1;
    out->id = id;
    return 0;
}

int
bg_fragment_shader(
    struct bg_shader *out,
    char const *filename,
    char const *content,
    size_t content_length
)
{
    unsigned int id = compile_shader_(
        GL_FRAGMENT_SHADER,
        filename,
        content,
        content_length
    );

    if (0 == id)
        return -1;
    out->id = id;
    return 0;
}

int
bg_shader_free(struct bg_shader *shader)
{
    glDeleteShader(shader->id);
    memset(shader, 0, sizeof(struct bg_shader));
    return 0;
}

int
bg_program(
    struct bg_program *out,
    struct bg_shader *shaders,
    size_t shaders_length,
    struct bg_vertex_attribute *attributes,
    size_t attributes_length
)
{
    unsigned int id = glCreateProgram();

    for (size_t i = 0; i < shaders_length; ++i)
        glAttachShader(id, shaders[i].id);

    for (size_t i = 0; i < attributes_length; ++i)
        glBindAttribLocation(
            id, 
            attributes[i].index, 
            attributes[i].name
        );

    glLinkProgram(id);
    
    out->id = id;
    return 0;
}

void
bg_program_free(struct bg_program *prog)
{
    glDeleteProgram(prog->id);
    memset(prog, 0, sizeof(struct bg_program));
}