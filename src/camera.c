#include <blockgame/camera.h>

#include <blockgame/math.h>
#include <blockgame/mouse.h>

#include <string.h>

static void normalizeAngles_(bgCamera *camera) {
    camera->horizontal = fmodf(camera->horizontal, 360.);

    if (camera->horizontal < 0.)
        camera->horizontal += 360.;

    if (camera->vertical > CAMERA_MAX_VERTICAL_DEGREES)
        camera->vertical = CAMERA_MAX_VERTICAL_DEGREES;
    else if (camera->vertical < -CAMERA_MAX_VERTICAL_DEGREES)
        camera->vertical = -CAMERA_MAX_VERTICAL_DEGREES;
}

void bg_camera(bgCamera *camera, float aspect) {
    camera->fovDegrees = 90.f;
    camera->aspect = aspect;
    camera->near = .01;
    camera->far = 1000.;

    camera->horizontal = 0.;
    camera->vertical = 0.;

    camera->speed = .05;

    bgVec3f_zero(camera->position);
}

void bgCamera_lookAt(bgCamera *camera, bgVec3f v) {
    bgVec3f dir;
    bgVec3f_sub(dir, v, camera->position);
    bgVec3f_normalize(dir, dir);

    camera->horizontal = -bg_degToRad(atan2f(-dir[0], -dir[2]));
    camera->vertical = bg_degToRad(asinf(-dir[1]));
    normalizeAngles_(camera);
}

void bgCamera_move(bgCamera *camera, enum bgCameraDirection direction) {
    // TODO: Implement camera movement
    /*
    bgVec4f dv = {0, 0, 0, 0};
    bgMat4 orient;

    bgMat4_identity(orient);
    bgCamera_orientation(camera, orient);
    bgMat4_invert(orient, orient);

    // Calculate dv

    bgVec4f_scale(dv, dv, camera->speed);
    bgVec3f_add(camera->position, camera->position, dv);
    */
}

void bgCamera_mouse(bgCamera *camera) {
    // NOTE: We probably dont have to use
    // `bgMouse_getRelativePosition` anymore
    // because this doesnt rely on delta positions

    bgVec2i mousePosition = {0, 0};
    bgMouse_getRelativePosition(mousePosition);

    camera->horizontal += 0.05 * (float)mousePosition[0];
    camera->vertical += 0.05 * (float)mousePosition[1];
    normalizeAngles_(camera);
}

void bgCamera_view(bgCamera const *camera, bgMat4 out) {
    bgVec3f invPos;
    bgMat4 trans;
    bgMat4 orient;

    bgVec3f_invert(invPos, camera->position);
    bgMat4_identity(trans);
    bgMat4_identity(orient);

    bgCamera_orientation(camera, orient);
    bgMat4_translate(trans, invPos);

    bgMat4_mul(out, orient, trans);
}

void bgCamera_projection(bgCamera const *camera, bgMat4 out) {
    bgMat4_perspective(out, bg_degToRad(camera->fovDegrees), camera->aspect,
                       camera->near, camera->far);
}

void bgCamera_orientation(bgCamera const *camera, bgMat4 out) {
    bgMat4_rotateX(out, out, bg_degToRad(camera->vertical));
    bgMat4_rotateY(out, out, bg_degToRad(camera->horizontal));
}
