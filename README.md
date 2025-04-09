# C Logger

A thread-safe, feature-rich logging library for C applications.

## Features

- **Multiple Log Levels**: FATAL, ERROR, WARN, INFO, DEBUG, TRACE, VERBOSE
- **Thread Safety**: Built-in mutex protection for concurrent logging
- **Color Coding**: ANSI color codes for different log levels (when using stdout/stderr)
- **Timestamps**: Each log message includes a timestamp
- **Custom Output Streams**: Support for custom FILE* streams for both stdout and stderr
- **Format String Support**: printf-style formatting for log messages
- **Memory Safety**: Proper resource cleanup with logger_free()

## Log Levels

The logger supports the following log levels in order of severity:

1. `FATAL` (0) - Critical errors that prevent the application from continuing
2. `ERROR` (1) - Serious errors that don't prevent the application from running
3. `WARN` (2) - Warning messages about potential issues
4. `INFO` (3) - General information about application operation
5. `DEBUG` (4) - Detailed information for debugging
6. `TRACE` (5) - Very detailed information for tracing execution
7. `VERBOSE` (6) - Most detailed level of logging

## Usage

### Basic Usage

```c
#include "c_logger.h"

int main() {
    // Create a logger with INFO level, using stdout and stderr
    Logger* logger = logger_new(INFO, stdout, stderr);
    
    // Log messages at different levels
    log_fatal(logger, "Critical error: %s", "Something went wrong");
    log_error(logger, "Error occurred: %d", 42);
    log_warn(logger, "Warning: %s", "This might be a problem");
    log_info(logger, "Information: %s", "Everything is fine");
    log_debug(logger, "Debug info: %d", 123);
    log_trace(logger, "Trace: %s", "Detailed execution info");
    log_verbose(logger, "Verbose: %s", "Very detailed info");
    
    // Clean up
    logger_free(logger);
    return 0;
}
```

### Custom Output Streams

```c
FILE* custom_out = fopen("app.log", "w");
FILE* custom_err = fopen("error.log", "w");
Logger* logger = logger_new(INFO, custom_out, custom_err);

// Log messages will go to the custom files
log_info(logger, "This goes to app.log");
log_error(logger, "This goes to error.log");

// Clean up
logger_free(logger);
fclose(custom_out);
fclose(custom_err);
```

## Building

The library uses a Makefile for building:

```bash
# Build everything (library, tests, and example app)
make

# Run tests
make exe_test

# Build release version
make release
```

## Thread Safety

The logger is thread-safe by default. Each logging operation is protected by a mutex, ensuring that log messages from different threads won't be interleaved.

Example of thread-safe usage:

```c
void* thread_function(void* arg) {
    Logger* logger = (Logger*)arg;
    log_info(logger, "Thread %d logging", pthread_self());
    return NULL;
}

// Create multiple threads that log simultaneously
pthread_t threads[NUM_THREADS];
for (int i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threads[i], NULL, thread_function, logger);
}
```

## Error Handling

The logger includes error handling for mutex operations and provides feedback when mutex operations fail. However, it's designed to be non-blocking - if a mutex operation fails, the log message is skipped rather than blocking the application.

## Memory Management

The logger uses dynamic memory allocation for the Logger structure. It's important to call `logger_free()` when you're done with a logger to prevent memory leaks:

```c
Logger* logger = logger_new(INFO, stdout, stderr);
// ... use the logger ...
logger_free(logger);  // Don't forget this!
```

## Testing

The library includes comprehensive tests using the Criterion testing framework. Tests cover:

- Log level filtering
- Message formatting
- Timestamp format
- Thread safety
- Edge cases (empty messages, long messages)
- Custom output streams

Run the tests with:
```bash
make exe_test
```

## License

This project is open source and available under the MIT License.
