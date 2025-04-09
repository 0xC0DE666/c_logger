#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "./c_logger.h"

//####################
// LOGGER
//####################

// Returns 0 on success, -1 on error
static int safe_mutex_lock(pthread_mutex_t* mutex) {
  int result = pthread_mutex_lock(mutex);
  if (result != 0) {
    fprintf(stderr, "Failed to lock mutex: %s\n", strerror(result));
    return -1;
  }
  return 0;
}

static int safe_mutex_unlock(pthread_mutex_t* mutex) {
  int result = pthread_mutex_unlock(mutex);
  if (result != 0) {
    fprintf(stderr, "Failed to unlock mutex: %s\n", strerror(result));
    return -1;
  }
  return 0;
}

void timestamp(char* buff, size_t size) {
  time_t now = time(NULL);
  struct tm* time = localtime(&now);
  strftime(buff, size, "%Y-%m-%d %H:%M:%S", time);
}

Logger* logger_new(LogLevel level, FILE* out, FILE* err) {
  Logger* logger = (Logger*)malloc(sizeof(Logger));
  if (!logger) return NULL;
  
  pthread_mutex_init(&logger->lock, NULL);
  logger->level = level;
  logger->out = out;
  logger->err = err;
  return logger;
}

void logger_free(Logger* logger) {
  if (logger) {
    pthread_mutex_destroy(&logger->lock);
    free(logger);
  }
}

void log_fatal(const Logger* logger, const char* message, ...) {
  if (logger->level < FATAL) return;
  if (safe_mutex_lock((pthread_mutex_t*)&logger->lock) != 0) return;

  char stamp[BUFF_SIZE_TIMESTAMP];
  timestamp(stamp, BUFF_SIZE_TIMESTAMP);

  va_list args;
  va_start(args, message);
  fprintf(logger->err, TXT_RED "[FATAL] " RESET);
  fprintf(logger->err, "(%s) -- ", stamp);
  vfprintf(logger->err, message, args);
  va_end(args);
  fflush(stderr);

  safe_mutex_unlock((pthread_mutex_t*)&logger->lock);
}

void log_error(const Logger* logger, const char* message, ...) {
  if (logger->level < ERROR) return;
  if (safe_mutex_lock((pthread_mutex_t*)&logger->lock) != 0) return;

  char stamp[BUFF_SIZE_TIMESTAMP];
  timestamp(stamp, BUFF_SIZE_TIMESTAMP);

  va_list args;
  va_start(args, message);
  fprintf(logger->err, TXT_BRIGHT_RED "[ERROR] " RESET);
  fprintf(logger->err, "(%s) -- ", stamp);
  vfprintf(logger->err, message, args);
  va_end(args);
  fflush(stderr);

  safe_mutex_unlock((pthread_mutex_t*)&logger->lock);
}

void log_warn(const Logger* logger, const char* message, ...) {
  if (logger->level < WARN) return;
  if (safe_mutex_lock((pthread_mutex_t*)&logger->lock) != 0) return;

  char stamp[BUFF_SIZE_TIMESTAMP];
  timestamp(stamp, BUFF_SIZE_TIMESTAMP);

  va_list args;
  va_start(args, message);
  fprintf(logger->err, TXT_BRIGHT_YELLOW "[WARN] " RESET);
  fprintf(logger->err, "(%s) -- ", stamp);
  vfprintf(logger->err, message, args);
  va_end(args);
  fflush(stderr);

  safe_mutex_unlock((pthread_mutex_t*)&logger->lock);
}

void log_info(const Logger* logger, const char* message, ...) {
  if (logger->level < INFO)  return;
  if (safe_mutex_lock((pthread_mutex_t*)&logger->lock) != 0) return;

  char stamp[BUFF_SIZE_TIMESTAMP];
  timestamp(stamp, BUFF_SIZE_TIMESTAMP);

  va_list args;
  va_start(args, message);
  fprintf(logger->out, TXT_BRIGHT_GREEN "[INFO] " RESET);
  fprintf(logger->out, "(%s) -- ", stamp);
  vfprintf(logger->out, message, args);
  va_end(args);
  fflush(stdout);

  safe_mutex_unlock((pthread_mutex_t*)&logger->lock);
}

void log_debug(const Logger* logger, const char* message, ...) {
  if (logger->level < DEBUG) return;
  if (safe_mutex_lock((pthread_mutex_t*)&logger->lock) != 0) return;

  char stamp[BUFF_SIZE_TIMESTAMP];
  timestamp(stamp, BUFF_SIZE_TIMESTAMP);

  va_list args;
  va_start(args, message);
  fprintf(logger->out, TXT_BRIGHT_BLUE "[DEBUG] " RESET);
  fprintf(logger->out, "(%s) -- ", stamp);
  vfprintf(logger->out, message, args);
  va_end(args);
  fflush(stdout);

  safe_mutex_unlock((pthread_mutex_t*)&logger->lock);
}

void log_trace(const Logger* logger, const char* message, ...) {
  if (logger->level < TRACE) return;
  if (safe_mutex_lock((pthread_mutex_t*)&logger->lock) != 0) return;

  char stamp[BUFF_SIZE_TIMESTAMP];
  timestamp(stamp, BUFF_SIZE_TIMESTAMP);

  va_list args;
  va_start(args, message);
  fprintf(logger->out, TXT_BRIGHT_CYAN "[TRACE] " RESET);
  fprintf(logger->out, "(%s) -- ", stamp);
  vfprintf(logger->out, message, args);
  va_end(args);
  fflush(stdout);

  safe_mutex_unlock((pthread_mutex_t*)&logger->lock);
}

void log_verbose(const Logger* logger, const char* message, ...) {
  if (logger->level < VERBOSE) return;
  if (safe_mutex_lock((pthread_mutex_t*)&logger->lock) != 0) return;

  char stamp[BUFF_SIZE_TIMESTAMP];
  timestamp(stamp, BUFF_SIZE_TIMESTAMP);

  va_list args;
  va_start(args, message);
  fprintf(logger->out, TXT_BRIGHT_WHITE "[VERBOSE] " RESET);
  fprintf(logger->out, "(%s) -- ", stamp);
  vfprintf(logger->out, message, args);
  va_end(args);
  fflush(stdout);

  safe_mutex_unlock((pthread_mutex_t*)&logger->lock);
}
