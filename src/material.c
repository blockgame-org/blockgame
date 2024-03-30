#include <blockgame/material.h>
#include <stdlib.h>
#include <string.h>
#include "str.h"
#include <blockgame/file.h>
#include <blockgame/utility.h>
#include <blockgame/panic.h>
#include <blockgame/log.h>

void
bg_colorf(union bg_colorf *out, float r, float g, float b)
{
    out->r = r;
    out->g = g;
    out->b = b;
}


void
bg_model_material(struct bg_model_material *out, char *name, size_t name_len)
{
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

    bg_object_vertex(&out->texture_origin, 0, 0, 0);
    bg_object_vertex(&out->texture_scale, 1, 1, 1);

    out->name = bg_calloc(name_len+1, sizeof(char));
    strncpy(out->name, name, name_len);
}

void
bg_model_material_set_ambient_texture(struct bg_model_material *mat, char *name, size_t name_len)
{
    mat->ambient_texture = bg_calloc(name_len+1, sizeof(char));
    strncpy(mat->ambient_texture, name, name_len);
}

void
bg_model_material_set_diffuse_texture(struct bg_model_material *mat, char *name, size_t name_len)
{
    mat->diffuse_texture = bg_calloc(name_len+1, sizeof(char));
    strncpy(mat->diffuse_texture, name, name_len);
}

void
bg_model_material_set_specular_texture(struct bg_model_material *mat, char *name, size_t name_len)
{
    mat->specular_texture = bg_calloc(name_len+1, sizeof(char));
    strncpy(mat->specular_texture, name, name_len);
}

void
bg_model_material_set_highlight_texture(struct bg_model_material *mat, char *name, size_t name_len)
{
    mat->highlight_texture = bg_calloc(name_len+1, sizeof(char));
    strncpy(mat->highlight_texture, name, name_len);
}

void
bg_model_material_set_alpha_texture(struct bg_model_material *mat, char *name, size_t name_len)
{
    mat->alpha_texture = bg_calloc(name_len+1, sizeof(char));
    strncpy(mat->alpha_texture, name, name_len);
}

void
bg_model_material_set_bump_texture(struct bg_model_material *mat, char *name, size_t name_len)
{
    mat->bump_texture = bg_calloc(name_len+1, sizeof(char));
    strncpy(mat->bump_texture, name, name_len);
}

void
bg_model_material_set_displacement_texture(struct bg_model_material *mat, char *name, size_t name_len)
{
    mat->displacement_texture = bg_calloc(name_len+1, sizeof(char));
    strncpy(mat->displacement_texture, name, name_len);
}

void
bg_model_material_set_decal_texture(struct bg_model_material *mat, char *name, size_t name_len)
{
    mat->decal_texture = bg_calloc(name_len+1, sizeof(char));
    strncpy(mat->decal_texture, name, name_len);
}

void
bg_model_material_free(struct bg_model_material *mat)
{
    free(mat->ambient_texture);
    free(mat->diffuse_texture);
    free(mat->specular_texture);
    free(mat->highlight_texture);
    free(mat->alpha_texture);
    free(mat->bump_texture);
    free(mat->displacement_texture);
    free(mat->decal_texture);
    free(mat->name);

    memset(mat, 0, sizeof(struct bg_model_material));
}

void
bg_model_material_cleanup(void *mat)
{
    bg_model_material_free((struct bg_model_material *) mat);
}


void
bg_mtllib(struct bg_mtllib *out, char *name, size_t name_len)
{
    bg_vector(&out->materials, sizeof(struct bg_model_material));
    out->mtllib_name = bg_calloc(name_len+1, sizeof(char));
    strncpy(out->mtllib_name, name, name_len);
}

void
bg_mtllib_free(struct bg_mtllib *mtllib)
{
    bg_vector_cleanup(&mtllib->materials, bg_model_material_cleanup);
    free(mtllib->mtllib_name);
    memset(mtllib, 0, sizeof(struct bg_mtllib));
}


// --parsing stuff--

void
parse_newmtl_(struct bg_model_material *out, char *line, size_t len)
{
    char *initial_line = line;
    line = skip_op_(line, len);

    if (len - (line - initial_line) == 0)
        bg_panic("Cannot create material without a name:\n  %.*s",
                 (int) len, initial_line);
    bg_model_material(out, line, len - (line - initial_line));
}

void
parse_filename_(char **str_out, size_t *len_out, char *line, size_t len)
{
    char *initial_line = line;
    line = skip_op_(line, len);

    if (len - (line - initial_line) == 0)
        bg_panic("Filename cannot be empty:\n  %.*s",
                 (int) len, initial_line);

    *len_out = len - (line - initial_line);
    *str_out = line;
}

void
parse_color_(union bg_colorf *out, char *line, size_t len)
{
    char *initial_line = line;
    line = skip_op_(line, len);

    for (int i = 0; i < 3; i++)
    {
        if (!line[0] || line[0]=='\n')
            bg_panic("color element does not contain enough values:\n  %.*s", (int) len, initial_line);
        out->values[i] = strtof(line, NULL); // TODO: check if it stops on an invalid char
        line = str_after(' ', line);
    }
}

void
parse_float_(float *out, char *line, size_t len)
{
    line = skip_op_(line, len);
    *out = strtof(line, NULL);
}

void
parse_int_(int *out, char *line, size_t len)
{
    line = skip_op_(line, len);
    *out = strtod(line, NULL);
}


void
parse_material_line_(struct bg_mtllib *out, char *line, size_t len)
{
    if (!len)
        return;

    if (str_starts_with(line, "#"))
        return;
    else if (str_starts_with(line, "newmtl"))
    {
        struct bg_model_material mat;
        parse_newmtl_(&mat, line, len);
        bg_vector_append(&out->materials, &mat, 1);
    }
    else if (str_starts_with(line, "Ka"))
    {
        union bg_colorf color;
        parse_color_(&color, line, len);
        struct bg_model_material *mat =  bg_vector_at(struct bg_model_material, &out->materials, out->materials.length-1);
        mat->ambient = color;
    }
    else if (str_starts_with(line, "Kd"))
    {
        union bg_colorf color;
        parse_color_(&color, line, len);
        struct bg_model_material *mat =  bg_vector_at(struct bg_model_material, &out->materials, out->materials.length-1);
        mat->diffuse = color;
    }
    else if (str_starts_with(line, "Ks"))
    {
        union bg_colorf color;
        parse_color_(&color, line, len);
        struct bg_model_material *mat =  bg_vector_at(struct bg_model_material, &out->materials, out->materials.length-1);
        mat->specular_color = color;
    }
    else if (str_starts_with(line, "Ns"))
    {
        float weight;
        parse_float_(&weight, line, len);
        struct bg_model_material *mat =  bg_vector_at(struct bg_model_material, &out->materials, out->materials.length-1);
        mat->specular_weight = weight;
    }
    else if (str_starts_with(line, "d"))
    {
        float d;
        parse_float_(&d, line, len);
        struct bg_model_material *mat =  bg_vector_at(struct bg_model_material, &out->materials, out->materials.length-1);
        mat->dissolve = d;
    }
    else if (str_starts_with(line, "Tr"))
    {
        float d;
        parse_float_(&d, line, len);
        struct bg_model_material *mat =  bg_vector_at(struct bg_model_material, &out->materials, out->materials.length-1);
        mat->dissolve = 1.0f - d;
    }
    else if (str_starts_with(line, "Tf"))
    {
        union bg_colorf color;
        parse_color_(&color, line, len);
        struct bg_model_material *mat =  bg_vector_at(struct bg_model_material, &out->materials, out->materials.length-1);
        mat->filter_color = color;
    }
    else if (str_starts_with(line, "Ni"))
    {
        float d;
        parse_float_(&d, line, len);
        struct bg_model_material *mat =  bg_vector_at(struct bg_model_material, &out->materials, out->materials.length-1);
        mat->refraction_index = d;
    }
    else if (str_starts_with(line, "illum"))
    {
        int d;
        parse_int_(&d, line, len);
        struct bg_model_material *mat =  bg_vector_at(struct bg_model_material, &out->materials, out->materials.length-1);
        mat->illumination_model = d;
    }
    else if (str_starts_with(line, "map_Ka"))
    {
        char *filename;
        size_t name_len;
        parse_filename_(&filename, &name_len, line, len);
        struct bg_model_material *mat =  bg_vector_at(struct bg_model_material, &out->materials, out->materials.length-1);
        bg_model_material_set_ambient_texture(mat, filename, name_len);
    }
    else if (str_starts_with(line, "map_Ks"))
    {
        char *filename;
        size_t name_len;
        parse_filename_(&filename, &name_len, line, len);
        struct bg_model_material *mat =  bg_vector_at(struct bg_model_material, &out->materials, out->materials.length-1);
        bg_model_material_set_specular_texture(mat, filename, name_len);
    }
    else if (str_starts_with(line, "map_Kd"))
    {
        char *filename;
        size_t name_len;
        parse_filename_(&filename, &name_len, line, len);
        struct bg_model_material *mat =  bg_vector_at(struct bg_model_material, &out->materials, out->materials.length-1);
        bg_model_material_set_diffuse_texture(mat, filename, name_len);
    }
    else if (str_starts_with(line, "map_Ns"))
    {
        char *filename;
        size_t name_len;
        parse_filename_(&filename, &name_len, line, len);
        struct bg_model_material *mat =  bg_vector_at(struct bg_model_material, &out->materials, out->materials.length-1);
        bg_model_material_set_highlight_texture(mat, filename, name_len);
    }
    else if (str_starts_with(line, "map_d"))
    {
        char *filename;
        size_t name_len;
        parse_filename_(&filename, &name_len, line, len);
        struct bg_model_material *mat =  bg_vector_at(struct bg_model_material, &out->materials, out->materials.length-1);
        bg_model_material_set_alpha_texture(mat, filename, name_len);
    }
    else if (str_starts_with(line, "map_bump") || str_starts_with(line, "bump"))
    {
        char *filename;
        size_t name_len;
        parse_filename_(&filename, &name_len, line, len);
        struct bg_model_material *mat =  bg_vector_at(struct bg_model_material, &out->materials, out->materials.length-1);
        bg_model_material_set_bump_texture(mat, filename, name_len);
    }
    else if (str_starts_with(line, "disp"))
    {
        char *filename;
        size_t name_len;
        parse_filename_(&filename, &name_len, line, len);
        struct bg_model_material *mat =  bg_vector_at(struct bg_model_material, &out->materials, out->materials.length-1);
        bg_model_material_set_displacement_texture(mat, filename, name_len);
    }
    else if (str_starts_with(line, "decal"))
    {
        char *filename;
        size_t name_len;
        parse_filename_(&filename, &name_len, line, len);
        struct bg_model_material *mat =  bg_vector_at(struct bg_model_material, &out->materials, out->materials.length-1);
        bg_model_material_set_decal_texture(mat, filename, name_len);
    }
    // TODO: add pbr attributes here.
    else
    {
        bg_panic("Invalid .mtl file syntax:\n  %.*s", (int) len, line);
    }
}

void
bg_parse_mtllib(struct bg_mtllib *out,  char *stream, char *name, size_t name_len)
{
    bg_mtllib(out, name, name_len);

    while (stream[0])
    {
        int line_len = str_find_next('\n', stream);
        parse_material_line_(out, stream, line_len);
        stream = str_next_line(stream);
    }

    bg_vector_shrink(&out->materials);
}

void
bg_load_mtllib(struct bg_mtllib *out, char *file_name)
{
    char *file_data = NULL;
    bg_read_file(&file_data, file_name);
    bg_parse_mtllib(out, file_data, file_name, strlen(file_name));
}