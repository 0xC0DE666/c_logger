#include <stdio.h>
#include <time.h>

#include "./c_logger.h"


Logger logger_new(LogLevel level) {
  return (Logger) {level};
}

void get_time(char* buff, size_t size) {
  // Get the current time
  time_t now = time(NULL);
  struct tm* time = localtime(&now);

  // Format the time
  strftime(buff, size, "%Y-%m-%d %H:%M:%S", time);
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
