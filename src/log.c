#include <blockgame/log.h>

#include <blockgame/panic.h>

#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <threads.h>

static mtx_t log_mtx;
static FILE *log_handle = NULL;

void
destroy_logger(void)
{
  log_handle = NULL;
  mtx_destroy(&log_mtx);
}

void
bg_init_logger(FILE *handle)
{
  if (mtx_init(&log_mtx, mtx_plain) != thrd_success)
    bg_panic("Failed to initialize mutex");

  log_handle = handle;

  atexit(destroy_logger);
}

void
bg_log_(
  enum bg_log_severity severity,
  char const *filename,
  int fileline,
  char const *format,
  ...
)
{
  if (!log_handle)
    bg_panic("Logger not initialized");

  if (mtx_lock(&log_mtx) != thrd_success)
    bg_panic("Failed to lock mutex");

  char const *severity_str = NULL; 
  switch (severity)
  {
  case BG_LOG_DEBUG:
    severity_str = "DEBUG";
    break;
  case BG_LOG_INFO:
    severity_str = "INFO";
    break;
  case BG_LOG_WARN:
    severity_str = "WARN";
    break;
  case BG_LOG_ERROR: 
    severity_str = "ERROR";
    break;
  }

  time_t t = time(NULL);
  struct tm *local = localtime(&t);

  fprintf(
    log_handle, 
    "%s [%d:%d:%d] (%s:%d) ", 
    severity_str, 
    local->tm_hour,
    local->tm_min,
    local->tm_sec,
    filename,
    fileline
  );

  va_list list;
  va_start(list, format);
  vfprintf(log_handle, format, list);
  va_end(list);

  fprintf(log_handle, "\n");
  fflush(log_handle);

  if (mtx_unlock(&log_mtx) != thrd_success)
    bg_panic("Failed to unlock mutex");
}