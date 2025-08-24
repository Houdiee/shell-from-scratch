#include "colors.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void print_error(char *command, char *error_message) {
  fprintf(stderr, "%s%s: %s%s\n", RED_COLOR, command, error_message, RESET);
}

void exit_on_exec_error(char *command) {
  switch (errno) {
  case EACCES:
    print_error(command, "permission denied");
    exit(126);
    break;

  case ENOENT:
    print_error(command, "command not found");
    exit(127);
    break;

  default:
    perror(command);
    exit(EXIT_FAILURE);
  }
}
