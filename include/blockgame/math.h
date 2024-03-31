// NOTE: Everything in this uses radians
// NOTE: Every output variable should be able to be
//       used as inputs.

#ifndef BLOCKGAME_MATH_H
#define BLOCKGAME_MATH_H

#include <blockgame/mathtypes.h>

#include <math.h>
#include <stdint.h>

#define BG_PI 3.14159265358979323846264338327950288

#define bg_degToRad(x) ((x) * BG_PI / 180.)

#define bgVec3f_magnitude(v)                                                   \
  (sqrt((v)[0] * (v)[0] + (v)[1] * (v)[1] + (v)[2] * (v)[2]))

static inline void bgVec3f_sub(bgVec3f out, bgVec3f v1, bgVec3f v2) {
  out[0] = v1[0] - v2[0];
  out[1] = v1[1] - v2[1];
  out[2] = v1[2] - v2[2];
}

static inline void bgVec3f_add(bgVec3f out, bgVec3f v1, bgVec3f v2) {
  out[0] = v1[0] + v2[0];
  out[1] = v1[1] + v2[1];
  out[2] = v1[2] + v2[2];
}

static inline void bgVec3f_invert(bgVec3f out, bgVec3f v) {
  out[0] = -v[0];
  out[1] = -v[1];
  out[2] = -v[2];
}

static inline void bgVec3f_normalize(bgVec3f out, bgVec3f v) {
  float mag = bgVec3f_magnitude(v);

  out[0] = v[0] / mag;
  out[1] = v[1] / mag;
  out[2] = v[2] / mag;
}

static inline void bgVec3f_cross(bgVec3f out, bgVec3f v, bgVec3f n) {
  float x = v[1] * n[2] - n[1] * v[2];
  float y = v[2] * n[0] - n[2] * v[0];
  float z = v[0] * n[1] - n[0] * v[1];

  out[0] = x;
  out[1] = y;
  out[2] = z;
}

static inline float bgVec4f_dot(bgVec4f v1, bgVec4f v2) {
  return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2] + v1[3] * v2[3];
}

static inline void bgVec4f_scale(bgVec4f out, bgVec4f v, float s) {
  out[0] = v[0] * s;
  out[1] = v[1] * s;
  out[2] = v[2] * s;
  out[3] = v[3] * s;
}

static inline void bgMat4_zero(bgMat4 out) {
  out[0][0] = 0.;
  out[0][1] = 0.;
  out[0][2] = 0.;
  out[0][3] = 0.;

  out[1][0] = 0.;
  out[1][1] = 0.;
  out[1][2] = 0.;
  out[1][3] = 0.;

  out[2][0] = 0.;
  out[2][1] = 0.;
  out[2][2] = 0.;
  out[2][3] = 0.;

  out[3][0] = 0.;
  out[3][1] = 0.;
  out[3][2] = 0.;
  out[3][3] = 0.;
}

static inline void bgMat4_identity(bgMat4 out) {
  out[0][0] = 1.;
  out[0][1] = 0.;
  out[0][2] = 0.;
  out[0][3] = 0.;

  out[1][0] = 0.;
  out[1][1] = 1.;
  out[1][2] = 0.;
  out[1][3] = 0.;

  out[2][0] = 0.;
  out[2][1] = 0.;
  out[2][2] = 1.;
  out[2][3] = 0.;

  out[3][0] = 0.;
  out[3][1] = 0.;
  out[3][2] = 0.;
  out[3][3] = 1.;
}

static inline void bgMat4_row(bgVec4f out, bgMat4 m, size_t ix) {
  out[0] = m[0][ix];
  out[1] = m[1][ix];
  out[2] = m[2][ix];
  out[3] = m[3][ix];
}

static inline void bgMat4_transpose(bgMat4 out, bgMat4 m) {
  for (size_t j = 0; j < 4; ++j)
    for (size_t i = 0; i < 4; ++i)
      out[i][j] = m[j][i];
}

static inline void bgMat4_translate(bgMat4 out, bgVec3f v)
{
  bgMat4_identity(out);

  out[3][0] = v[0];
  out[3][1] = v[1];
  out[3][2] = v[2];
}

static inline void bgMat4_translateInPlace(bgMat4 inout, bgVec3f v) {
  bgVec4f t = {v[0], v[1], v[2], 0.};
  bgVec4f r = {0., 0., 0., 0.};

  bgMat4_row(r, inout, 0);
  inout[3][0] += bgVec4f_dot(r, t);

  bgMat4_row(r, inout, 1);
  inout[3][1] += bgVec4f_dot(r, t);

  bgMat4_row(r, inout, 2);
  inout[3][2] += bgVec4f_dot(r, t);

  bgMat4_row(r, inout, 3);
  inout[3][3] += bgVec4f_dot(r, t);
}

static inline void bgMat4_perspective(bgMat4 out, float yfov, float aspect,
                                      float near, float far) {
  bgMat4_zero(out);

  float x = tan(yfov / 2.);

  out[0][0] = 1. / (aspect * x);
  out[1][1] = 1. / x;
  out[2][2] = -(far + near) / (far - near);
  out[2][3] = -1.;
  out[3][2] = -(2. * far * near) / (far - near);
}

static inline void bgMat4_scale(bgMat4 out, bgMat4 m, float s) {
  bgVec4f_scale(out[0], m[0], s);
  bgVec4f_scale(out[1], m[1], s);
  bgVec4f_scale(out[2], m[2], s);
  bgVec4f_scale(out[3], m[3], s);
}

static inline void bgMat4_lookAt(bgMat4 out, bgVec3f eye, bgVec3f center,
                                 bgVec3f up) {
  bgVec3f f = {0., 0., 0.};
  bgVec3f s = {0., 0., 0.};
  bgVec3f t = {0., 0., 0.};

  bgVec3f_sub(f, center, eye);
  bgVec3f_normalize(f, f);

  bgVec3f_cross(s, f, up);
  bgVec3f_normalize(s, s);

  bgVec3f_cross(t, s, f);

  out[0][0] = s[0];
  out[0][1] = t[0];
  out[0][2] = -f[0];
  out[0][3] = 0.;
  out[1][0] = s[1];
  out[1][1] = t[1];
  out[1][2] = -f[1];
  out[1][3] = 0.;
  out[2][0] = s[2];
  out[2][1] = t[2];
  out[2][2] = -f[2];
  out[2][3] = 0.;
  out[3][0] = 0.;
  out[3][1] = 0.;
  out[3][2] = 0.;
  out[3][3] = 1.;

  bgVec3f eyeInv = {0., 0., 0.};
  bgVec3f_invert(eyeInv, eye);

  bgMat4_translateInPlace(out, eyeInv);
}

static inline void bgQuat_angleAxis(bgQuat out, float angle, bgVec3f n) {
  out[0] = n[0] * sin(angle / 2);
  out[1] = n[1] * sin(angle / 2);
  out[2] = n[2] * sin(angle / 2);
  out[3] = cos(angle / 2);
}

static inline float bgQuat_dot(bgQuat q1, bgQuat q2) {
  return q1[0] * q2[0] + q1[1] * q2[1] + q1[2] * q2[2] + q1[3] * q2[3];
}

static inline float bgQuat_length(bgQuat q) { return sqrtf(bgQuat_dot(q, q)); }

static inline void bgQuat_cross(bgQuat out, bgQuat q1, bgQuat q2) {
  float x = q1[3] * q2[0] + q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1];
  float y = q1[3] * q2[1] + q1[1] * q2[3] + q1[2] * q2[0] - q1[0] * q2[2];
  float z = q1[3] * q2[2] + q1[2] * q2[3] + q1[0] * q2[1] - q1[1] * q2[0];
  float w = q1[3] * q2[3] - q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2];

  out[0] = x;
  out[1] = y;
  out[2] = z;
  out[3] = w;
}

static inline void bgQuat_normalize(bgQuat out, bgQuat q) {
  float x = 1. / bgQuat_length(q);

  out[0] = q[0] * x;
  out[1] = q[1] * x;
  out[2] = q[2] * x;
  out[3] = q[3] * x;
}

static inline void bgQuat_conjugate(bgQuat out, bgQuat q) {
  out[0] = -q[0];
  out[1] = -q[1];
  out[2] = -q[2];
  out[3] = q[3];
}

static inline void bgQuat_rotate(bgVec3f out, bgQuat q, bgVec3f v) {
  // https://blog.molecular-matters.com/2013/05/24/a-faster-quaternion-vector-multiplication/

  bgVec3f t = {q[0], q[1], q[2]};

  bgVec3f uv = {0., 0., 0.};
  bgVec3f_cross(uv, t, v);

  bgVec3f uuv = {0., 0., 0.};
  bgVec3f_cross(uuv, t, uv);

  out[0] = v[0] + ((uv[0] * q[3]) + uuv[0]) * 2.;
  out[1] = v[1] + ((uv[1] * q[3]) + uuv[1]) * 2.;
  out[2] = v[2] + ((uv[2] * q[3]) + uuv[2]) * 2.;

  /*
    bgVec3f t1 = {0.,0.,0.};
    bgVec3f_cross(t1, q, v);

    t1[0] = 2 * t1[0];
    t1[1] = 2 * t1[1];
    t1[2] = 2 * t1[2];

    bgVec3f t2 = {0., 0., 0.};
    bgVec3f_cross(t2, q, t1);

    out[0] = v[0] + q[3] * t1[0] + t2[0];
    out[1] = v[1] + q[3] * t1[1] + t2[1];
    out[2] = v[2] + q[3] * t1[2] + t2[2];
  */
}

#endif // BLOCKGAME_MATH_H