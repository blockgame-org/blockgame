#ifndef BG_MATERIAL_H
#define BG_MATERIAL_H

#include <blockgame/model.h>

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
    float dissolve; // 0 - 1
    union bg_colorf filter_color; // color of light through transparent materials
    float refraction_index;
    enum bg_illumination_model illumination_model;

    char *ambient_texture; // filename
    char *diffuse_texture; // filename
    char *specular_texture; // filename
    char *highlight_texture; // filename
    char *alpha_texture; // filename
    char *bump_texture; // filename
    char *displacement_texture; // filename
    char *decal_texture; // filename
    union bg_object_vertex texture_origin; // aka texture_offset
    union bg_object_vertex texture_scale;

    char* name;
};

void
bg_model_material(struct bg_model_material *out, char *name, size_t name_len);

void
bg_model_material_set_ambient_texture(struct bg_model_material *mat, char *name, size_t name_len);

void
bg_model_material_set_diffuse_texture(struct bg_model_material *mat, char *name, size_t name_len);

void
bg_model_material_set_specular_texture(struct bg_model_material *mat, char *name, size_t name_len);

void
bg_model_material_set_highlight_texture(struct bg_model_material *mat, char *name, size_t name_len);

void
bg_model_material_set_alpha_texture(struct bg_model_material *mat, char *name, size_t name_len);

void
bg_model_material_set_bump_texture(struct bg_model_material *mat, char *name, size_t name_len);

void
bg_model_material_set_displacement_texture(struct bg_model_material *mat, char *name, size_t name_len);

void
bg_model_material_set_decal_texture(struct bg_model_material *mat, char *name, size_t name_len);

void
bg_model_material_free(struct bg_model_material *material);

void
bg_model_material_cleanup(void *mat);


struct bg_mtllib
{
    struct bg_vector materials;

    char* mtllib_name;
};

void
bg_mtllib(struct bg_mtllib *out, char *name, size_t name_len);

void
bg_mtllib_free(struct bg_mtllib *mtllib);


void
bg_parse_mtllib(struct bg_mtllib *out,  char *stream, char *name, size_t name_len);

void
bg_load_mtllib(struct bg_mtllib *out, char *file_name);

#endif // BG_MATERIAL_H