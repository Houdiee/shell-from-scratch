#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdbool.h>

bool exec_built_in_commands(char **args);

bool is_built_in(const char *command);

#endif
