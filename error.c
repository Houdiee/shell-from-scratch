#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void exit_on_exec_error(char *command) {
  switch (errno) {
  case EACCES:
    fprintf(stderr, "%s: permission denied\n", command);
    exit(126);
    break;

  case ENOENT:
    fprintf(stderr, "%s: command not found\n", command);
    exit(127);
    break;

  default:
    perror(command);
    exit(EXIT_FAILURE);
  }
}

void print_error(char *command, char *error_message) {
  fprintf(stderr, "%s: %s", command, error_message);
}
