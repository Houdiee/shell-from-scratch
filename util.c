#include "cache.h"
#include "string.h"
#include "unistd.h"
#include <libgen.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool is_executable_in_path(const char *command) {
  return is_in_cache(&executable_cache, command);
}

const int PATHNAME_MAX_BYTES = 1024;

char *get_current_directory() {
  char full_path[PATHNAME_MAX_BYTES];
  char *dir_name;

  if (getcwd(full_path, sizeof(full_path)) == NULL) {
    perror("getcwd() error");
    exit(EXIT_FAILURE);
  }

  dir_name = basename(full_path);

  return dir_name;
}
