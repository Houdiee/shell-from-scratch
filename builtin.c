#include "builtins/exit.h"
#include <stdbool.h>
#include <string.h>

bool exec_built_in_commands(char **args) {
  const char *command = args[0];

  if (!strcmp(command, "exit")) {
    exit_with_status(args);
    return true;
  }

  return false;
}
