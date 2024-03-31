#ifndef BG_MODEL_H
#define BG_MODEL_H

#include <blockgame/vector.h>


// QUESTION FOR WHEN I GET BACK
// SHOULD BE CREATE A "SHRINK" / "FIT EXACT" function to vectors to remove excess space.

struct bg_object_face
{
    struct bg_vector vertices; // indices as a uint32_t
    struct bg_vector normals; // indices as a uint32_t
    struct bg_vector uvs; // indices as a uint32_t

};

void
bg_object_face(struct bg_object_face *out);

void
bg_object_face_free(struct bg_object_face *out);

void
bg_object_face_cleanup(void *out);

union bg_object_vertex
{
    struct { float x, y, z; };
    float values[3];
};

void
bg_object_vertex(union bg_object_vertex *out, float x, float y, float z);

union bg_object_uv
{
    struct { float x, y; };
    float values[2];
};


void
bg_object_uv(union bg_object_uv *out, float x, float y);


struct bg_object
{
    struct bg_vector vertices; // bg_object_vertex vector
    struct bg_vector normals; // bg_object_vertex vector
    struct bg_vector uvs; // bg_object_uv vector
    struct bg_vector faces; // bg_object_face vector

    char* object_name;
    char* group_name;
    char* material_name;
};

void
bg_object(struct bg_object *object, char *name, size_t name_len);

void
bg_object_set_material(struct bg_object *object, char *name, size_t name_len);

void
bg_object_set_group(struct bg_object *object, char *name, size_t name_len);

void
bg_object_shrink(struct bg_object *object);

void
bg_object_free(struct bg_object *object);

void
bg_object_cleanup(void *object);


struct bg_model
{
    struct bg_vector objects;

    char* mtllib_name;
};

void
bg_model(struct bg_model *model);

void
bg_model_set_mtllib(struct bg_model *model, char *name, size_t name_len);

void
bg_model_free(struct bg_model *model);

void
bg_parse_model(struct bg_model *out, char *stream);

void
bg_load_model(struct bg_model *out, char *file_name);

#endif // BG_MODEL_H