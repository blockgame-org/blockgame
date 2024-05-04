#include <blockgame/animator.h>
#include <blockgame/panic.h>
#include <string.h>;
#include <blockgame/math.h>

void bgAnimator_timeline(bgAnimatorTimeline *out, bgAnimation *anim,
                         bgObject *obj, enum bgAnimationChannel channel) {
    out->obj = obj;
    bg_vector(&out->keyframes, sizeof(bgAnimationKeyframe *));

    // get animation object
    bgAnimationObject *anim_obj;
    for (int i = 0; i < anim->objects.length; i++) {
        anim_obj = bgVector_at(bgAnimationObject, &anim->objects, i);
        if (strcmp(anim_obj->name, obj->object_name))
            break;
    }
    out->anim_obj = anim_obj;
    out->channel = channel;

    for (int i = 0; i < anim_obj->keyframes.length; i++) {
        bgAnimatorKeyframe animator_keyframe;
        bgAnimationKeyframe keyframe =
            *bgVector_at(bgAnimationKeyframe, &anim_obj->keyframes, i);
        animator_keyframe.time = keyframe.timestamp;
        switch (channel) {
        case BG_ANIM_R:
            if (keyframe.interp_mode[BG_ANIM_RX] != BG_ANIM_NOCHANGE) {
                memcpy(animator_keyframe.transformation, keyframe.rotation,
                       sizeof(bgVec3f));
                int indice =
                    bgAnimationKeyframe_getBezierIndice(&keyframe, BG_ANIM_RX);
                memcpy(animator_keyframe.interp_mode,
                       keyframe.interp_mode[indice], sizeof(bgVec3f));
                memcpy(animator_keyframe.bezier_indices,
                       keyframe.interp_bezier_ind[indice], sizeof(bgVec3f));
            }
            break;
        case BG_ANIM_T:
            if (keyframe.interp_mode[BG_ANIM_TX] != BG_ANIM_NOCHANGE) {
                memcpy(animator_keyframe.transformation, keyframe.translation,
                       sizeof(bgVec3f));
                int indice =
                    bgAnimationKeyframe_getBezierIndice(&keyframe, BG_ANIM_TX);
                memcpy(animator_keyframe.interp_mode,
                       keyframe.interp_mode[indice], sizeof(bgVec3f));
                memcpy(animator_keyframe.bezier_indices,
                       keyframe.interp_bezier_ind[indice], sizeof(bgVec3f));
            }
            break;
        case BG_ANIM_S:
            if (keyframe.interp_mode[BG_ANIM_SX] != BG_ANIM_NOCHANGE) {
                memcpy(animator_keyframe.transformation, keyframe.scale,
                       sizeof(bgVec3f));
                int indice =
                    bgAnimationKeyframe_getBezierIndice(&keyframe, BG_ANIM_SX);
                memcpy(animator_keyframe.interp_mode,
                       keyframe.interp_mode[indice], sizeof(bgVec3f));
                memcpy(animator_keyframe.bezier_indices,
                       keyframe.interp_bezier_ind[indice], sizeof(bgVec3f));
            }
            break;
        default:
            bg_panic("Must use compound channel when initializing a timeline");
            return;
        }
        bgVector_append(&out->keyframes, &animator_keyframe, 1);
    }
}

void bgAnimatorTimeline_free(bgAnimatorTimeline *timeline) {
    bgVector_free(&timeline->keyframes);
}

void bgAnimatorTimeline_cleanup(void *timeline) {
    bgAnimatorTimeline_free((bgAnimatorTimeline *)timeline);
}

void bg_animator(bgAnimator *out, bgAnimation *anim, bgModel *model) {
    out->animation = anim;
    out->model = model;
    out->current_time = 0.0;

    bg_vector(&out->timelines, sizeof(bgAnimatorTimeline));
}

void bgAnimator_free(bgAnimator *animator) {
    bgVector_cleanup(&animator->timelines, bgAnimatorTimeline_cleanup);
}

void bgAnimator_step(bgAnimator *animator, float step) {
    animator->current_time += step;
}

// get a transformation matrix at the current time step
void bgAnimator_getTransformationMatrix(bgMat4 *out, bgAnimator *animator,
                                        bgObject *obj) {
    bgAnimatorTimeline *trans_timeline; // :O trans-timeline! woah!
    bgAnimatorTimeline *rot_timeline;
    bgAnimatorTimeline *scale_timeline;

    for (int i = 0; i < animator->timelines.length; i++) {
        bgAnimatorTimeline *timeline = bgVector_at(bgAnimatorTimeline, &animator->timelines, i);
        if (timeline->obj != obj) // TODO: add id's to objects instead
            continue;
        switch (timeline->channel) {
        case BG_ANIM_R:
            rot_timeline = timeline;
            break;
        case BG_ANIM_T:
            trans_timeline = timeline;
            break;
        case BG_ANIM_S:
            scale_timeline = timeline;
            break;
        default:
            break;
        }
    }

    bgVec3f trans;
    bgVec3f rot;
    bgVec3f scale;

    bgAnimator_getChannelValues(&rot, &trans, &scale,
                                rot_timeline, trans_timeline, scale_timeline,
                                animator);
    rot[0] = rot[0] / 180 * BG_PI;
    rot[1] = rot[1] / 180 * BG_PI;
    rot[2] = rot[2] / 180 * BG_PI;
    bgMat4_rotateX(out, out, rot[0]);
    bgMat4_rotateY(out, out, rot[1]);
    bgMat4_rotateZ(out, out, rot[2]);
    bgMat4_translate(out, trans);
    bgMat4_scale(out, out, scale);
}

float bgAnimator_interpolate(float t, float current_value, float next_value,
                             int current_ind, int next_ind, int val_index,
                             enum bgAnimationInterpMode current_imode,
                             enum bgAnimationInterpMode next_imode,
                             bgAnimationBezier current_bezier,
                             bgAnimationBezier next_bezier,
                             bgAnimatorTimeline *timeline) {
    if (epsilon(t, 0, ANIMATOR_EPSILON))
        return current_value;
    if (epsilon(t, 1, ANIMATOR_EPSILON))
        return next_value;
    if (current_imode == BG_ANIM_LINEAR && next_imode == BG_ANIM_LINEAR)
        return lerp(t, current_value, next_value);
    if (current_imode == BG_ANIM_CATMULLROM ||
        next_imode == BG_ANIM_CATMULLROM) {
        bgAnimatorKeyframe *before_keyframe =
            bgVector_at(bgAnimatorKeyframe, &timeline->keyframes,
                        current_ind > 0 ? current_ind - 1 : 0);
        bgAnimatorKeyframe *next_keyframe_plus = bgVector_at(
            bgAnimatorKeyframe, &timeline->keyframes,
            next_ind < timeline->keyframes.length ? current_ind + 1
                                                  : timeline->keyframes.length);
        return catmull_rom_interp(t, before_keyframe->transformation[val_index],
                                  current_value, next_value,
                                  next_keyframe_plus->transformation[val_index])
    }
    if (current_imode == BG_ANIM_BEZIER || next_imode == BG_ANIM_BEZIER) {
        return bezier_interp(current_value,
                             current_value + current_bezier.right_value,
                             next_value + next_bezier.left_value, next_value);
    }
    return current_value;
}

void bgAnimator_getChannelValue(bgVec3f *out, bgAnimatorTimeline *timeline,
                                bgAnimator *animator) {
    bgAnimatorKeyframe current_keyframe;
    bgAnimatorKeyframe next_keyframe;
    int current_index = bgAnimator_getCurrentKeyframe(
        &current_keyframe, timeline, animator->current_time);
    int next_index = bgAnimator_getNextKeyframe(&next_keyframe, timeline,
                                                animator->current_time);

    const float diff = ((next_keyframe.time - animator->current_time) -
                        (current_keyframe.time - animator->current_time));
    float t = (animator->current_time - current_keyframe.time) / diff;

    bgAnimationBezier x_bezier_current;
    bgAnimationBezier x_bezier_next;
    bgAnimationBezier y_bezier_current;
    bgAnimationBezier y_bezier_next;
    bgAnimationBezier z_bezier_current;
    bgAnimationBezier z_bezier_next;

    if (timeline->anim_obj->keyframe_beziers.length > 0) {
        x_bezier_current = *bgVector_at(bgAnimationBezier,
                                        &timeline->anim_obj->keyframe_beziers,
                                        current_keyframe.bezier_indices[0]);
        x_bezier_next = *bgVector_at(bgAnimationBezier,
                                     &timeline->anim_obj->keyframe_beziers,
                                     next_keyframe.bezier_indices[0]);
        y_bezier_current = *bgVector_at(bgAnimationBezier,
                                        &timeline->anim_obj->keyframe_beziers,
                                        current_keyframe.bezier_indices[2]);
        y_bezier_next = *bgVector_at(bgAnimationBezier,
                                     &timeline->anim_obj->keyframe_beziers,
                                     next_keyframe.bezier_indices[1]);
        z_bezier_current = *bgVector_at(bgAnimationBezier,
                                        &timeline->anim_obj->keyframe_beziers,
                                        current_keyframe.bezier_indices[2]);
        z_bezier_next = *bgVector_at(bgAnimationBezier,
                                     &timeline->anim_obj->keyframe_beziers,
                                     next_keyframe.bezier_indices[2]);
    } // * it won't matter if we don't set these. This will only *not* run if we
      // don't have an bezier interpolation anways.

    *out[0] = bgAnimator_interpolate(
        t, current_keyframe.transformation[0], next_keyframe.transformation[0],
        current_index, next_index, 0, current_keyframe.interp_mode[0],
        next_keyframe.interp_mode[0], x_bezier_current, x_bezier_next,
        timeline);
    *out[1] = bgAnimator_interpolate(
        t, current_keyframe.transformation[1], next_keyframe.transformation[1],
        current_index, next_index, 1, current_keyframe.interp_mode[1],
        next_keyframe.interp_mode[1], y_bezier_current, y_bezier_next,
        timeline);
    *out[1] = bgAnimator_interpolate(
        t, current_keyframe.transformation[2], next_keyframe.transformation[2],
        current_index, next_index, 2, current_keyframe.interp_mode[2],
        next_keyframe.interp_mode[2], z_bezier_current, z_bezier_next,
        timeline);
}

void bgAnimator_getChannelValues(bgVec3f *rot_out, bgVec3f *trans_out,
                                 bgVec3f *scale_out,
                                 bgAnimatorTimeline *rot_timeline,
                                 bgAnimatorTimeline *trans_timeline,
                                 bgAnimatorTimeline *scale_timeline,
                                 bgAnimator *animator) {
    bgAnimator_getChannelValue(rot_out, rot_timeline, animator);
    bgAnimator_getChannelValue(trans_out, trans_timeline, animator);
    bgAnimator_getChannelValue(scale_out, scale_timeline, animator);
}

int bgAnimator_getCurrentKeyframe(bgAnimatorKeyframe *out,
                                  bgAnimatorTimeline *timeline, float time) {
    for (int i = 0; i < timeline->keyframes.length; i++) {
        bgAnimatorKeyframe keyframe =
            *bgVector_at(bgAnimatorKeyframe, &timeline->keyframes, i);
        if (keyframe.time > time)
            continue;
        *out = keyframe;
        return i;
    }
}

int bgAnimator_getNextKeyframe(bgAnimatorKeyframe *out,
                               bgAnimatorTimeline *timeline, float time) {
    int index = 0;
    for (int i = 0; i < timeline->keyframes.length; i++) {
        bgAnimatorKeyframe keyframe =
            *bgVector_at(bgAnimatorKeyframe, &timeline->keyframes, i);
        if (keyframe.time > time)
            continue;
        index = i;
        break;
    }
    if (index == timeline->keyframes.length)
        index = 0;

    *out = *bgVector_at(bgAnimatorKeyframe, &timeline->keyframes, index + 1);
    return index + 1;
}