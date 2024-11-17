#include <stdio.h>

#include "../lib/c_logger.h"

int main() {
  Logger logger = logger_new(VERBOSE);
  log_error(&logger, "logger message\n");
  log_warn(&logger, "logger message\n");
  log_info(&logger, "logger message\n");
  log_debug(&logger, "logger message\n");
  log_verbose(&logger, "logger message\n");
  return 0;
}
