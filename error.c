#include "colors.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_error(char *command, char *error_message) {
  fprintf(stderr, "%s%s: %s%s\n", RED_COLOR, command, error_message, RESET);
}

void exit_on_exec_error(char *command) {
  int errsv = errno;

  switch (errsv) {
  case ENOENT:
    print_error(command, "Command not found");
    exit(127);

  default:
    print_error(command, strerror(errsv));
    exit(EXIT_FAILURE);
  }
}
