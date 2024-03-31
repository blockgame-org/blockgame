#include <blockgame/camera.h>

#include <blockgame/log.h>
#include <blockgame/math.h>

#include <string.h>

void bg_camera(bgCamera *out) {
  memset(out, 0, sizeof(bgCamera));

  out->aspect = 0.;
  out->fov = bg_degToRad(45);
  out->near = .1;
  out->far = 1000.;

  out->pitch = 0.;
  out->heading = 0.;

  out->up[0] = 0.;
  out->up[1] = 1.;
  out->up[2] = 0.;

  out->position[0] = 0.;
  out->position[1] = 0.;
  out->position[2] = 0.;

  out->lookAt[0] = 0.;
  out->lookAt[1] = 0.;
  out->lookAt[2] = 0.;

  out->direction[1] = 0.;
  out->direction[2] = 0.;
  out->direction[3] = 0.;

  bgMat4_identity(out->model);
  bgMat4_identity(out->view);
  bgMat4_identity(out->projection);
}

void bgCamera_step(bgCamera *cam, float dt) {
  bgVec3f_sub(cam->direction, cam->lookAt, cam->direction);
  bgVec3f_normalize(cam->direction, cam->direction);

  // TODO: Set viewport?

  bgMat4_perspective(cam->projection, cam->fov, cam->aspect, cam->near,
                     cam->far);

  bgVec3f axis = {0., 0., 0.};
  bgVec3f_cross(axis, cam->direction, cam->up);

  bgQuat pitch = {0., 0., 0., 0.};
  bgQuat_angleAxis(pitch, cam->pitch, axis);

  bgQuat heading = {0., 0., 0., 0.};
  bgQuat_angleAxis(heading, cam->heading, cam->up);

  bgQuat rot = {0., 0., 0., 0.};
  bgQuat_cross(rot, pitch, heading);
  bgQuat_normalize(rot, rot);

  bgQuat_rotate(cam->direction, rot, cam->direction);

  bgVec3f_add(cam->lookAt, cam->position, cam->direction);

  bgMat4_lookAt(cam->view, cam->position, cam->lookAt, cam->up);
  
  //bgMat4_identity(cam->model);
  // NOTE: This is temporary
  bgMat4_translate(cam->model, (bgVec3f){.5 * 10, 1. * 10, .5 * 10});
  bgMat4_scale(cam->model, cam->model, 5.);
}