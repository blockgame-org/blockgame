#include <blockgame/log.h>

#include <blockgame/engine.h>
#include <blockgame/file.h>
#include <blockgame/log.h>

int main(int argc, char const **argv) {
  (void)argc;
  (void)argv;

  bg_init_logger(stderr);

  char *content = NULL;
  if (-1 == bg_read_file(&content, "res/shaders/main.vert"))
    bg_error("hmm");
  bg_info(content);

  struct bg_engine e;

  if (0 != bg_engine(&e))
    return 1;
  if (0 != bg_engine_run(&e))
    return 1;
  bg_engine_free(&e);

  return 0;
}