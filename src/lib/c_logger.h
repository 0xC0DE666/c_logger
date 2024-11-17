#ifndef C_LOGGER_H
#define C_LOGGER_H

// Define ANSI color codes
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define RESET "\033[0m"

typedef enum LogLevel {
  ERROR = 0, WARN = 1, INFO = 2, DEBUG = 3, VERBOSE = 4
} LogLevel;

typedef struct Logger {
  LogLevel level;
} Logger;

Logger logger_new(LogLevel level);

void get_time(char* buff, size_t size);

void log_error(const Logger* logger, const char* message);
void log_warn(const Logger* logger, const char* message);
void log_info(const Logger* logger, const char* message);
void log_debug(const Logger* logger, const char* message);
void log_verbose(const Logger* logger, const char* message);

#endif
