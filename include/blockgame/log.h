#ifndef BLOCKGAME_LOG_H
#define BLOCKGAME_LOG_H

#include <stdio.h>

enum bg_log_severity
{
  BG_LOG_DEBUG,
  BG_LOG_INFO,
  BG_LOG_WARN,
  BG_LOG_ERROR
};

#define bg_debug(...) bg_log_(BG_LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define bg_info(...) bg_log_(BG_LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define bg_warn(...) bg_log_(BG_LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define bg_error(...) bg_log_(BG_LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)

void
bg_init_logger(FILE *handle);

void
bg_log_(
  enum bg_log_severity severity, 
  char const *filename,
  int fileline,
  char const *format, 
  ...
);

#endif // BLOCKGAME_LOG_H