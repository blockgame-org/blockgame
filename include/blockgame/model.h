#ifndef BG_MODEL_H
#define BG_MODEL_H

#include <blockgame/vector.h>

// QUESTION FOR WHEN I GET BACK
// SHOULD BE CREATE A "SHRINK" / "FIT EXACT" function to vectors to remove
// excess space.

typedef struct {
    bgVector vertices; // indices as a uint32_t
    bgVector normals;  // indices as a uint32_t
    bgVector uvs;      // indices as a uint32_t
} bgObjectFace;

void bg_objectFace(bgObjectFace *out);

void bgObjectFace_free(bgObjectFace *out);

void bgObjectFace_cleanup(void *out);

void bgObjectFace_copy(void *out, void *src);

typedef union {
    struct {
        float x, y, z;
    };
    float values[3];
} bgObjectVertex;

void bg_objectVertex(bgObjectVertex *out, float x, float y, float z);

typedef union {
    struct {
        float x, y;
    };
    float values[2];
} bgObjectUV;

void bg_objectUV(bgObjectUV *out, float x, float y);

typedef struct {
    bgVector vertices; // bg_object_vertex vector
    bgVector normals;  // bg_object_vertex vector
    bgVector uvs;      // bg_object_uv vector
    bgVector faces;    // bg_object_face vector

    char *object_name;
    char *group_name;
    char *material_name;
} bgObject;

void bg_object(bgObject *object, char *name, size_t name_len);

void bgObject_setMaterial(bgObject *object, char *name, size_t name_len);

void bgObject_setGroup(bgObject *object, char *name, size_t name_len);

void bgObject_shrink(bgObject *object);

void bgObject_free(bgObject *object);

void bgObject_cleanup(void *object);

void bgObject_copy(void *out, void *src);

typedef struct {
    bgVector objects;

    char *mtllib_name;
} bgModel;

void bg_model(bgModel *model);

void bgModel_setMtllib(bgModel *model, char *name, size_t name_len);

void bgModel_free(bgModel *model);

void bgModel_parse(bgModel *out, char *stream);

void bgModel_load(bgModel *out, char *file_name);

void bgModel_copy(void *out, void *src);

#endif // BG_MODEL_H