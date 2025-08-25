#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

bool args_is_expected_len(char **args, int expected_len) {
  int arg_count = 0;
  for (int i = 0; args[i] != NULL; i++) {
    arg_count++;
  }

  if (arg_count - 1 > expected_len) {
    errno = E2BIG;
    return false;
  }

  if (arg_count - 1 < expected_len) {
    errno = EINVAL;
    return false;
  }

  return true;
}

bool arg_is_valid_number(char *arg) {
  char *endptr;
  const long num = strtol(arg, &endptr, 10);

  if (*endptr != '\0') {
    errno = EINVAL;
    return false;
  }

  if ((num == LONG_MAX || num == LONG_MIN) && errno == ERANGE) {
    return false;
  }

  return true;
}
