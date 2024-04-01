#include "str.h"
#include <blockgame/file.h>
#include <blockgame/log.h>
#include <blockgame/model.h>
#include <blockgame/panic.h>
#include <blockgame/utility.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void bg_objectFace(bgObjectFace *out) {
    bg_vector(&out->vertices, sizeof(uint32_t));
    bg_vector(&out->normals, sizeof(uint32_t));
    bg_vector(&out->uvs, sizeof(uint32_t));
}

void bgObjectFace_free(bgObjectFace *face) {
    bgVector_free(&face->vertices);
    bgVector_free(&face->normals);
    bgVector_free(&face->uvs);

    memset(face, 0, sizeof(bgObjectFace));
}

void bgObjectFace_cleanup(void *face) {
    bgObjectFace_free((bgObjectFace *)face);
}

void bg_objectVertex(bgObjectVertex *out, float x, float y, float z) {
    out->x = x;
    out->y = y;
    out->z = z;
}

void bgObject_uv(bgObjectUV *out, float x, float y) {
    out->x = x;
    out->y = y;
}

void bg_object(bgObject *object, char *name, size_t name_len) {
    bg_vector(&object->vertices, sizeof(bgObjectVertex));
    bg_vector(&object->normals, sizeof(bgObjectVertex));
    bg_vector(&object->uvs, sizeof(bgObjectUV));
    bg_vector(&object->faces, sizeof(bgObjectFace));

    object->object_name = bg_calloc(name_len + 1, sizeof(char));
    object->group_name = NULL;
    object->material_name = NULL;

    strncpy(object->object_name, name, name_len);
}

void bgObject_setMaterial(bgObject *object, char *name, size_t name_len) {
    object->material_name = bg_calloc(name_len + 1, sizeof(char));
    strncpy(object->material_name, name, name_len);
}

void bgObject_setGroup(bgObject *object, char *name, size_t name_len) {
    object->group_name = bg_calloc(name_len + 1, sizeof(char));
    strncpy(object->group_name, name, name_len);
}

void bgObject_shrink(bgObject *object) {
    bgVector_shrink(&object->vertices);
    bgVector_shrink(&object->normals);
    bgVector_shrink(&object->uvs);
    bgVector_shrink(&object->faces);
}

void bgObject_free(bgObject *object) {
    free(object->object_name);
    free(object->group_name);
    free(object->material_name);
    bgVector_free(&object->vertices);
    bgVector_free(&object->normals);
    bgVector_free(&object->uvs);
    bgVector_cleanup(&object->faces, bgObjectFace_cleanup);

    memset(object, 0, sizeof(bgObject));
}

void bgObject_cleanup(void *object) { bgObject_free(object); }

void bg_model(bgModel *model) {
    bg_vector(&model->objects, sizeof(bgObject));
    model->mtllib_name = NULL;
}

void bgModel_setMtllib(bgModel *model, char *name, size_t name_len) {
    model->mtllib_name = bg_calloc(name_len + 1, sizeof(char));
    strncpy(model->mtllib_name, name, name_len);
}

void bg_model_free(bgModel *model) {
    free(model->mtllib_name);
    bgVector_cleanup(&model->objects, bgObject_cleanup);
    memset(model, 0, sizeof(bgModel));
}

void parse_object_(bgObject *out, char *line, size_t len) {
    char *initial_line = line;
    line = skip_op_(line, len);

    if (len - (line - initial_line) == 0)
        bg_panic("Cannot create object without a name: %.*s", (int)len,
                 initial_line);
    bg_object(out, line, len - (line - initial_line));
}

void parse_usemtl_(bgObject *out, char *line, size_t len) {
    char *initial_line = line;
    line = skip_op_(line, len);

    if (len - (line - initial_line) == 0)
        bg_panic("Cannot use material without a name: %.*s", (int)len, line);
    bgObject_setMaterial(out, line, len - (line - initial_line));
}

void parse_group_(bgObject *out, char *line, size_t len) {
    char *initial_line = line;
    line = skip_op_(line, len);

    if (len - (line - initial_line) == 0)
        bg_panic("Cannot set gruop without a name: %.*s", (int)len, line);
    bgObject_setGroup(out, line, len - (line - initial_line));
}

void parse_mtllib_(bgModel *out, char *line, size_t len) {
    char *initial_line = line;
    line = skip_op_(line, len);

    if (len - (line - initial_line) == 0)
        bg_panic("Cannot use material library without a name: %.*s", (int)len,
                 line);
    bgModel_setMtllib(out, line, len - (line - initial_line));
}

void parse_vertex_(bgObjectVertex *out, char *line, size_t len) {
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

void parse_uv_(bgObjectUV *out, char *line, size_t len) {
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

void parse_face_(bgObjectFace *out, char *line, size_t len) {
    char *initial_line = line;
    line = skip_op_(line, len);
    while ((line - initial_line) < len) {
        int values[3];
        parse_face_digit_((int *)&values, line);
        if (values[0] == -2)
            bg_panic("A parsing error has occured:\n  %s", initial_line);
        bgVector_append(&out->vertices, &values[0], 1);
        if (values[1] != -2)
            bgVector_append(&out->uvs, &values[1], 1);
        if (values[2] != -2)
            bgVector_append(&out->normals, &values[2], 1);
        line = str_after(' ', line);
    }

    bgVector_shrink(&out->vertices);
    bgVector_shrink(&out->uvs);
    bgVector_shrink(&out->normals);
}

void parse_model_line_(bgModel *out, char *line, size_t len) {
    if (!len)
        return;

    if (str_startsWith(line, "#"))
        return;
    else if (str_startsWith(line, "mtllib")) {
        parse_mtllib_(out, line, len);
    } else if (str_startsWith(line, "usemtl")) {
        bgObject *object =
            bgVector_at(bgObject, &out->objects, out->objects.length - 1);
        parse_usemtl_(object, line, len);
    } else if (str_startsWith(line, "g")) {
        bgObject *object =
            bgVector_at(bgObject, &out->objects, out->objects.length - 1);
        parse_group_(object, line, len);
    } else if (str_startsWith(line, "o")) {
        bgObject obj;
        parse_object_(&obj, line, len);
        bgVector_append(&out->objects, &obj, 1);
    } else if (str_startsWith(line, "vt")) {
        bgObjectUV uv;
        parse_uv_(&uv, line, len);
        bgObject *object =
            bgVector_at(bgObject, &out->objects, out->objects.length - 1);
        bgVector_append(&object->uvs, &uv, 1);
    } else if (str_startsWith(line, "vn")) {
        bgObjectVertex vert;
        parse_vertex_(&vert, line, len);
        bgObject *object =
            bgVector_at(bgObject, &out->objects, out->objects.length - 1);
        bgVector_append(&object->normals, &vert, 1);
    } else if (str_startsWith(line, "vp")) {
        bg_warn("`vp` is unimplemented for .obj files:\n  %.*s", (int)len,
                line);
    } else if (str_startsWith(line, "v")) {
        bgObjectVertex vert;
        parse_vertex_(&vert, line, len);
        bgObject *object =
            bgVector_at(bgObject, &out->objects, out->objects.length - 1);
        bgVector_append(&object->vertices, &vert, 1);
    } else if (str_startsWith(line, "f")) {
        bgObjectFace face;
        parse_face_(&face, line, len);
        bgObject *object =
            bgVector_at(bgObject, &out->objects, out->objects.length - 1);
        bgVector_append(&object->faces, &face, 1);
    } else if (str_startsWith(line, "l")) {
        bg_warn("`l` is unimplemented for .obj files:\n  %.*s", (int)len, line);
    } else if (str_startsWith(line, "s")) {
        bg_warn("`s` is unimplemented for .obj files:\n  %.*s", (int)len, line);
    } else {
        bg_panic("Invalid object file syntax:\n  %.*s", (int)len, line);
    }
}

void bgModel_parse(bgModel *out, char *stream) {
    bg_model(out);

    while (stream[0]) {
        int line_len = str_findNext('\n', stream);
        parse_model_line_(out, stream, line_len);
        stream = str_nextLine(stream);
    }

    for (int i = 0; i < out->objects.length; i++) {
        bgObject *object =
            bgVector_at(bgObject, &out->objects, out->objects.length - 1);
        bgVector_shrink(&object->vertices);
        bgVector_shrink(&object->normals);
        bgVector_shrink(&object->uvs);
        bgVector_shrink(&object->faces);
    }

    bgVector_shrink(&out->objects);
}

void bgModel_load(bgModel *out, char *file_name) {
    char *file_data = NULL;
    bg_readFile(&file_data, file_name);
    bgModel_parse(out, file_data);
    free(file_data);
}