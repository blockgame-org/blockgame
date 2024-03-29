#ifndef BLOCKGAME_LOG_H
#define BLOCKGAME_LOG_H

#include <stdio.h>

#define BG_INFO 0
#define BG_WARN 1
#define BG_ERROR 2

#define bg_info(...) bg_log_(BG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define bg_warn(...) bg_log_(BG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define bg_error(...) bg_log_(BG_ERROR, __FILE__, __LINE__, __VA_ARGS__)

void
bg_init_logger(FILE *handle);

void
bg_log_(
  int severity, 
  char const *filename,
  int fileline,
  char const *format, 
  ...
);

#endif // BLOCKGAME_LOG_H