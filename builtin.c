#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool exec_built_in_commands(char **args) {
  const char *command = args[0];

  if (!strcmp(command, "exit")) {
    exit(EXIT_SUCCESS);
  }

  return false;
}
