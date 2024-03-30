#include <blockgame/model.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <blockgame/utility.h>
#include <blockgame/panic.h>
#include "str.h"

void
bg_object_face(struct bg_object_face *out)
{
     bg_vector(&out->vertices, sizeof(uint32_t));
     bg_vector(&out->normals, sizeof(uint32_t));
     bg_vector(&out->uvs, sizeof(uint32_t));
}

void
bg_object_face_free(struct bg_object_face *out)
{
     bg_vector_free(&out->vertices);
     bg_vector_free(&out->normals);
     bg_vector_free(&out->uvs);
}

void
bg_object_face_cleanup(void *out)
{
     bg_object_face_free((struct bg_object_face *)out);
}


void
bg_object_vertex(union bg_object_vertex *out, float x, float y, float z)
{
    out->x = x;
    out->y = y;
    out->z = z;
}


void
bg_object_uv(struct bg_object_uv *out, float x, float y)
{
    out->x = x;
    out->y = y;
}


void
bg_object(struct bg_object *object, char* name, size_t name_len)
{
    bg_vector(&object->vertices, sizeof(union bg_object_vertex));
    bg_vector(&object->normals, sizeof(union bg_object_vertex));
    bg_vector(&object->uvs, sizeof(struct bg_object_uv));
    bg_vector(&object->faces, sizeof(struct bg_object_face));

    object->object_name = bg_calloc(name_len, sizeof(char)+1);
    object->group_name = NULL;

    strncpy(object->object_name, name, name_len);
}

void
bg_object_shrink(struct bg_object *object)
{
    bg_vector_shrink(&object->vertices);
    bg_vector_shrink(&object->normals);
    bg_vector_shrink(&object->uvs);
    bg_vector_shrink(&object->faces);
}

void
bg_object_free(struct bg_object *object)
{
    free(object->object_name);
    free(object->group_name);
    bg_vector_free(&object->vertices);
    bg_vector_free(&object->normals);
    bg_vector_free(&object->uvs);
    bg_vector_cleanup(&object->faces, bg_object_face_cleanup);

    memset(object, 0, sizeof(struct bg_object));
}

void
bg_object_cleanup(void *object)
{
    bg_object_free(object);
}


void
bg_model(struct bg_model *model)
{
    bg_vector(&model->objects, sizeof(struct bg_object));
}

void
bg_model_free(struct bg_model *model)
{
    bg_vector_cleanup(&model->objects, bg_object_cleanup);
}


void
parse_object_(struct bg_object *out, char *line, size_t len)
{
    char *initial_line = line;

    line = str_after(' ', line);
    if (!line[0])
        bg_panic("An error occured while parsing .obj file: %s",
                 initial_line);

    size_t str_size = str_find_next('\n', line);
    if (len - (line - initial_line) == 0)
        bg_panic("Cannot create object without a name: %s",
                 line);
    bg_object(out, line, len - (line - initial_line));
}


// TODO: MAKE THIS WORK, IT IS UNFINISHED
void
parse_vertex_(union bg_object_vertex *out, char *line, size_t len)
{
    char *initial_line = line;

    line = str_after(' ', line);
    if (!line[0])
        bg_panic("An error occured while parsing .obj file: %s",
                 initial_line);

    for (int i = 0; i < 3; i++)
    {
        out->values[i] = strtof(line, NULL);
        line = str_after(' ', line);
    }
}

void
parse_line_(struct bg_model *out, char *line, size_t len)
{
    if (!len)
        return;


    if (str_starts_with(line, "#"))
        return;
    else if (str_starts_with(line, "o"))
    {
        struct bg_object obj;
        parse_object_(&obj, line, len);
        bg_vector_append(&out->objects, &obj, 1);
    }
    else if (str_starts_with(line, "v"))
    {
        union bg_object_vertex vert;
        parse_vertex_(&vert, line, len);
        struct bg_object *object =  bg_vector_at(struct bg_object, &out->objects, out->objects.length-1);
        bg_vector_append(&object->vertices, &vert, 1);
    }
    else
        bg_panic("Invalid object file syntax:\n  %s", line);
}

void
bg_load_model(struct bg_model *out, char *stream)
{
    bg_model(out);

    while (stream[0])
    {
        int line_len = str_find_next('\n', stream);
        parse_line_(out, stream, line_len);
        stream = str_next_line(stream);
    }
}