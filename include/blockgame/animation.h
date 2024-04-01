#ifndef BG_ANIMATION_H
#define BG_ANIMATION_H

#include "stdint.h"
#include <blockgame/mathtypes.h>
#include <blockgame/vector.h>

enum bgAnimationInterpMode {
    BG_ANIM_LINEAR = 0,
    BG_ANIM_BEZIER = 1,
    BG_ANIM_CATMULLROM = 2
};

// silly bitflags used here. Honestly pretty epic
enum bgAnimationChannel {
    BG_ANIM_NONE = 0,

    BG_ANIM_RX = 1,
    BG_ANIM_RY = 2,
    BG_ANIM_RZ = 4,
    BG_ANIM_R = BG_ANIM_RX | BG_ANIM_RY | BG_ANIM_RZ,

    BG_ANIM_TX = 8,
    BG_ANIM_TY = 16,
    BG_ANIM_TZ = 32,
    BG_ANIM_T = BG_ANIM_TX | BG_ANIM_TY | BG_ANIM_TZ,

    BG_ANIM_SX = 64,
    BG_ANIM_SY = 128,
    BG_ANIM_SZ = 256,
    BG_ANIM_S = BG_ANIM_SX | BG_ANIM_SY | BG_ANIM_SZ,

    BG_ANIM_ALL = BG_ANIM_R | BG_ANIM_T | BG_ANIM_S
};

uint8_t bgAnimation_getChannelIndice(enum bgAnimationChannel channel);

typedef struct {
    float left_value, left_time;
    float right_value, right_time;
} bgAnimationBezier;

void bgAnimation_bezier(bgAnimationBezier *out, float lv, float lt, float rv,
                        float rt);

typedef struct {
    float timestamp;

    bgVec3f rotation;
    bgVec3f translation;
    bgVec3f scale;

    bgVec3f pivot;
    enum bgAnimationInterpMode interp_mode[9]; // indices for each channel
    int interp_bezier_ind[9];                  // indices for each channel
} bgAnimationKeyframe;

void bgAnimation_keyframe(bgAnimationKeyframe *out, float timestamp);

void bgAnimationKeyframe_applyImode(bgAnimationKeyframe *keyframe,
                                    enum bgAnimationChannel channel,
                                    enum bgAnimationInterpMode mode);

enum bgAnimationInterpMode
bgAnimationKeyframe_getImode(bgAnimationKeyframe *keyframe,
                             enum bgAnimationChannel channel);

void bgAnimationKeyframe_applyBezierIndice(bgAnimationKeyframe *keyframe,
                                           enum bgAnimationChannel channel,
                                           int bezier_indice);

int bgAnimationKeyframe_getBezierIndice(bgAnimationKeyframe *keyframe,
                                        enum bgAnimationChannel channel);

typedef struct {
    bgVector keyframes;
    bgVector keyframe_beziers;
    char *name;
} bgAnimationObject;

void bgAnimation_object(bgAnimationObject *obj, char *name, size_t name_len);

void bgAnimationObject_free(bgAnimationObject *obj);

void bgAnimationObject_cleanup(void *obj);

void bgAnimationKeyframe_getBezier(bgAnimationBezier *out,
                                   bgAnimationObject const *obj,
                                   bgAnimationKeyframe const *keyframe,
                                   enum bgAnimationChannel channel);

enum bgAnimationRepeatMode {
    BG_ANIM_ONCE = 0,
    BG_ANIM_REPEAT = 1,
    BG_ANIM_HOLD = 2
};

typedef struct {
    float duration;
    enum bgAnimationRepeatMode repeat_mode;

    bgVector objects;
} bgAnimation;

void bg_animation(bgAnimation *out, float duration,
                  enum bgAnimationRepeatMode repeat_mode);

void bgAnimation_free(bgAnimation *anim);

void bgAnimation_parse(bgAnimation *out, char *stream);

void bgAnimation_load(bgAnimation *out, char *file_name);

#endif // BG_ANIMATION_H