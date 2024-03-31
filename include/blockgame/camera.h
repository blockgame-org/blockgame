#ifndef BLOCKGAME_CAMERA_H
#define BLOCKGAME_CAMERA_H

#include <blockgame/mathtypes.h>

typedef struct {
  bgVec4u viewport;

  float aspect;
  float fov;
  float near;
  float far;

  float pitch;
  float heading;

  bgVec3f up;

  bgVec3f position;
  bgVec3f lookAt;
  bgVec3f direction;

  bgMat4 model;
  bgMat4 view;
  bgMat4 projection;
} bgCamera;

void bg_camera(bgCamera *out);

void bgCamera_step(bgCamera *cam, float dt);

#endif // BLOCKGAME_CAMERA_H