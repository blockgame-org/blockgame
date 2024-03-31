#include <blockgame/animation.h>

void bgAnimation_bezier(bgAnimationBezier *out, float lv, float lt, float rv,
                        float rt) {
    out->left_value = lv;
    out->left_time = lt;
    out->right_value = rv;
    out->right_time = rt;
}