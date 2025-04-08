#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include "./c_logger.h"

//####################
// LOGGER
//####################

void timestamp(char* buff, size_t size) {
  time_t now = time(NULL);
  struct tm* time = localtime(&now);
  strftime(buff, size, "%Y-%m-%d %H:%M:%S", time);
}

Logger logger_new(LogLevel level) {
  return (Logger) {PTHREAD_MUTEX_INITIALIZER, level};
}

void log_fatal(const Logger* logger, const char* message, ...) {
  if (logger->level < FATAL) return;
  pthread_mutex_lock(&logger->lock);

  char stamp[BUFF_SIZE_TIMESTAMP];
  timestamp(stamp, BUFF_SIZE_TIMESTAMP);

  va_list args;
  va_start(args, message);
  fprintf(stderr, TXT_BRIGHT_RED "[FATAL] " RESET);
  fprintf(stderr, "(%s) -- ", stamp);
  vfprintf(stderr, message, args);
  va_end(args);

  pthread_mutex_unlock(&logger->lock);
}

void log_error(const Logger* logger, const char* message, ...) {
  if (logger->level < ERROR) return;
  pthread_mutex_lock(&logger->lock);

  char stamp[BUFF_SIZE_TIMESTAMP];
  timestamp(stamp, BUFF_SIZE_TIMESTAMP);

  va_list args;
  va_start(args, message);
  fprintf(stderr, TXT_RED "[ERROR] " RESET);
  fprintf(stderr, "(%s) -- ", stamp);
  vfprintf(stderr, message, args);
  va_end(args);

  pthread_mutex_unlock(&logger->lock);
}

void log_warn(const Logger* logger, const char* message, ...) {
  if (logger->level < WARN) return;
  pthread_mutex_lock(&logger->lock);

  char stamp[BUFF_SIZE_TIMESTAMP];
  timestamp(stamp, BUFF_SIZE_TIMESTAMP);

  va_list args;
  va_start(args, message);
  fprintf(stderr, TXT_YELLOW "[WARN] " RESET);
  fprintf(stderr, "(%s) -- ", stamp);
  vfprintf(stderr, message, args);
  va_end(args);

  pthread_mutex_unlock(&logger->lock);
}

void log_info(const Logger* logger, const char* message, ...) {
  if (logger->level < INFO) return;
  pthread_mutex_lock(&logger->lock);

  char stamp[BUFF_SIZE_TIMESTAMP];
  timestamp(stamp, BUFF_SIZE_TIMESTAMP);

  va_list args;
  va_start(args, message);
  printf(TXT_GREEN "[INFO] " RESET);
  printf("(%s) -- ", stamp);
  vfprintf(stdout, message, args);
  va_end(args);

  pthread_mutex_unlock(&logger->lock);
}

void log_debug(const Logger* logger, const char* message, ...) {
  if (logger->level < DEBUG) return;
  pthread_mutex_lock(&logger->lock);

  char stamp[BUFF_SIZE_TIMESTAMP];
  timestamp(stamp, BUFF_SIZE_TIMESTAMP);

  va_list args;
  va_start(args, message);
  printf(TXT_BLUE "[DEBUG] " RESET);
  printf("(%s) -- ", stamp);
  vfprintf(stdout, message, args);
  va_end(args);

  pthread_mutex_unlock(&logger->lock);
}

void log_trace(const Logger* logger, const char* message, ...) {
  if (logger->level < TRACE) return;
  pthread_mutex_lock(&logger->lock);

  char stamp[BUFF_SIZE_TIMESTAMP];
  timestamp(stamp, BUFF_SIZE_TIMESTAMP);

  va_list args;
  va_start(args, message);
  printf(TXT_CYAN "[TRACE] " RESET);
  printf("(%s) -- ", stamp);
  vfprintf(stdout, message, args);
  va_end(args);

  pthread_mutex_unlock(&logger->lock);
}

void log_verbose(const Logger* logger, const char* message, ...) {
  if (logger->level < VERBOSE) return;
  pthread_mutex_lock(&logger->lock);

  char stamp[BUFF_SIZE_TIMESTAMP];
  timestamp(stamp, BUFF_SIZE_TIMESTAMP);

  va_list args;
  va_start(args, message);
  printf("[VERBOSE] ");
  printf("(%s) -- ", stamp);
  vfprintf(stdout, message, args);
  va_end(args);

  pthread_mutex_unlock(&logger->lock);
}
