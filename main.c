#include "builtin.h"
#include "cache.h"
#include "error.h"
#include "input.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// TODO
// 1. add tab completion

int main(void) {
  populate_executable_cache();
  while (true) {
    fflush(stdout);
    char *user_input = get_user_input();
    char **args = tokenize_string(user_input);

    if (args[0] != NULL && exec_built_in_commands(args)) {
    } else {
      pid_t pid = fork();
      int status;
      if (pid == -1) {
        perror("failed to fork process");
        exit(EXIT_FAILURE);
      } else if (pid == 0) {
        const int exec_result = execvp(args[0], args);
        if (exec_result == -1) {
          exit_on_exec_error(args[0]);
        }
      } else {
        waitpid(pid, &status, WUNTRACED);
      }
    }

    printf("\n");

    free(args);
    free(user_input);
  }

  return 0;
}
