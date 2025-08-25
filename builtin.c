#include "builtins/cd.h"
#include "builtins/exit.h"
#include <stdbool.h>
#include <string.h>

typedef struct {
  const char *name;
  void (*func)(char **);
} built_in_command;

static const built_in_command built_ins[] = {
    {"exit", exit_with_status}, {"cd", change_working_directory}, {NULL, NULL}};

bool exec_built_in_commands(char **args) {
  if (!args || !args[0]) {
    return false;
  }

  const char *command = args[0];

  for (int i = 0; built_ins[i].name != NULL; i++) {
    if (strcmp(command, built_ins[i].name) == 0) {
      built_ins[i].func(args);
      return true;
    }
  }

  return false;
}

bool is_built_in(const char *command) {
  if (!command) {
    return false;
  }
  for (int i = 0; built_ins[i].name != NULL; i++) {
    if (strcmp(command, built_ins[i].name) == 0) {
      return true;
    }
  }
  return false;
}
