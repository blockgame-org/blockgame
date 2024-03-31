#include "str.h"
#include <blockgame/file.h>
#include <blockgame/log.h>
#include <blockgame/model.h>
#include <blockgame/panic.h>
#include <blockgame/utility.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void bg_object_face(struct bg_object_face *out) {
  bg_vector(&out->vertices, sizeof(uint32_t));
  bg_vector(&out->normals, sizeof(uint32_t));
  bg_vector(&out->uvs, sizeof(uint32_t));
}

void bg_object_face_free(struct bg_object_face *face) {
  bg_vector_free(&face->vertices);
  bg_vector_free(&face->normals);
  bg_vector_free(&face->uvs);

  memset(face, 0, sizeof(struct bg_object_face));
}

void bg_object_face_cleanup(void *face) {
  bg_object_face_free((struct bg_object_face *)face);
}

void bg_object_vertex(union bg_object_vertex *out, float x, float y, float z) {
  out->x = x;
  out->y = y;
  out->z = z;
}

void bg_object_uv(union bg_object_uv *out, float x, float y) {
  out->x = x;
  out->y = y;
}

void bg_object(struct bg_object *object, char *name, size_t name_len) {
  bg_vector(&object->vertices, sizeof(union bg_object_vertex));
  bg_vector(&object->normals, sizeof(union bg_object_vertex));
  bg_vector(&object->uvs, sizeof(union bg_object_uv));
  bg_vector(&object->faces, sizeof(struct bg_object_face));

  object->object_name = bg_calloc(name_len + 1, sizeof(char));
  object->group_name = NULL;
  object->material_name = NULL;

  strncpy(object->object_name, name, name_len);
}

void bg_object_set_material(struct bg_object *object, char *name,
                            size_t name_len) {
  object->material_name = bg_calloc(name_len + 1, sizeof(char));
  strncpy(object->material_name, name, name_len);
}

void bg_object_set_group(struct bg_object *object, char *name,
                         size_t name_len) {
  object->group_name = bg_calloc(name_len + 1, sizeof(char));
  strncpy(object->group_name, name, name_len);
}

void bg_object_shrink(struct bg_object *object) {
  bg_vector_shrink(&object->vertices);
  bg_vector_shrink(&object->normals);
  bg_vector_shrink(&object->uvs);
  bg_vector_shrink(&object->faces);
}

void bg_object_free(struct bg_object *object) {
  free(object->object_name);
  free(object->group_name);
  free(object->material_name);
  bg_vector_free(&object->vertices);
  bg_vector_free(&object->normals);
  bg_vector_free(&object->uvs);
  bg_vector_cleanup(&object->faces, bg_object_face_cleanup);

  memset(object, 0, sizeof(struct bg_object));
}

void bg_object_cleanup(void *object) { bg_object_free(object); }

void bg_model(struct bg_model *model) {
  bg_vector(&model->objects, sizeof(struct bg_object));
  model->mtllib_name = NULL;
}

void bg_model_set_mtllib(struct bg_model *model, char *name, size_t name_len) {
  model->mtllib_name = bg_calloc(name_len + 1, sizeof(char));
  strncpy(model->mtllib_name, name, name_len);
}

void bg_model_free(struct bg_model *model) {
  free(model->mtllib_name);
  bg_vector_cleanup(&model->objects, bg_object_cleanup);
  memset(model, 0, sizeof(struct bg_model));
}

void parse_object_(struct bg_object *out, char *line, size_t len) {
  char *initial_line = line;
  line = skip_op_(line, len);

  if (len - (line - initial_line) == 0)
    bg_panic("Cannot create object without a name: %.*s", (int)len,
             initial_line);
  bg_object(out, line, len - (line - initial_line));
}

void parse_usemtl_(struct bg_object *out, char *line, size_t len) {
  char *initial_line = line;
  line = skip_op_(line, len);

  if (len - (line - initial_line) == 0)
    bg_panic("Cannot use material without a name: %.*s", (int)len, line);
  bg_object_set_material(out, line, len - (line - initial_line));
}

void parse_group_(struct bg_object *out, char *line, size_t len) {
  char *initial_line = line;
  line = skip_op_(line, len);

  if (len - (line - initial_line) == 0)
    bg_panic("Cannot set gruop without a name: %.*s", (int)len, line);
  bg_object_set_group(out, line, len - (line - initial_line));
}

void parse_mtllib_(struct bg_model *out, char *line, size_t len) {
  char *initial_line = line;
  line = skip_op_(line, len);

  if (len - (line - initial_line) == 0)
    bg_panic("Cannot use material library without a name: %.*s", (int)len,
             line);
  bg_model_set_mtllib(out, line, len - (line - initial_line));
}

void parse_vertex_(union bg_object_vertex *out, char *line, size_t len) {
  char *initial_line = line;
  line = skip_op_(line, len);

  for (int i = 0; i < 3; i++) {
    if (!line[0] || line[0] == '\n')
      bg_panic("Vertex does not contain enough values:\n  %.*s", (int)len,
               initial_line);
    out->values[i] =
        strtof(line, NULL); // TODO: check if it stops on an invalid char
    line = str_after(' ', line);
  }
}

void parse_uv_(union bg_object_uv *out, char *line, size_t len) {
  char *initial_line = line;
  line = skip_op_(line, len);

  for (int i = 0; i < 2; i++) {
    if (!line[0] || line[0] == '\n')
      bg_panic("UV does not contain enough values:\n  %.*s", (int)len,
               initial_line);
    out->values[i] = strtof(line, NULL);
    line = str_after(' ', line);
  }
}

/// @brief parses the digits in a face. `-2` is an unintialized/unset value.
void parse_face_digit_(int out[3], char *line) {
  for (int i = 0; i < 3; i++) {
    if (line[0] == '/') {
      line += 1;
      out[i] = -2;
      continue;
    }
    char *end = line;
    out[i] = strtod(line, &end);
    if (end[0] != '/')
      break;
    line = end + 1;
  }
}

void parse_face_(struct bg_object_face *out, char *line, size_t len) {
  char *initial_line = line;
  line = skip_op_(line, len);
  while ((line - initial_line) < len) {
    int values[3];
    parse_face_digit_((int *)&values, line);
    if (values[0] == -2)
      bg_panic("A parsing error has occured:\n  %s", initial_line);
    bg_vector_append(&out->vertices, &values[0], 1);
    if (values[1] != -2)
      bg_vector_append(&out->uvs, &values[1], 1);
    if (values[2] != -2)
      bg_vector_append(&out->normals, &values[2], 1);
    line = str_after(' ', line);
  }

  bg_vector_shrink(&out->vertices);
  bg_vector_shrink(&out->uvs);
  bg_vector_shrink(&out->normals);
}

void parse_model_line_(struct bg_model *out, char *line, size_t len) {
  if (!len)
    return;

  if (str_starts_with(line, "#"))
    return;
  else if (str_starts_with(line, "mtllib")) {
    parse_mtllib_(out, line, len);
  } else if (str_starts_with(line, "usemtl")) {
    struct bg_object *object =
        bg_vector_at(struct bg_object, &out->objects, out->objects.length - 1);
    parse_usemtl_(object, line, len);
  } else if (str_starts_with(line, "g")) {
    struct bg_object *object =
        bg_vector_at(struct bg_object, &out->objects, out->objects.length - 1);
    parse_group_(object, line, len);
  } else if (str_starts_with(line, "o")) {
    struct bg_object obj;
    parse_object_(&obj, line, len);
    bg_vector_append(&out->objects, &obj, 1);
  } else if (str_starts_with(line, "vt")) {
    union bg_object_uv uv;
    parse_uv_(&uv, line, len);
    struct bg_object *object =
        bg_vector_at(struct bg_object, &out->objects, out->objects.length - 1);
    bg_vector_append(&object->uvs, &uv, 1);
  } else if (str_starts_with(line, "vn")) {
    union bg_object_vertex vert;
    parse_vertex_(&vert, line, len);
    struct bg_object *object =
        bg_vector_at(struct bg_object, &out->objects, out->objects.length - 1);
    bg_vector_append(&object->normals, &vert, 1);
  } else if (str_starts_with(line, "vp")) {
    bg_warn("`vp` is unimplemented for .obj files:\n  %.*s", (int)len, line);
  } else if (str_starts_with(line, "v")) {
    union bg_object_vertex vert;
    parse_vertex_(&vert, line, len);
    struct bg_object *object =
        bg_vector_at(struct bg_object, &out->objects, out->objects.length - 1);
    bg_vector_append(&object->vertices, &vert, 1);
  } else if (str_starts_with(line, "f")) {
    struct bg_object_face face;
    parse_face_(&face, line, len);
    struct bg_object *object =
        bg_vector_at(struct bg_object, &out->objects, out->objects.length - 1);
    bg_vector_append(&object->faces, &face, 1);
  } else if (str_starts_with(line, "l")) {
    bg_warn("`l` is unimplemented for .obj files:\n  %.*s", (int)len, line);
  } else if (str_starts_with(line, "s")) {
    bg_warn("`s` is unimplemented for .obj files:\n  %.*s", (int)len, line);
  } else {
    bg_panic("Invalid object file syntax:\n  %.*s", (int)len, line);
  }
}

void bg_parse_model(struct bg_model *out, char *stream) {
  bg_model(out);

  while (stream[0]) {
    int line_len = str_find_next('\n', stream);
    parse_model_line_(out, stream, line_len);
    stream = str_next_line(stream);
  }

  for (int i = 0; i < out->objects.length; i++) {
    struct bg_object *object =
        bg_vector_at(struct bg_object, &out->objects, out->objects.length - 1);
    bg_vector_shrink(&object->vertices);
    bg_vector_shrink(&object->normals);
    bg_vector_shrink(&object->uvs);
    bg_vector_shrink(&object->faces);
  }

  bg_vector_shrink(&out->objects);
}

void bg_load_model(struct bg_model *out, char *file_name) {
  char *file_data = NULL;
  bg_read_file(&file_data, file_name);
  bg_parse_model(out, file_data);
}