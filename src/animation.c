#include "parsing.h"
#include "stdlib.h"
#include "str.h"
#include <blockgame/animation.h>
#include <blockgame/file.h>
#include <blockgame/log.h>
#include <blockgame/panic.h>
#include <blockgame/utility.h>
#include <string.h>

void bgAnimation_bezier(bgAnimationBezier *out, float lv, float lt, float rv,
                        float rt) {
    out->left_value = lv;
    out->left_time = lt;
    out->right_value = rv;
    out->right_time = rt;
}

// * does not work with compound channels (BG_ANIM_R, BG_ANIM_T, BG_ANIM_S,
// BG_ANIM_ALL, BG_ANIM_NONE)
uint8_t bgAnimation_getChannelIndice(enum bgAnimationChannel channel) {
    switch (channel) {
    case BG_ANIM_SX:
        return 0;
    case BG_ANIM_SY:
        return 1;
    case BG_ANIM_SZ:
        return 2;
    case BG_ANIM_TX:
        return 3;
    case BG_ANIM_TY:
        return 4;
    case BG_ANIM_TZ:
        return 5;
    case BG_ANIM_RX:
        return 6;
    case BG_ANIM_RY:
        return 7;
    case BG_ANIM_RZ:
        return 8;
    default:
        bg_panic("Compound Channels do not have indices");
    }
}

void bgAnimation_keyframe(bgAnimationKeyframe *out, float timestamp) {
    out->timestamp = timestamp;
    memcpy(&out->rotation, (bgVec3f){0.0f, 0.0f, 0.0f}, sizeof(out->rotation));
    memcpy(&out->translation, (bgVec3f){0.0f, 0.0f, 0.0f},
           sizeof(out->translation));
    memcpy(&out->scale, (bgVec3f){0.0f, 0.0f, 0.0f}, sizeof(out->scale));
    memcpy(&out->pivot, (bgVec3f){0.0f, 0.0f, 0.0f}, sizeof(out->pivot));

    memset(&out->interp_mode, (int)BG_ANIM_NOCHANGE, sizeof(out->interp_mode));
    memset(&out->interp_bezier_ind, -1, sizeof(out->interp_bezier_ind));
}

void bgAnimationKeyframe_applyImode(bgAnimationKeyframe *keyframe,
                                    enum bgAnimationChannel channel,
                                    enum bgAnimationInterpMode mode) {
    switch (channel) {
    case BG_ANIM_NONE:
        return;
    case BG_ANIM_R:
        keyframe->interp_mode[bgAnimation_getChannelIndice(BG_ANIM_RX)] = mode;
        keyframe->interp_mode[bgAnimation_getChannelIndice(BG_ANIM_RY)] = mode;
        keyframe->interp_mode[bgAnimation_getChannelIndice(BG_ANIM_RZ)] = mode;
        return;
    case BG_ANIM_T:
        keyframe->interp_mode[bgAnimation_getChannelIndice(BG_ANIM_TX)] = mode;
        keyframe->interp_mode[bgAnimation_getChannelIndice(BG_ANIM_TY)] = mode;
        keyframe->interp_mode[bgAnimation_getChannelIndice(BG_ANIM_TZ)] = mode;
        return;
    case BG_ANIM_S:
        keyframe->interp_mode[bgAnimation_getChannelIndice(BG_ANIM_SX)] = mode;
        keyframe->interp_mode[bgAnimation_getChannelIndice(BG_ANIM_SY)] = mode;
        keyframe->interp_mode[bgAnimation_getChannelIndice(BG_ANIM_SZ)] = mode;
        return;
    case BG_ANIM_ALL:
        keyframe->interp_mode[bgAnimation_getChannelIndice(BG_ANIM_RX)] = mode;
        keyframe->interp_mode[bgAnimation_getChannelIndice(BG_ANIM_RY)] = mode;
        keyframe->interp_mode[bgAnimation_getChannelIndice(BG_ANIM_RZ)] = mode;
        keyframe->interp_mode[bgAnimation_getChannelIndice(BG_ANIM_TX)] = mode;
        keyframe->interp_mode[bgAnimation_getChannelIndice(BG_ANIM_TY)] = mode;
        keyframe->interp_mode[bgAnimation_getChannelIndice(BG_ANIM_TZ)] = mode;
        keyframe->interp_mode[bgAnimation_getChannelIndice(BG_ANIM_SX)] = mode;
        keyframe->interp_mode[bgAnimation_getChannelIndice(BG_ANIM_SY)] = mode;
        keyframe->interp_mode[bgAnimation_getChannelIndice(BG_ANIM_SZ)] = mode;
        return;
    default:
        keyframe->interp_mode[bgAnimation_getChannelIndice(channel)] = mode;
        return;
    }
}

void bgAnimationKeyframe_applyBezier(bgAnimationKeyframe *keyframe,
                                     enum bgAnimationChannel channel,
                                     int bezier_indice) {
    switch (channel) {
    case BG_ANIM_NONE:
        return;
    case BG_ANIM_R:
        keyframe->interp_bezier_ind[bgAnimation_getChannelIndice(BG_ANIM_RX)] =
            bezier_indice;
        keyframe->interp_bezier_ind[bgAnimation_getChannelIndice(BG_ANIM_RY)] =
            bezier_indice;
        keyframe->interp_bezier_ind[bgAnimation_getChannelIndice(BG_ANIM_RZ)] =
            bezier_indice;
        return;
    case BG_ANIM_T:
        keyframe->interp_bezier_ind[bgAnimation_getChannelIndice(BG_ANIM_TX)] =
            bezier_indice;
        keyframe->interp_bezier_ind[bgAnimation_getChannelIndice(BG_ANIM_TY)] =
            bezier_indice;
        keyframe->interp_bezier_ind[bgAnimation_getChannelIndice(BG_ANIM_TZ)] =
            bezier_indice;
        return;
    case BG_ANIM_S:
        keyframe->interp_bezier_ind[bgAnimation_getChannelIndice(BG_ANIM_SX)] =
            bezier_indice;
        keyframe->interp_bezier_ind[bgAnimation_getChannelIndice(BG_ANIM_SY)] =
            bezier_indice;
        keyframe->interp_bezier_ind[bgAnimation_getChannelIndice(BG_ANIM_SZ)] =
            bezier_indice;
        return;
    case BG_ANIM_ALL:
        keyframe->interp_bezier_ind[bgAnimation_getChannelIndice(BG_ANIM_RX)] =
            bezier_indice;
        keyframe->interp_bezier_ind[bgAnimation_getChannelIndice(BG_ANIM_RY)] =
            bezier_indice;
        keyframe->interp_bezier_ind[bgAnimation_getChannelIndice(BG_ANIM_RZ)] =
            bezier_indice;
        keyframe->interp_bezier_ind[bgAnimation_getChannelIndice(BG_ANIM_TX)] =
            bezier_indice;
        keyframe->interp_bezier_ind[bgAnimation_getChannelIndice(BG_ANIM_TY)] =
            bezier_indice;
        keyframe->interp_bezier_ind[bgAnimation_getChannelIndice(BG_ANIM_TZ)] =
            bezier_indice;
        keyframe->interp_bezier_ind[bgAnimation_getChannelIndice(BG_ANIM_SX)] =
            bezier_indice;
        keyframe->interp_bezier_ind[bgAnimation_getChannelIndice(BG_ANIM_SY)] =
            bezier_indice;
        keyframe->interp_bezier_ind[bgAnimation_getChannelIndice(BG_ANIM_SZ)] =
            bezier_indice;
        return;
    default:
        keyframe->interp_bezier_ind[bgAnimation_getChannelIndice(channel)] =
            bezier_indice;
        return;
    }
}

// * does not work with compound channels (BG_ANIM_R, BG_ANIM_T, BG_ANIM_S,
// BG_ANIM_ALL, BG_ANIM_NONE)
int bgAnimationKeyframe_getBezierIndice(bgAnimationKeyframe *keyframe,
                                        enum bgAnimationChannel channel) {
    return keyframe->interp_bezier_ind[bgAnimation_getChannelIndice(channel)];
}

void bgAnimation_object(bgAnimationObject *obj, char *name, size_t name_len) {
    bg_vector(&obj->keyframes, sizeof(bgAnimationKeyframe));
    bg_vector(&obj->keyframe_beziers, sizeof(bgAnimationBezier));

    obj->name = bg_calloc(name_len + 1, sizeof(char));

    strncpy(obj->name, name, name_len);
}

void bgAnimationObject_free(bgAnimationObject *obj) {
    bgVector_free(&obj->keyframes);
    bgVector_free(&obj->keyframe_beziers);

    free(obj->name);
}

void bgAnimationObject_cleanup(void *obj) { bgAnimationObject_free(obj); }

void bgAnimationKeyframe_getBezier(bgAnimationBezier *out,
                                   bgAnimationObject const *obj,
                                   bgAnimationKeyframe const *keyframe,
                                   enum bgAnimationChannel channel) {
    *out = *bgVector_at(bgAnimationBezier, &obj->keyframe_beziers,
                        bgAnimation_getChannelIndice(channel));
}

void bg_animation(bgAnimation *out, float duration,
                  enum bgAnimationRepeatMode repeat_mode) {
    out->duration = duration;
    out->repeat_mode = repeat_mode;
    bg_vector(&out->objects, sizeof(bgAnimationObject));
}

void bgAnimation_free(bgAnimation *anim) { bgVector_free(&anim->objects); }

void bgAnimation_getActiveObject_(bgAnimationObject **out, bgAnimation *anim) {
    if (!anim->objects.length)
        bg_panic("No active object in animation");

    *out = bgVector_at(bgAnimationObject, &anim->objects,
                       anim->objects.length - 1);
}

void bgAnimationObject_getActiveKeyframe_(bgAnimationKeyframe **out,
                                          bgAnimationObject *obj) {
    if (!obj->keyframes.length)
        bg_panic("No active keyframe in object: %s", obj->name);
    *out = bgVector_at(bgAnimationKeyframe, &obj->keyframes,
                       obj->keyframes.length - 1);
}

/// returns the new char * after parsing
char *parseChannel(enum bgAnimationChannel *out, char *line, size_t len) {
    char *initial_line = line;
    line = skip_op_(line, len);

    if (len - (line - initial_line) == 0)
        bg_panic("Channel name cannot be empty:\n  %.*s", (int)len,
                 initial_line);
    size_t channel_len = str_findNext(' ', line);
    if (channel_len == 1) {
        switch (line[0]) {
        case 'r':
            *out = BG_ANIM_R;
            return line;
        case 't':
            *out = BG_ANIM_T;
            return line;
        case 's':
            *out = BG_ANIM_S;
            return line;
        default:
            bg_panic("Invalid compound animation channel:\n  %.*s", (int)len,
                     initial_line);
        }
    } else if (channel_len == 2) {
        if (line[0] == 'r' && line[1] == 'x') {
            *out = BG_ANIM_RX;
            return line;
        } else if (line[0] == 'r' && line[1] == 'y') {
            *out = BG_ANIM_RY;
            return line;
        } else if (line[0] == 'r' && line[1] == 'z') {
            *out = BG_ANIM_RZ;
            return line;
        } else if (line[0] == 't' && line[1] == 'x') {
            *out = BG_ANIM_TX;
            return line;
        } else if (line[0] == 't' && line[1] == 'y') {
            *out = BG_ANIM_TY;
            return line;
        } else if (line[0] == 't' && line[1] == 'z') {
            *out = BG_ANIM_TZ;
            return line;
        } else if (line[0] == 's' && line[1] == 'x') {
            *out = BG_ANIM_SX;
            return line;
        } else if (line[0] == 's' && line[1] == 'y') {
            *out = BG_ANIM_SY;
            return line;
        } else if (line[0] == 's' && line[1] == 'z') {
            *out = BG_ANIM_SZ;
            return line;
        } else
            bg_panic("Invalid animation channel:\n  %.*s", (int)len,
                     initial_line);
    } else {
        bg_panic("Invalid animation channel:\n  %.*s", (int)len, initial_line);
    }
}

void parse_animation_line_(bgAnimation *out, char *line, size_t len) {
    if (!len)
        return;

    if (str_startsWith(line, "#"))
        return;
    else if (str_startsWith(line, "o")) {
        char *name = NULL;
        size_t name_len;
        bgAnimationObject obj;
        parsing_parseName(&name, &name_len, line, len);
        bgAnimation_object(&obj, name, name_len);
        bgVector_append(&out->objects, &obj, 1);
    } else if (str_startsWith(line, "rmode")) {
        int mode;
        parsing_parseInt(&mode, line, len);
        out->repeat_mode = mode;
    } else if (str_startsWith(line, "duration")) {
        float duration;
        parsing_parseFloat(&duration, line, len);
        out->duration = duration;
    } else if (str_startsWith(line, "k")) {
        bgAnimationObject *obj;
        bgAnimationKeyframe keyframe;
        float timestamp;
        bgAnimation_getActiveObject_(&obj, out);
        parsing_parseFloat(&timestamp, line, len);
        bgAnimation_keyframe(&keyframe, timestamp);
        bgVector_append(&obj->keyframes, &keyframe, 1);
    } else if (str_startsWith(line, "piv")) {
        bgAnimationObject *obj;
        bgAnimationKeyframe *keyframe;
        bgVec3f pivot;
        bgAnimation_getActiveObject_(&obj, out);
        bgAnimationObject_getActiveKeyframe_(&keyframe, obj);
        parsing_parseFloatArray((float *)&pivot, 3, line, len);
        keyframe->pivot[0] = pivot[0];
        keyframe->pivot[1] = pivot[1];
        keyframe->pivot[2] = pivot[2];
    } else if (str_startsWith(line, "r")) {
        bgAnimationObject *obj;
        bgAnimationKeyframe *keyframe;
        bgVec3f rotation;
        bgAnimation_getActiveObject_(&obj, out);
        bgAnimationObject_getActiveKeyframe_(&keyframe, obj);
        parsing_parseFloatArray((float *)&rotation, 3, line, len);
        keyframe->rotation[0] = rotation[0];
        keyframe->rotation[1] = rotation[1];
        keyframe->rotation[2] = rotation[2];
    } else if (str_startsWith(line, "t")) {
        bgAnimationObject *obj;
        bgAnimationKeyframe *keyframe;
        bgVec3f translation;
        bgAnimation_getActiveObject_(&obj, out);
        bgAnimationObject_getActiveKeyframe_(&keyframe, obj);
        parsing_parseFloatArray((float *)&translation, 3, line, len);
        keyframe->translation[0] = translation[0];
        keyframe->translation[1] = translation[1];
        keyframe->translation[2] = translation[2];
    } else if (str_startsWith(line, "s")) {
        bgAnimationObject *obj;
        bgAnimationKeyframe *keyframe;
        bgVec3f scale;
        bgAnimation_getActiveObject_(&obj, out);
        bgAnimationObject_getActiveKeyframe_(&keyframe, obj);
        parsing_parseFloatArray((float *)&scale, 3, line, len);
        keyframe->scale[0] = scale[0];
        keyframe->scale[1] = scale[1];
        keyframe->scale[2] = scale[2];
    } else if (str_startsWith(line, "imode")) {
        bgAnimationObject *obj;
        bgAnimationKeyframe *keyframe;
        enum bgAnimationChannel channel;
        int mode;
        bgAnimation_getActiveObject_(&obj, out);
        bgAnimationObject_getActiveKeyframe_(&keyframe, obj);
        char *new_line = parseChannel(&channel, line, len);
        parsing_parseInt(&mode, new_line, len);
        bgAnimationKeyframe_applyImode(keyframe, channel, mode);
    } else if (str_startsWith(line, "ibez")) {
        bgAnimationObject *obj;
        bgAnimationKeyframe *keyframe;
        enum bgAnimationChannel channel;
        bgAnimationBezier bezier;
        bgAnimation_getActiveObject_(&obj, out);
        bgAnimationObject_getActiveKeyframe_(&keyframe, obj);
        char *new_line = parseChannel(&channel, line, len);
        parsing_parseFloatArray((float *)&bezier, 4, new_line, len);
        bgVector_append(&obj->keyframe_beziers, &bezier, 1);
        bgAnimationKeyframe_applyBezier(keyframe, channel,
                                        obj->keyframe_beziers.length - 1);
    } else {
        bg_panic("Invalid .bganim file syntax:\n  %.*s", (int)len, line);
    }
}

void bgAnimation_parse(bgAnimation *out, char *stream) {
    bg_animation(out, 0.0, BG_ANIM_ONCE);
    while (stream[0]) {
        int line_len = str_findNext('\n', stream);
        parse_animation_line_(out, stream, line_len);
        stream = str_nextLine(stream);
    }
}

void bgAnimation_load(bgAnimation *out, char *file_name) {
    char *file_data = NULL;
    bg_readFile(&file_data, file_name);
    bgAnimation_parse(out, file_data);
    free(file_data);
}