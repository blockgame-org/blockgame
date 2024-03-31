#ifndef BG_MATERIAL_H
#define BG_MATERIAL_H

#include <blockgame/model.h>

typedef union {
    struct {
        float r, g, b;
    };
    float values[3];
} bgColorf;

void bg_colorf(bgColorf *out, float r, float g, float b);

// https://en.wikipedia.org/wiki/Wavefront_.obj_file#Basic_materials
enum bgIlluminationModel {
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

typedef struct {
    bgColorf ambient;
    bgColorf diffuse;
    bgColorf specular_color;
    float specular_weight;
    float dissolve;        // 0 - 1
    bgColorf filter_color; // color of light through transparent materials
    float refraction_index;
    enum bgIlluminationModel illumination_model;

    char *ambient_texture;         // filename
    char *diffuse_texture;         // filename
    char *specular_texture;        // filename
    char *highlight_texture;       // filename
    char *alpha_texture;           // filename
    char *bump_texture;            // filename
    char *displacement_texture;    // filename
    char *decal_texture;           // filename
    bgObjectVertex texture_origin; // aka texture_offset
    bgObjectVertex texture_scale;

    char *name;
} bgModelMaterial;

void bg_modelMaterial(bgModelMaterial *out, char *name, size_t name_len);

void bgModelMaterial_setAmbientTexture(bgModelMaterial *mat, char *name,
                                       size_t name_len);

void bgModelMaterial_setDiffuseTexture(bgModelMaterial *mat, char *name,
                                       size_t name_len);

void bgModelMaterial_setSpecularTexture(bgModelMaterial *mat, char *name,
                                        size_t name_len);

void bgModelMaterial_setHighlightTexture(bgModelMaterial *mat, char *name,
                                         size_t name_len);

void bgModelMaterial_setAlphaTexture(bgModelMaterial *mat, char *name,
                                     size_t name_len);

void bgModelMaterial_setBumpTexture(bgModelMaterial *mat, char *name,
                                    size_t name_len);

void bgModelMaterial_setDisplacementTexture(bgModelMaterial *mat, char *name,
                                            size_t name_len);

void bgModelMaterial_setDecalTexture(bgModelMaterial *mat, char *name,
                                     size_t name_len);

void bgModelMaterial_free(bgModelMaterial *material);

void bgModelMaterial_cleanup(void *mat);

typedef struct {
    bgVector materials;

    char *mtllib_name;
} bgMtllib;

void bg_mtllib(bgMtllib *out, char *name, size_t name_len);

void bgMtllib_free(bgMtllib *mtllib);

void bgMtllib_parse(bgMtllib *out, char *stream, char *name, size_t name_len);

void bgMtllib_load(bgMtllib *out, char *file_name);

#endif // BG_MATERIAL_H