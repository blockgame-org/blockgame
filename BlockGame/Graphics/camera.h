// NOTE: It's probably a better idea to
// compute the orientation matrix in an
// update method and save it for re-use.

#ifndef BLOCKGAME_CAMERA_H
#define BLOCKGAME_CAMERA_H

#include <Basic/mathtypes.h>

// Maximum vertical degrees to avoid gimbal lock
#define CAMERA_MAX_VERTICAL_DEGREES 85.

enum bgCameraDirection {
    BG_CAMERA_LEFT,
    BG_CAMERA_RIGHT,
    BG_CAMERA_UP,
    BG_CAMERA_DOWN,
    BG_CAMERA_FORWARD,
    BG_CAMERA_BACKWARD
};

typedef struct {
    float fovDegrees;
    float aspect;
    float near;
    float far;

    float vertical;
    float horizontal;

    float speed;

    bgVec3f position;
} bgCamera;

void bg_camera(bgCamera *camera, float aspect);
void bgCamera_lookAt(bgCamera *camera, bgVec3f v);
void bgCamera_move(bgCamera *camera, enum bgCameraDirection direction);
void bgCamera_mouse(bgCamera *camera);

void bgCamera_view(bgCamera const *camera, bgMat4 out);
void bgCamera_projection(bgCamera const *camera, bgMat4 out);
void bgCamera_orientation(bgCamera const *camera, bgMat4 out);

#endif // BLOCKGAME_CAMERA_H