#ifndef BLOCKGAME_CAMERA_H
#define BLOCKGAME_CAMERA_H

#include <blockgame/mathtypes.h>

enum bgCameraMoveDirection {
    BG_CAMERA_MOVE_UP,
    BG_CAMERA_MOVE_DOWN,
    BG_CAMERA_MOVE_LEFT,
    BG_CAMERA_MOVE_RIGHT,
    BG_CAMERA_MOVE_FORWARD,
    BG_CAMERA_MOVE_BACKWARD
};

typedef struct {
    bgVec4u viewport;

    float aspect;
    float fov;
    float near;
    float far;

    float pitch;
    float heading;
    float speed;

    bgVec2i oldRelativeMousePosition;

    bgVec3f up;

    bgVec3f position;
    bgVec3f lookAt;
    bgVec3f direction;

    bgMat4 model;
    bgMat4 view;
    bgMat4 projection;
} bgCamera;

void bg_camera(bgCamera *out);

void bgCamera_offsetHeading(bgCamera *cam, float angle);
void bgCamera_offsetPitch(bgCamera *cam, float angle);

void bgCamera_mouse(bgCamera *cam);

void bgCamera_move(bgCamera *cam, enum bgCameraMoveDirection dir);

void bgCamera_step(bgCamera *cam, float dt);

#endif // BLOCKGAME_CAMERA_H