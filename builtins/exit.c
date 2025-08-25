#include "../args.h"
#include "../error.h"
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

const int EXIT_MAX_ARGS_LEN = 2;

void exit_with_status(char **args) {
  char *command = args[0];
  int arg_count = 0;

  for (int i = 0; args[i] != NULL; i++) {
    arg_count++;
  }

  if (args[1] == NULL) {
    exit(EXIT_SUCCESS);
    return;
  }

  if (!args_is_expected_len(args, EXIT_MAX_ARGS_LEN)) {
    print_error(command, strerror(errno));
    return;
  };

  char *status_code_str = args[1];
  if (!arg_is_valid_number(status_code_str)) {
    print_error(command, strerror(errno));
    return;
  }

  int status_code = atoi(status_code_str);
  if (status_code < 0 || status_code > 255) {
    exit(255);
  }

  exit(status_code);
}
