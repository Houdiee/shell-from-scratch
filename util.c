#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

bool is_executable_in_path(const char *command) {
  if (strchr(command, '/') != NULL) {
    struct stat st;
    return stat(command, &st) == 0 && (st.st_mode & S_IXUSR);
  }

  char *path = getenv("PATH");
  if (path == NULL) {
    return false;
  }

  char *path_copy = strdup(path);
  if (path_copy == NULL) {
    return false;
  }

  char *dir = strtok(path_copy, ":");
  while (dir != NULL) {
    char executable_path[1024];
    snprintf(executable_path, sizeof(executable_path), "%s/%s", dir, command);

    struct stat st;
    if (stat(executable_path, &st) == 0 && (st.st_mode & S_IXUSR)) {
      free(path_copy);
      return true;
    }

    dir = strtok(NULL, ":");
  }

  free(path_copy);
  return false;
}
