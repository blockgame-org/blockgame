#ifndef BG_ANIMATION_H
#define BG_ANIMATION_H

#include <blockgame/mathtypes.h>
#include <blockgame/vector.h>


enum bg_animation_interp_mode
{
    LINEAR = 0,
    BEZIER = 1,
    CATMULLROM = 2
};


// silly bitflags used here. Honestly pretty epic
enum bg_animation_channel
{
    NONE = 0,

    RX = 1,
    RY = 2,
    RZ = 4,
    R = RX | RY | RZ,

    TX = 8,
    TY = 16,
    TZ = 32,
    T = TX | TY | TZ,

    SX = 64,
    SY = 128,
    SZ = 256,
    S = SX | SY | SZ,

    ALL = R | T | S
};


struct bg_animation_bezier
{
    enum bg_animation_channel channel;
    float left_value, left_time;
    float right_value, right_time;
};


struct bg_animation_keyframe
{
    float timestamp;

    bg_vec3f rotation;
    bg_vec3f translation;
    bg_vec3f scale;

    bg_vec3f pivot;
    enum bg_animation_interp_mode interp_mode[9]; // indices for each channel
    size_t interp_bezier_ind[9]; // indices for each channel
};

void
bg_animation_keyframe(struct bg_animation_keyframe *out, float timestamp);

void
bg_animation_keyframe_apply_imode(struct bg_animation_keyframe *out, float timestamp);

void
bg_animation_keyframe_get_bezier(struct bg_animation_bezier *out, struct bg_animation_object const *obj,
                                 struct bg_animation_keyframe const *keyframe, enum bg_animation_channel);


struct bg_animation_object
{
    struct bg_vector keyframes;
    struct bg_vector keyframe_beziers;
    char *name;
};


enum bg_animation_repeat_mode
{
    ONCE = 0,
    REPEAT = 1,
    HOLD = 2
};


struct bg_animation
{
    enum bg_animation_repeat_mode repeat_mode;
    float duration;
};

#endif // BG_ANIMATION_H