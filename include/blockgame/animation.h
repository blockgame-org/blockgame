#ifndef BG_ANIMATION_H
#define BG_ANIMATION_H

#include <blockgame/mathtypes.h>
#include <blockgame/vector.h>

enum bgAnimationInterpMode { LINEAR = 0, BEZIER = 1, CATMULLROM = 2 };

// silly bitflags used here. Honestly pretty epic
enum bgAnimationChannel {
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

typedef struct {
  enum bgAnimationChannel channel;
  float left_value, left_time;
  float right_value, right_time;
} bgAnimationBezier;

typedef struct {
  float timestamp;

  bgVec3f rotation;
  bgVec3f translation;
  bgVec3f scale;

  bgVec3f pivot;
  enum bgAnimationInterpMode interp_mode[9]; // indices for each channel
  size_t interp_bezier_ind[9];                  // indices for each channel
} bgAnimationKeyframe;

void
bgAnimation_keyframe(bgAnimationKeyframe *out, float timestamp);

void
bgAnimationKeyframe_applyImode(bgAnimationKeyframe *out);

void bgAnimationKeyframe_getBezier(
    bgAnimationBezier *out, bgAnimationObject const *obj,
    bgAnimationKeyframe const *keyframe, enum bgAnimationChannel);

typedef struct {
  bgVector keyframes;
  bgVector keyframe_beziers;
  char *name;
} bgAnimationObject;

enum bgAnimationRepeatMode { ONCE = 0, REPEAT = 1, HOLD = 2 };

typedef struct bgAnimation {
  enum bgAnimationRepeatMode repeat_mode;
  float duration;
};

#endif // BG_ANIMATION_H