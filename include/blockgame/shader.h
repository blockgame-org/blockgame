#ifndef BLOCKGAME_SHADER_H
#define BLOCKGAME_SHADER_H

#include <blockgame/mathtypes.h>

#include <stddef.h>

struct bg_shader {
  unsigned int id;
};

int bg_vertex_shader(struct bg_shader *out, char const *filename,
                     char const *content, size_t content_length);

int bg_fragment_shader(struct bg_shader *out, char const *filename,
                       char const *content, size_t content_length);

int bg_shader_free(struct bg_shader *shader);

struct bg_vertex_attribute {
  unsigned int index;
  char const *name;
};

struct bg_program {
  unsigned int id;
};

int bg_program(struct bg_program *out, struct bg_shader *shaders,
               size_t shaders_length, struct bg_vertex_attribute *attributes,
               size_t attributes_length);

int bg_program_use(struct bg_program *prog);

int bg_program_get_uniform_id(struct bg_program *prog,
                              char const *uniform_name);

int bg_program_set_uniform_mat4(struct bg_program *prog, int location,
                                bg_mat4 mat);

void bg_program_free(struct bg_program *prog);

#endif // BLOCKGAME_SHADER_H