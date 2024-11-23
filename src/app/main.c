#include <stdio.h>

#include "../lib/c_logger.h"

int i = 0;

void* t_log(void* logger) {
  Logger* l = (Logger*) logger;
  pthread_t thread_id = pthread_self();
  char msg[50] = {};
  ++i;
  sprintf(msg, "%d thread log %lu\n", i, thread_id);
  log_info(l, msg);

  return NULL;
}

int main() {
  Logger logger = logger_new(VERBOSE);
  pthread_t threads[10] = {};
  for (int i = 0; i < 10; ++i) {
    pthread_create(&threads[i], NULL, t_log, &logger);
  }

  for (int i = 0; i < 10; ++i) {
    pthread_join(threads[i], NULL);
  }

  log_fatal(&logger, "logger message %d\n", FATAL);
  log_error(&logger, "logger message\n");
  log_warn(&logger, "logger message\n");
  log_info(&logger, "logger message\n");
  log_debug(&logger, "logger message\n");
  log_trace(&logger, "logger message\n");
  log_verbose(&logger, "logger message\n");

  return 0;
}
