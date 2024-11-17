#ifndef C_LOGGER_H
#define C_LOGGER_H

#include <stdbool.h>
#include <pthread.h>

// Define ANSI color codes
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define RESET "\033[0m"

void get_time(char* buff, size_t size);

typedef enum LogLevel {
  ERROR = 0, WARN = 1, INFO = 2, DEBUG = 3, VERBOSE = 4
} LogLevel;

//####################
// BASIC LOGGER
//####################
typedef struct Logger {
  LogLevel level;
} Logger;

Logger logger_new(LogLevel level);

void log_error(const Logger* logger, const char* message);
void log_warn(const Logger* logger, const char* message);
void log_info(const Logger* logger, const char* message);
void log_debug(const Logger* logger, const char* message);
void log_verbose(const Logger* logger, const char* message);

//####################
// SAFE LOGGER
//####################

typedef struct SafeLogger {
  pthread_mutex_t lock;
  Logger this;
} SafeLogger;

SafeLogger safe_logger_new(LogLevel level);

void safe_log_error(SafeLogger* logger, const char* message);
void safe_log_warn(SafeLogger* logger, const char* message);
void safe_log_info(SafeLogger* logger, const char* message);
void safe_log_debug(SafeLogger* logger, const char* message);
void safe_log_verbose(SafeLogger* logger, const char* message);

#endif
