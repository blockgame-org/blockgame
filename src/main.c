#include <blockgame/engine.h>

#include <blockgame/log.h>

int main(int argc, char const **argv) {
    (void)argc;
    (void)argv;

    bg_initLogger(stderr);

    if (0 != bg_initEngine())
        return 1;
    if (0 != bgEngine_run())
        return 1;

    return 0;
}