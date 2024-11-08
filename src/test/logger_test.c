#include <errno.h>
#include <string.h>

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "../lib/c_logger.h"
#include "./utils.h"

// ####################
// template
// ####################
Test(error_new, _1) {
  int actual = add(1, 2);
  int expected = 3;

  cr_assert_eq(actual, expected);
}
