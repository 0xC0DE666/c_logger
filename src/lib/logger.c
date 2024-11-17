#include <stdio.h>
#include <time.h>

#include "./c_logger.h"

void get_time(char* buff, size_t size) {
  // Get the current time
  time_t now = time(NULL);
  struct tm* time = localtime(&now);

  // Format the time
  strftime(buff, size, "%Y-%m-%d %H:%M:%S", time);
}

//####################
// UNSAFE LOGGER
//####################

Logger logger_new(LogLevel level) {
  return (Logger) {level};
}

void log_error(const Logger* logger, const char* message) {
  if (logger->level < ERROR) return;

  int size = 20;
  char str_time[size];
  get_time(str_time, size);

  printf(RED "[ERROR] " RESET);
  printf("(%s) -- ", str_time);
  printf(message);
}

void log_warn(const Logger* logger, const char* message) {
  if (logger->level < WARN) return;

  int size = 20;
  char str_time[size];
  get_time(str_time, size);

  printf(YELLOW "[WARN] " RESET);
  printf("(%s) -- ", str_time);
  printf(message);
}

void log_info(const Logger* logger, const char* message) {
  if (logger->level < INFO) return;

  int size = 20;
  char str_time[size];
  get_time(str_time, size);

  printf(GREEN "[INFO] " RESET);
  printf("(%s) -- ", str_time);
  printf(message);
}

void log_debug(const Logger* logger, const char* message) {
  if (logger->level < DEBUG) return;

  int size = 20;
  char str_time[size];
  get_time(str_time, size);

  printf(BLUE "[DEBUG] " RESET);
  printf("(%s) -- ", str_time);
  printf(message);
}

void log_verbose(const Logger* logger, const char* message) {
  if (logger->level < VERBOSE) return;

  int size = 20;
  char str_time[size];
  get_time(str_time, size);

  printf("[VERBOSE] ");
  printf("(%s) -- ", str_time);
  printf(message);
}

//####################
// SAFE LOGGER
//####################

SafeLogger safe_logger_new(LogLevel level) {
  return (SafeLogger) {PTHREAD_MUTEX_INITIALIZER, logger_new(level)};
}

void safe_log_error(SafeLogger* logger, const char* message) {
  if (logger->this.level < ERROR) return;
  pthread_mutex_lock(&logger->lock);

  int size = 20;
  char str_time[size];
  get_time(str_time, size);

  printf(RED "[ERROR] " RESET);
  printf("(%s) -- ", str_time);
  printf(message);
  pthread_mutex_unlock(&logger->lock);
}

void safe_log_warn(SafeLogger* logger, const char* message) {
  if (logger->this.level < WARN) return;
  pthread_mutex_lock(&logger->lock);

  int size = 20;
  char str_time[size];
  get_time(str_time, size);

  printf(YELLOW "[WARN] " RESET);
  printf("(%s) -- ", str_time);
  printf(message);
  pthread_mutex_unlock(&logger->lock);
}

void safe_log_info(SafeLogger* logger, const char* message) {
  if (logger->this.level < INFO) return;
  pthread_mutex_lock(&logger->lock);

  int size = 20;
  char str_time[size];
  get_time(str_time, size);

  printf(GREEN "[INFO] " RESET);
  printf("(%s) -- ", str_time);
  printf(message);
  pthread_mutex_unlock(&logger->lock);
}

void safe_log_debug(SafeLogger* logger, const char* message) {
  if (logger->this.level < DEBUG) return;
  pthread_mutex_lock(&logger->lock);

  int size = 20;
  char str_time[size];
  get_time(str_time, size);

  printf(BLUE "[DEBUG] " RESET);
  printf("(%s) -- ", str_time);
  printf(message);
  pthread_mutex_unlock(&logger->lock);
}

void safe_log_verbose(SafeLogger* logger, const char* message) {
  if (logger->this.level < VERBOSE) return;
  pthread_mutex_lock(&logger->lock);

  int size = 20;
  char str_time[size];
  get_time(str_time, size);

  printf("[VERBOSE] ");
  printf("(%s) -- ", str_time);
  printf(message);
  pthread_mutex_unlock(&logger->lock);
}
