#ifndef BG_ANIMATOR_H
#define BG_ANIMATOR_H

#include "stdlib.h"
#include <blockgame/animation.h>
#include <blockgame/mathtypes.h>
#include <blockgame/model.h>
#include <blockgame/vector.h>

#define ANIMATOR_EPSILON (1.0 / 1200.0)

typedef struct {
    float time;
    bgVec3f transformation;
    enum bgAnimationInterpMode interp_mode[3];
    int bezier_indices[3];
} bgAnimatorKeyframe;

typedef struct {
    bgVector keyframes; // bgAnimationKeyframe *[]
    bgObject *obj;
    bgAnimationObject *anim_obj;
    enum bgAnimationChannel channel;

} bgAnimatorTimeline;

void bgAnimator_timeline(bgAnimatorTimeline *out, bgAnimation *anim,
                         bgObject *obj, enum bgAnimationChannel channel);

void bgAnimatorTimeline_free(bgAnimatorTimeline *timeline);

void bgAnimatorTimeline_cleanup(void *timeline);

typedef struct {
    bgAnimation const *animation;
    bgModel const *model;

    bgVector timelines; // timelines for each object
    float current_time; // time in seconds
} bgAnimator;

void bg_animator(bgAnimator *out, bgAnimation *anim, bgModel *model);

void bgAnimator_free(bgAnimator *animator);

void bgAnimator_step(bgAnimator *animator, float step);

// get a transformation matrix at the current time step
void bgAnimator_getTransformationMatrix(bgMat4 *out, bgAnimator *animator,
                                        bgAnimatorTimeline *obj);

void bgAnimator_getChannelValue(bgVec3f *out, bgAnimatorTimeline *timeline,
                                bgAnimator *animator);

void bgAnimator_getChannelValues(bgVec3f *rot_out, bgVec3f *trans_out,
                                 bgVec3f *scale_out,
                                 bgAnimatorTimeline *rot_timeline,
                                 bgAnimatorTimeline *trans_timeline,
                                 bgAnimatorTimeline *scale_timeline,
                                 bgAnimator *animator);

void bgAnimator_getCurrentKeyframe(bgAnimatorKeyframe *out,
                                   bgAnimatorTimeline *timeline, float time);

void bgAnimator_getNextKeyframe(bgAnimatorKeyframe *out,
                                bgAnimatorTimeline *timeline, float time);

#endif // BG_ANIMATOR_H