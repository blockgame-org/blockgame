// NOTE: Everything in this uses radians
// NOTE: Every output variable should be able to be
//       used as inputs.

#ifndef BLOCKGAME_MATH_H
#define BLOCKGAME_MATH_H

#include <blockgame/mathtypes.h>

#include <math.h>
#include <stddef.h>
#include <stdint.h>

#define BG_PI 3.14159265358979323846264338327950288

#define bg_degToRad(x) ((x) * BG_PI / 180.)

#define bgVec3f_magnitude(v)                                                   \
    (sqrt((v)[0] * (v)[0] + (v)[1] * (v)[1] + (v)[2] * (v)[2]))

static inline void bgVec2i_zero(bgVec2i out) {
    out[0] = 0;
    out[1] = 0;
}

static inline void bgVec2i_sub(bgVec2i out, bgVec2i v1, bgVec2i v2) {
    out[0] = v1[0] - v2[0];
    out[1] = v1[1] - v2[1];
}

static inline void bgVec3f_zero(bgVec3f out) {
    out[0] = 0.;
    out[1] = 0.;
    out[2] = 0.;
}

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

static inline void bgVec3f_invert(bgVec3f out, bgVec3f const v) {
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

static inline void bgVec3f_scale(bgVec3f out, bgVec3f v, float s) {
    out[0] = v[0] * s;
    out[1] = v[1] * s;
    out[2] = v[2] * s;
}

static inline float bgVec3f_dot(bgVec3f v1, bgVec3f v2) {
    return v2[0] * v1[0] + v2[1] * v1[1] + v2[2] * v1[2];
}

static inline float bgVec4f_dot(bgVec4f v1, bgVec4f v2) {
    return v2[0] * v1[0] + v2[1] * v1[1] + v2[2] * v1[2] + v2[3] * v1[3];
}

static inline void bgVec4f_scale(bgVec4f out, bgVec4f v, float s) {
    out[0] = v[0] * s;
    out[1] = v[1] * s;
    out[2] = v[2] * s;
    out[3] = v[3] * s;
}

static inline void bgVec4f_copy(bgVec4f out, bgVec4f v) {
    out[0] = v[0];
    out[1] = v[1];
    out[2] = v[2];
    out[3] = v[3];
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

static inline void bgMat4_translate(bgMat4 out, bgVec3f v) {
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

    out[3][0] = m[3][0];
    out[3][1] = m[3][1];
    out[3][2] = m[3][2];
    out[3][3] = m[3][3];
}

static inline void bgMat4_lookAt(bgMat4 out, bgVec3f eye, bgVec3f center,
                                 bgVec3f up) {
    bgVec3f f = {0., 0., 0.};
    bgVec3f s = {0., 0., 0.};
    bgVec3f u = {0., 0., 0.};

    bgVec3f_sub(f, center, eye);
    bgVec3f_normalize(f, f);

    bgVec3f_cross(s, f, up);
    bgVec3f_normalize(s, s);

    bgVec3f_cross(u, s, f);

    bgMat4_identity(out);

    /*
    out[0][0] = s[0];
    out[1][0] = s[1];
    out[2][0] = s[2];
    out[0][1] = u[0];
    out[1][1] = u[1];
    out[2][1] = u[2];
    out[0][2] = -f[0];
    out[1][2] = -f[1];
    out[2][2] = -f[2];
    out[0][3] = -bgVec3f_dot(s, eye);
    out[1][3] = -bgVec3f_dot(u, eye);
    out[2][3] = bgVec3f_dot(f, eye);
    */

    out[0][0] = s[0];
    out[0][1] = u[0];
    out[0][2] = -f[0];
    out[0][3] = 0.;
    out[1][0] = s[1];
    out[1][1] = u[1];
    out[1][2] = -f[1];
    out[1][3] = 0.;
    out[2][0] = s[2];
    out[2][1] = u[2];
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

static inline void bgMat4_mul_vec4(bgVec4f out, bgMat4 m, bgVec4f v) {
    for (size_t i = 0; i < 4; ++i) {
        out[i] = 0.;
        for (size_t j = 0; j < 4; ++j)
            out[i] += m[j][i] * v[j];
    }
}

static inline void bgMat4_invert(bgMat4 out, bgMat4 m) {
    float s[6];
    float c[6];

    s[0] = m[0][0] * m[1][1] - m[1][0] * m[0][1];
    s[1] = m[0][0] * m[1][2] - m[1][0] * m[0][2];
    s[2] = m[0][0] * m[1][3] - m[1][0] * m[0][3];
    s[3] = m[0][1] * m[1][2] - m[1][1] * m[0][2];
    s[4] = m[0][1] * m[1][3] - m[1][1] * m[0][3];
    s[5] = m[0][2] * m[1][3] - m[1][2] * m[0][3];

    c[0] = m[2][0] * m[3][1] - m[3][0] * m[2][1];
    c[1] = m[2][0] * m[3][2] - m[3][0] * m[2][2];
    c[2] = m[2][0] * m[3][3] - m[3][0] * m[2][3];
    c[3] = m[2][1] * m[3][2] - m[3][1] * m[2][2];
    c[4] = m[2][1] * m[3][3] - m[3][1] * m[2][3];
    c[5] = m[2][2] * m[3][3] - m[3][2] * m[2][3];

    float i = 1. / (s[0] * c[5] - s[1] * c[4] + s[2] * c[3] + s[3] * c[2] -
                    s[4] * c[1] + s[5] * c[0]);

    out[0][0] = (m[1][1] * c[5] - m[1][2] * c[4] + m[1][3] * c[3]) * i;
    out[0][1] = (-m[0][1] * c[5] + m[0][2] * c[4] - m[0][3] * c[3]) * i;
    out[0][2] = (m[3][1] * s[5] - m[3][2] * s[4] + m[3][3] * s[3]) * i;
    out[0][3] = (-m[2][1] * s[5] + m[2][2] * s[4] - m[2][3] * s[3]) * i;

    out[1][0] = (-m[1][0] * c[5] + m[1][2] * c[2] - m[1][3] * c[1]) * i;
    out[1][1] = (m[0][0] * c[5] - m[0][2] * c[2] + m[0][3] * c[1]) * i;
    out[1][2] = (-m[3][0] * s[5] + m[3][2] * s[2] - m[3][3] * s[1]) * i;
    out[1][3] = (m[2][0] * s[5] - m[2][2] * s[2] + m[2][3] * s[1]) * i;

    out[2][0] = (m[1][0] * c[4] - m[1][1] * c[2] + m[1][3] * c[0]) * i;
    out[2][1] = (-m[0][0] * c[4] + m[0][1] * c[2] - m[0][3] * c[0]) * i;
    out[2][2] = (m[3][0] * s[4] - m[3][1] * s[2] + m[3][3] * s[0]) * i;
    out[2][3] = (-m[2][0] * s[4] + m[2][1] * s[2] - m[2][3] * s[0]) * i;

    out[3][0] = (-m[1][0] * c[3] + m[1][1] * c[1] - m[1][2] * c[0]) * i;
    out[3][1] = (m[0][0] * c[3] - m[0][1] * c[1] + m[0][2] * c[0]) * i;
    out[3][2] = (-m[3][0] * s[3] + m[3][1] * s[1] - m[3][2] * s[0]) * i;
    out[3][3] = (m[2][0] * s[3] - m[2][1] * s[1] + m[2][2] * s[0]) * i;
}

static inline void bgMat4_copy(bgMat4 out, bgMat4 m) {
    bgVec4f_copy(out[0], m[0]);
    bgVec4f_copy(out[1], m[1]);
    bgVec4f_copy(out[2], m[2]);
    bgVec4f_copy(out[3], m[3]);
}

static inline void bgMat4_mul(bgMat4 out, bgMat4 m1, bgMat4 m2) {
    bgMat4 t;

    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 4; ++j) {
            t[i][j] = 0.;
            for (size_t k = 0; k < 4; ++k)
                t[i][j] += m1[k][j] * m2[i][k];
        }

    bgMat4_copy(out, t);
}

static inline void bgMat4_rotateX(bgMat4 out, bgMat4 m, float angle) {
    float s = sinf(angle);
    float c = cosf(angle);

    bgMat4 r;
    r[0][0] = 1.;
    r[0][1] = 0.;
    r[0][2] = 0.;
    r[0][3] = 0.;
    r[1][0] = 0.;
    r[1][1] = c;
    r[1][2] = s;
    r[1][3] = 0.;
    r[2][0] = 0.;
    r[2][1] = -s;
    r[2][2] = c;
    r[2][3] = 0.;
    r[3][0] = 0.;
    r[3][1] = 0.;
    r[3][2] = 0.;
    r[3][3] = 1.;

    bgMat4_mul(out, m, r);
}

static inline void bgMat4_rotateY(bgMat4 out, bgMat4 m, float angle) {
    float s = sinf(angle);
    float c = cosf(angle);

    bgMat4 r;
    r[0][0] = c;
    r[0][1] = 0.;
    r[0][2] = -s;
    r[0][3] = 0.;
    r[1][0] = 0.;
    r[1][1] = 1.;
    r[1][2] = 0.;
    r[1][3] = 0.;
    r[2][0] = s;
    r[2][1] = 0.;
    r[2][2] = c;
    r[2][3] = 0.;
    r[3][0] = 0.;
    r[3][1] = 0.;
    r[3][2] = 0.;
    r[3][3] = 1.;

    bgMat4_mul(out, m, r);
}

static inline void bgQuat_angleAxis(bgQuat out, float angle, bgVec3f n) {
    float s = sinf(angle * .5);

    out[0] = n[0] * s;
    out[1] = n[1] * s;
    out[2] = n[2] * s;
    out[3] = cosf(angle * .5);
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

    /*
    bgVec3f t = {q[0], q[1], q[2]};

    bgVec3f uv = {0., 0., 0.};
    bgVec3f_cross(uv, t, v);

    bgVec3f uuv = {0., 0., 0.};
    bgVec3f_cross(uuv, t, uv);

    out[0] = v[0] + ((uv[0] * q[3]) + uuv[0]) * 2.;
    out[1] = v[1] + ((uv[1] * q[3]) + uuv[1]) * 2.;
    out[2] = v[2] + ((uv[2] * q[3]) + uuv[2]) * 2.;
    */

    bgVec3f t1 = {0., 0., 0.};
    bgVec3f_cross(t1, q, v);
    bgVec3f_scale(t1, t1, 2.);

    bgVec3f t2 = {0., 0., 0.};
    bgVec3f_cross(t2, q, t1);

    out[0] = v[0] + q[3] * t1[0] + t2[0];
    out[1] = v[1] + q[3] * t1[1] + t2[1];
    out[2] = v[2] + q[3] * t1[2] + t2[2];
}

// t is between 0 and 1
static inline float catmull_rom_interp(float t, float v0, float v1, float v2, float v3) {
    float c1 = 1. * v1;
    float c2 = -.5 * v0 + 0.5 * v2;
    float c3 = 1.0 * v0 + -2.5 * v1 + 2.0 * v2 - .5 * v3;
    float c4 = -.5 * v0 + 1.5 * v1 + -1.5 * v2 + .5 * v3;
    return (((c4 * t + c3) * t + c2) * t + c1);
}

// t is between 0 and 1
static inline float lerp(float t, float v0, float v1) { return v0 + (v1 - v0) * t; }

// * https://blog.maximeheckel.com/posts/cubic-bezier-from-math-to-motion/
// p0 is the left control point
// p3 is the right control point
// t is between 0 and 1
static inline float bezier_interp(float t, float v0, float v1, float v2, float v3) {
    return pow(1. - t, 3) * v0 +
           3 * pow(1. - t, 2) * t * v1 +
           3 * (1. - t) * pow(t, 2) * v2 +
           pow(t, 3) * v3;
}

#endif // BLOCKGAME_MATH_H