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

union bg_colorf
{
    struct { float r, g, b; };
    float values[3];
};

void
bg_colorf(union bg_colorf *out, float r, float g, float b);


// https://en.wikipedia.org/wiki/Wavefront_.obj_file#Basic_materials
enum bg_illumination_model
{
    COLOR = 0,
    COLOR_AND_AMBIENT = 1,
    HIGHLIGHT = 2,
    REFLECTION_RAYTRACE = 3,
    TRANSPARENCY_GLASS_RAYTRACE = 4,
    REFLECTION_FRESNEL_RAYTRACE = 5,
    TRANSPARENCY_REFRACTION_RAYTRACE = 6,
    TRANSPARENCY_REFRACTION_FRESNEL_RAYTRACE = 7,
    REFLECTION = 8,
    TRANSPARENCY_GLASS = 9,
    SHADOWS_ON_INVISIBLE_SURF = 10
};

struct bg_model_material
{
    union bg_colorf ambient;
    union bg_colorf diffuse;
    union bg_colorf specular_color;
    float specular_weight;
    float dissolve; // 0-1
    union bg_colorf filter_color; // color of light through transparent materials
    float refraction_index;
    enum bg_illumination_model illumination_model;

    char *ambient_texture; // filename
    char *specular_texture; // filename
    char *highlight_texture; // filename
    char *alpha_texture; // filename
    char *bump_texture; // filename
    char *displacement_texture; // filename
    char *decal_texture; // filename
    union bg_object_vertex texture_origin;
    union bg_object_vertex texture_scale;

    char* name;
};

struct bg_mtllib
{
    struct bg_vector materials;

    char* mtllib_name;
};

#endif // BG_MODEL_H