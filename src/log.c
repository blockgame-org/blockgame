#include <blockgame/log.h>

#include <blockgame/panic.h>

#include <stdarg.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>

static mtx_t logMtx;
static FILE *logFd = NULL;

void destroyLogger_(void) {
  logFd = NULL;
  mtx_destroy(&logMtx);
}

void bg_initLogger(FILE *handle) {
  if (mtx_init(&logMtx, mtx_plain) != thrd_success)
    bg_panic("Failed to initialize mutex");

  logFd = handle;

  atexit(destroyLogger_);
}

void bg_log_(enum bgLogSeverity logSev, char const *fileName, int fileLine,
             char const *format, ...) {
  if (!logFd)
    bg_panic("Logger not initialized");

  if (mtx_lock(&logMtx) != thrd_success)
    bg_panic("Failed to lock mutex");

  char const *sevStr = NULL;
  switch (logSev) {
  case BG_LOG_DEBUG:
    sevStr = "DEBUG";
    break;
  case BG_LOG_INFO:
    sevStr = "INFO";
    break;
  case BG_LOG_WARN:
    sevStr = "WARN";
    break;
  case BG_LOG_ERROR:
    sevStr = "ERROR";
    break;
  }

  time_t t = time(NULL);
  struct tm *local = localtime(&t);

  fprintf(logFd, "%s [%d:%d:%d] (%s:%d) ", sevStr, local->tm_hour,
          local->tm_min, local->tm_sec, fileName, fileLine);

  va_list list;
  va_start(list, format);
  vfprintf(logFd, format, list);
  va_end(list);

  fprintf(logFd, "\n");
  fflush(logFd);

  if (mtx_unlock(&logMtx) != thrd_success)
    bg_panic("Failed to unlock mutex");
}