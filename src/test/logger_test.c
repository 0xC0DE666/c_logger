#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <criterion/parameterized.h>

#include "../lib/c_logger.h"

// Redirect stdout/stderr before each test
void redirect_all_std(void) {
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Test logger initialization
Test(logger_init, creates_logger_with_correct_level) {
    Logger* logger = logger_new(INFO);
    cr_assert_eq(logger->level, INFO, "Logger level should be INFO");
    logger_free(logger);
}

// Test different log levels
Test(logger_levels, fatal_shows_only_fatal, .init = redirect_all_std) {
    Logger* logger = logger_new(FATAL);
    
    log_fatal(logger, "Fatal message\n");
    log_error(logger, "Error message\n");
    log_warn(logger, "Warn message\n");
    
    fflush(stderr);
    cr_assert_stderr_neq_str("", "Should have some output");
    cr_assert_stderr_eq_str(TXT_BRIGHT_RED "[FATAL] " RESET "(" /* timestamp */ ") -- Fatal message\n",
                           "Should only show FATAL message");
    logger_free(logger);
}

Test(logger_levels, error_shows_fatal_and_error, .init = redirect_all_std) {
    Logger* logger = logger_new(ERROR);
    
    log_fatal(logger, "Fatal message\n");
    log_error(logger, "Error message\n");
    log_warn(logger, "Warn message\n");
    
    fflush(stderr);
    cr_assert_stderr_neq_str("", "Should have some output");
    // Can't check exact string due to timestamps, but can check for presence of messages
    char stderr_output[4096] = {0};
    FILE* f = cr_get_redirected_stderr();
    rewind(f);
    size_t bytes = fread(stderr_output, 1, sizeof(stderr_output) - 1, f);
    stderr_output[bytes] = '\0';
    
    cr_assert(strstr(stderr_output, "Fatal message") != NULL, "Should contain FATAL message");
    cr_assert(strstr(stderr_output, "Error message") != NULL, "Should contain ERROR message");
    cr_assert(strstr(stderr_output, "Warn message") == NULL, "Should not contain WARN message");
    logger_free(logger);
}

// Test message formatting
Test(logger_formatting, supports_format_strings, .init = redirect_all_std) {
    Logger* logger = logger_new(INFO);
    
    log_info(logger, "Test number: %d, string: %s\n", 42, "hello");
    
    fflush(stdout);
    char stdout_output[4096] = {0};
    FILE* f = cr_get_redirected_stdout();
    rewind(f);
    size_t bytes = fread(stdout_output, 1, sizeof(stdout_output) - 1, f);
    stdout_output[bytes] = '\0';
    
    cr_assert(strstr(stdout_output, "Test number: 42, string: hello") != NULL,
              "Should format strings correctly");
    logger_free(logger);
}

// Test timestamp format
Test(logger_timestamp, has_correct_format, .init = redirect_all_std) {
    Logger* logger = logger_new(INFO);
    
    log_info(logger, "Test message\n");
    
    fflush(stdout);
    char stdout_output[4096] = {0};
    FILE* f = cr_get_redirected_stdout();
    rewind(f);
    size_t bytes = fread(stdout_output, 1, sizeof(stdout_output) - 1, f);
    stdout_output[bytes] = '\0';
    
    // Check for timestamp format components
    cr_assert(strstr(stdout_output, "[INFO]") != NULL, "Should have INFO level");
    cr_assert(strstr(stdout_output, "Test message") != NULL, "Should have message");
    
    // Verify timestamp format (YYYY-MM-DD HH:MM:SS)
    char* timestamp_start = strchr(stdout_output, '(');
    cr_assert(timestamp_start != NULL, "Should have timestamp start");
    char* timestamp_end = strchr(timestamp_start, ')');
    cr_assert(timestamp_end != NULL, "Should have timestamp end");
    
    // Verify timestamp length (should be 19 chars: YYYY-MM-DD HH:MM:SS)
    cr_assert_eq(timestamp_end - timestamp_start - 1, 19, "Timestamp should be in correct format");
    logger_free(logger);
}

// Thread safety test structure
#define NUM_THREADS 4
#define MESSAGES_PER_THREAD 100

void* thread_log_function(void* arg) {
    Logger* logger = (Logger*)arg;
    for (int i = 0; i < MESSAGES_PER_THREAD; i++) {
        log_info(logger, "Thread message %d\n", i);
        usleep(1); // Small delay to increase chance of race conditions
    }
    return NULL;
}

Test(logger_thread_safety, multiple_threads, .init = redirect_all_std) {
    Logger* logger = logger_new(INFO);
    pthread_t threads[NUM_THREADS];
    
    // Create multiple threads that log simultaneously
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_log_function, logger);
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    fflush(stdout);
    char stdout_output[40960] = {0};  // Large buffer for thread output
    FILE* f = cr_get_redirected_stdout();
    rewind(f);
    size_t bytes = fread(stdout_output, 1, sizeof(stdout_output) - 1, f);
    stdout_output[bytes] = '\0';
    
    // Verify we have the expected number of messages
    int message_count = 0;
    const char* pos = stdout_output;
    while ((pos = strstr(pos + 1, "Thread message")) != NULL) {
        message_count++;
    }
    
    cr_assert_eq(message_count, NUM_THREADS * MESSAGES_PER_THREAD,
                 "All messages should be logged without corruption");
    logger_free(logger);
}

// Test all log levels output
Test(logger_output, all_levels_verbose, .init = redirect_all_std) {
    Logger* logger = logger_new(VERBOSE);
    
    log_fatal(logger, "Fatal test\n");
    log_error(logger, "Error test\n");
    log_warn(logger, "Warn test\n");
    log_info(logger, "Info test\n");
    log_debug(logger, "Debug test\n");
    log_trace(logger, "Trace test\n");
    log_verbose(logger, "Verbose test\n");
    
    fflush(stdout);
    fflush(stderr);
    
    char stderr_output[4096] = {0}, stdout_output[4096] = {0};
    
    FILE* ferr = cr_get_redirected_stderr();
    rewind(ferr);
    size_t bytes_err = fread(stderr_output, 1, sizeof(stderr_output) - 1, ferr);
    stderr_output[bytes_err] = '\0';
    
    FILE* fout = cr_get_redirected_stdout();
    rewind(fout);
    size_t bytes_out = fread(stdout_output, 1, sizeof(stdout_output) - 1, fout);
    stdout_output[bytes_out] = '\0';
    
    cr_assert(strstr(stderr_output, "Fatal test") != NULL, "FATAL should be present");
    cr_assert(strstr(stderr_output, "Error test") != NULL, "ERROR should be present");
    cr_assert(strstr(stderr_output, "Warn test") != NULL, "WARN should be present");
    cr_assert(strstr(stdout_output, "Info test") != NULL, "INFO should be present");
    cr_assert(strstr(stdout_output, "Debug test") != NULL, "DEBUG should be present");
    cr_assert(strstr(stdout_output, "Trace test") != NULL, "TRACE should be present");
    cr_assert(strstr(stdout_output, "Verbose test") != NULL, "VERBOSE should be present");
    logger_free(logger);
}

// Test empty messages
Test(logger_edge_cases, empty_message, .init = redirect_all_std) {
    Logger* logger = logger_new(INFO);
    log_info(logger, "");
    
    fflush(stdout);
    char stdout_output[4096] = {0};
    FILE* f = cr_get_redirected_stdout();
    rewind(f);
    size_t bytes = fread(stdout_output, 1, sizeof(stdout_output) - 1, f);
    stdout_output[bytes] = '\0';
    
    cr_assert(strstr(stdout_output, "[INFO]") != NULL, "Empty message should still be logged with proper format");
    logger_free(logger);
}

// Test long messages
Test(logger_edge_cases, long_message, .init = redirect_all_std) {
    Logger* logger = logger_new(INFO);
    char long_msg[1024];
    memset(long_msg, 'a', 1023);
    long_msg[1023] = '\0';
    
    log_info(logger, "%s\n", long_msg);
    
    fflush(stdout);
    char stdout_output[4096] = {0};
    FILE* f = cr_get_redirected_stdout();
    rewind(f);
    size_t bytes = fread(stdout_output, 1, sizeof(stdout_output) - 1, f);
    stdout_output[bytes] = '\0';
    
    cr_assert(strstr(stdout_output, long_msg) != NULL, "Long message should be logged correctly");
    logger_free(logger);
}
