#include "parsing.h"
#include <Basic/str.h>
#include <Basic/file.h>
#include <Basic/log.h>
#include <material.h>
#include <Basic/panic.h>
#include <Basic/utility.h>
#include <stdlib.h>
#include <string.h>

void bg_colorf(bgColorf *out, float r, float g, float b) {
    out->r = r;
    out->g = g;
    out->b = b;
}

void bg_modelMaterial(bgModelMaterial *out, char *name, size_t name_len) {
    bg_colorf(&out->ambient, 0.0f, 0.0f, 0.0f);
    bg_colorf(&out->diffuse, 0.0f, 0.0f, 0.0f);
    bg_colorf(&out->specular_color, 0.0f, 0.0f, 0.0f);
    out->specular_weight = 0;
    out->dissolve = 1;
    bg_colorf(&out->filter_color, 0.0f, 0.0f, 0.0f);
    out->refraction_index = 1;
    out->illumination_model = COLOR;

    out->ambient_texture = NULL;
    out->diffuse_texture = NULL;
    out->specular_texture = NULL;
    out->highlight_texture = NULL;
    out->alpha_texture = NULL;
    out->bump_texture = NULL;
    out->displacement_texture = NULL;
    out->decal_texture = NULL;

    bg_objectVertex(&out->texture_origin, 0, 0, 0);
    bg_objectVertex(&out->texture_scale, 1, 1, 1);

    out->name = bg_calloc(name_len + 1, sizeof(char));
    strncpy(out->name, name, name_len);
}

void bgModelMaterial_setAmbientTexture(bgModelMaterial *mat, char *name,
                                       size_t name_len) {
    mat->ambient_texture = bg_calloc(name_len + 1, sizeof(char));
    strncpy(mat->ambient_texture, name, name_len);
}

void bgModelMaterial_setDiffuseTexture(bgModelMaterial *mat, char *name,
                                       size_t name_len) {
    mat->diffuse_texture = bg_calloc(name_len + 1, sizeof(char));
    strncpy(mat->diffuse_texture, name, name_len);
}

void bgModelMaterial_setSpecularTexture(bgModelMaterial *mat, char *name,
                                        size_t name_len) {
    mat->specular_texture = bg_calloc(name_len + 1, sizeof(char));
    strncpy(mat->specular_texture, name, name_len);
}

void bgModelMaterial_setHighlightTexture(bgModelMaterial *mat, char *name,
                                         size_t name_len) {
    mat->highlight_texture = bg_calloc(name_len + 1, sizeof(char));
    strncpy(mat->highlight_texture, name, name_len);
}

void bgModelMaterial_setAlphaTexture(bgModelMaterial *mat, char *name,
                                     size_t name_len) {
    mat->alpha_texture = bg_calloc(name_len + 1, sizeof(char));
    strncpy(mat->alpha_texture, name, name_len);
}

void bgModelMaterial_setBumpTexture(bgModelMaterial *mat, char *name,
                                    size_t name_len) {
    mat->bump_texture = bg_calloc(name_len + 1, sizeof(char));
    strncpy(mat->bump_texture, name, name_len);
}

void bgModelMaterial_setDisplacementTexture(bgModelMaterial *mat, char *name,
                                            size_t name_len) {
    mat->displacement_texture = bg_calloc(name_len + 1, sizeof(char));
    strncpy(mat->displacement_texture, name, name_len);
}

void bgModelMaterial_setDecalTexture(bgModelMaterial *mat, char *name,
                                     size_t name_len) {
    mat->decal_texture = bg_calloc(name_len + 1, sizeof(char));
    strncpy(mat->decal_texture, name, name_len);
}

void bgModelMaterial_free(bgModelMaterial *mat) {
    free(mat->ambient_texture);
    free(mat->diffuse_texture);
    free(mat->specular_texture);
    free(mat->highlight_texture);
    free(mat->alpha_texture);
    free(mat->bump_texture);
    free(mat->displacement_texture);
    free(mat->decal_texture);
    free(mat->name);

    memset(mat, 0, sizeof(bgModelMaterial));
}

void bgModelMaterial_cleanup(void *mat) {
    bgModelMaterial_free((bgModelMaterial *)mat);
}

void bg_mtllib(bgMtllib *out, char *name, size_t name_len) {
    bg_vector(&out->materials, sizeof(bgModelMaterial));
    out->mtllib_name = bg_calloc(name_len + 1, sizeof(char));
    strncpy(out->mtllib_name, name, name_len);
}

void bgMtllib_free(bgMtllib *mtllib) {
    bgVector_cleanup(&mtllib->materials, bgModelMaterial_cleanup);
    free(mtllib->mtllib_name);
    memset(mtllib, 0, sizeof(bgMtllib));
}

// --parsing stuff--

void parse_newmtl_(bgModelMaterial *out, char *line, size_t len) {
    char *initial_line = line;
    line = skip_op_(line, len);

    if (len - (line - initial_line) == 0)
        bg_panic("Cannot create material without a name:\n  %.*s", (int)len,
                 initial_line);
    bg_modelMaterial(out, line, len - (line - initial_line));
}

void parse_color_(bgColorf *out, char *line, size_t len) {
    parsing_parseFloatArray((float *)out, 3, line, len);
}

void parse_material_line_(bgMtllib *out, char *line, size_t len) {
    if (!len)
        return;

    if (str_startsWith(line, "#"))
        return;
    else if (str_startsWith(line, "newmtl")) {
        bgModelMaterial mat;
        parse_newmtl_(&mat, line, len);
        bgVector_append(&out->materials, &mat, 1);
    } else if (str_startsWith(line, "Ka")) {
        bgColorf color;
        parse_color_(&color, line, len);
        bgModelMaterial *mat = bgVector_at(bgModelMaterial, &out->materials,
                                           out->materials.length - 1);
        mat->ambient = color;
    } else if (str_startsWith(line, "Kd")) {
        bgColorf color;
        parse_color_(&color, line, len);
        bgModelMaterial *mat = bgVector_at(bgModelMaterial, &out->materials,
                                           out->materials.length - 1);
        mat->diffuse = color;
    } else if (str_startsWith(line, "Ks")) {
        bgColorf color;
        parse_color_(&color, line, len);
        bgModelMaterial *mat = bgVector_at(bgModelMaterial, &out->materials,
                                           out->materials.length - 1);
        mat->specular_color = color;
    } else if (str_startsWith(line, "Ns")) {
        float weight;
        parsing_parseFloat(&weight, line, len);
        bgModelMaterial *mat = bgVector_at(bgModelMaterial, &out->materials,
                                           out->materials.length - 1);
        mat->specular_weight = weight;
    } else if (str_startsWith(line, "d")) {
        float d;
        parsing_parseFloat(&d, line, len);
        bgModelMaterial *mat = bgVector_at(bgModelMaterial, &out->materials,
                                           out->materials.length - 1);
        mat->dissolve = d;
    } else if (str_startsWith(line, "Tr")) {
        float d;
        parsing_parseFloat(&d, line, len);
        bgModelMaterial *mat = bgVector_at(bgModelMaterial, &out->materials,
                                           out->materials.length - 1);
        mat->dissolve = 1.0f - d;
    } else if (str_startsWith(line, "Tf")) {
        bgColorf color;
        parse_color_(&color, line, len);
        bgModelMaterial *mat = bgVector_at(bgModelMaterial, &out->materials,
                                           out->materials.length - 1);
        mat->filter_color = color;
    } else if (str_startsWith(line, "Ni")) {
        float d;
        parsing_parseFloat(&d, line, len);
        bgModelMaterial *mat = bgVector_at(bgModelMaterial, &out->materials,
                                           out->materials.length - 1);
        mat->refraction_index = d;
    } else if (str_startsWith(line, "illum")) {
        int d;
        parsing_parseInt(&d, line, len);
        bgModelMaterial *mat = bgVector_at(bgModelMaterial, &out->materials,
                                           out->materials.length - 1);
        mat->illumination_model = d;
    } else if (str_startsWith(line, "map_Ka")) {
        char *filename;
        size_t name_len;
        parsing_parseName(&filename, &name_len, line, len);
        bgModelMaterial *mat = bgVector_at(bgModelMaterial, &out->materials,
                                           out->materials.length - 1);
        bgModelMaterial_setAmbientTexture(mat, filename, name_len);
    } else if (str_startsWith(line, "map_Ks")) {
        char *filename;
        size_t name_len;
        parsing_parseName(&filename, &name_len, line, len);
        bgModelMaterial *mat = bgVector_at(bgModelMaterial, &out->materials,
                                           out->materials.length - 1);
        bgModelMaterial_setSpecularTexture(mat, filename, name_len);
    } else if (str_startsWith(line, "map_Kd")) {
        char *filename;
        size_t name_len;
        parsing_parseName(&filename, &name_len, line, len);
        bgModelMaterial *mat = bgVector_at(bgModelMaterial, &out->materials,
                                           out->materials.length - 1);
        bgModelMaterial_setDiffuseTexture(mat, filename, name_len);
    } else if (str_startsWith(line, "map_Ns")) {
        char *filename;
        size_t name_len;
        parsing_parseName(&filename, &name_len, line, len);
        bgModelMaterial *mat = bgVector_at(bgModelMaterial, &out->materials,
                                           out->materials.length - 1);
        bgModelMaterial_setHighlightTexture(mat, filename, name_len);
    } else if (str_startsWith(line, "map_d")) {
        char *filename;
        size_t name_len;
        parsing_parseName(&filename, &name_len, line, len);
        bgModelMaterial *mat = bgVector_at(bgModelMaterial, &out->materials,
                                           out->materials.length - 1);
        bgModelMaterial_setAlphaTexture(mat, filename, name_len);
    } else if (str_startsWith(line, "map_bump") ||
               str_startsWith(line, "bump")) {
        char *filename;
        size_t name_len;
        parsing_parseName(&filename, &name_len, line, len);
        bgModelMaterial *mat = bgVector_at(bgModelMaterial, &out->materials,
                                           out->materials.length - 1);
        bgModelMaterial_setBumpTexture(mat, filename, name_len);
    } else if (str_startsWith(line, "disp")) {
        char *filename;
        size_t name_len;
        parsing_parseName(&filename, &name_len, line, len);
        bgModelMaterial *mat = bgVector_at(bgModelMaterial, &out->materials,
                                           out->materials.length - 1);
        bgModelMaterial_setDisplacementTexture(mat, filename, name_len);
    } else if (str_startsWith(line, "decal")) {
        char *filename;
        size_t name_len;
        parsing_parseName(&filename, &name_len, line, len);
        bgModelMaterial *mat = bgVector_at(bgModelMaterial, &out->materials,
                                           out->materials.length - 1);
        bgModelMaterial_setDecalTexture(mat, filename, name_len);
    }
    // TODO: add pbr attributes here.
    else {
        bg_panic("Invalid .mtl file syntax:\n  %.*s", (int)len, line);
    }
}

void bgMtllib_parse(bgMtllib *out, char *stream, char *name, size_t name_len) {
    bg_mtllib(out, name, name_len);

    while (stream[0]) {
        int line_len = str_findNext('\n', stream);
        parse_material_line_(out, stream, line_len);
        stream = str_nextLine(stream);
    }

    bgVector_shrink(&out->materials);
}

void bgMtllib_load(bgMtllib *out, char *file_name) {
    char *file_data = NULL;
    bg_readFile(&file_data, file_name);
    bgMtllib_parse(out, file_data, file_name, strlen(file_name));
    free(file_data);
}