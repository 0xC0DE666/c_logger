#include <stdio.h>
#include <time.h>

#include "./c_logger.h"

void write_log(LogLevel level, const char* message) {
  // Get the current time
  time_t now = time(NULL);
  struct tm* time = localtime(&now);

  // Format the time
  char str_time[20];
  strftime(str_time, sizeof(str_time), "%Y-%m-%d %H:%M:%S", time);

  if (level == ERROR && level >= ERROR) {
    printf(RED "[ERROR] " RESET);
    printf("(%s) -- ", str_time);
    printf(message);
  }
  if (level == WARN && level >= WARN) {
    printf(YELLOW "[WARN] " RESET);
    printf("(%s) -- ", str_time);
    printf(message);
  }
  if (level == INFO && level >= INFO) {
    printf(GREEN "[INFO] " RESET);
    printf("(%s) -- ", str_time);
    printf(message);
  }
  if (level == DEBUG && level >= DEBUG) {
    printf(BLUE "[DEBUG] " RESET);
    printf("(%s) -- ", str_time);
    printf(message);
  }
  if (level == VERBOSE && level >= VERBOSE) {
    printf("[VERBOSE] ");
    printf("(%s) -- ", str_time);
    printf(message);
  }
}


