#include "../error.h"
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

const int MAX_ARGS_LEN = 2;

void exit_with_status(char **args) {
  if (args[1] == NULL || args[2] != NULL) {
    errno = (args[1] == NULL) ? EINVAL : E2BIG;
    print_error(args[0], strerror(errno));
    return;
  }

  char *endptr;
  const long code = strtol(args[1], &endptr, 10);

  if (*endptr != '\0') {
    errno = EINVAL;
    print_error(args[0], strerror(errno));
    return;
  }

  if ((code == LONG_MAX || code == LONG_MIN) && errno == ERANGE) {
    print_error(args[0], strerror(errno));
    return;
  }

  exit((int)code);
}
