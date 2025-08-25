#ifndef UTIL_H
#define UTIL_H

#include "cache.h"
#include <stdbool.h>

bool is_executable_in_path(const char *command);
char *get_current_directory();

#endif
