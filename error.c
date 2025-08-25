#include "colors.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_error(char *command, char *error_message) {
  fprintf(stderr, "%s%s: %s%s\n", RED_COLOR, command, error_message, RESET);
}

void exit_on_exec_error(char *command) {
  switch (errno) {
  case EACCES:
    print_error(command, strerror(errno));
    exit(126);

  case ENOENT:
    print_error(command, "Command not found");
    exit(127);

  default:
    print_error(command, strerror(errno));
    exit(EXIT_FAILURE);
  }
}
