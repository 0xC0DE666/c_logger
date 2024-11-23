#ifndef C_LOGGER_H
#define C_LOGGER_H

#include <stdbool.h>
#include <pthread.h>

//####################
// ANSI COLOR CODES
//####################

#define RESET       "\033[0m"

// Text Colors
#define TXT_BLACK       "\033[30m"
#define TXT_RED         "\033[31m"
#define TXT_GREEN       "\033[32m"
#define TXT_YELLOW      "\033[33m"
#define TXT_BLUE        "\033[34m"
#define TXT_MAGENTA     "\033[35m"
#define TXT_CYAN        "\033[36m"
#define TXT_WHITE       "\033[37m"

// Bright Text Colors
#define TXT_BRIGHT_BLACK   "\033[90m"
#define TXT_BRIGHT_RED     "\033[91m"
#define TXT_BRIGHT_GREEN   "\033[92m"
#define TXT_BRIGHT_YELLOW  "\033[93m"
#define TXT_BRIGHT_BLUE    "\033[94m"
#define TXT_BRIGHT_MAGENTA "\033[95m"
#define TXT_BRIGHT_CYAN    "\033[96m"
#define TXT_BRIGHT_WHITE   "\033[97m"

//####################
// LOGGER
//####################

void get_time(char* buff, size_t size);

typedef enum LogLevel {
  FATAL = 0, ERROR = 1, WARN = 2, INFO = 2, DEBUG = 3, TRACE = 5, VERBOSE = 6
} LogLevel;

typedef struct Logger {
  LogLevel level;
  pthread_mutex_t lock;
} Logger;

Logger logger_new(LogLevel level);

void log_fatal(Logger* logger, const char* message);
void log_error(Logger* logger, const char* message);
void log_warn(Logger* logger, const char* message);
void log_info(Logger* logger, const char* message);
void log_debug(Logger* logger, const char* message);
void log_trace(Logger* logger, const char* message);
void log_verbose(Logger* logger, const char* message);

#endif
