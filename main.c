#include "errno.h"
#include "sys/types.h"
#include "sys/wait.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int INITIAL_BUFF_SIZE = 4096;
const int INITIAL_MAX_ARGS_COUNT = 128;
const int GROWTH_FACTOR = 2;

char *get_user_input() {
  int current_buf_size = INITIAL_BUFF_SIZE;
  char *buffer = malloc(sizeof(char) * current_buf_size);
  if (buffer == NULL) {
    perror("failed to allocate memory for user input buffer");
    exit(EXIT_FAILURE);
  }

  int i = 0;
  while (true) {
    int ch = getchar();

    if (ch == EOF || ch == '\n') {
      break;
    }

    if (i >= current_buf_size) {
      current_buf_size *= GROWTH_FACTOR;
      buffer = realloc(buffer, current_buf_size);
      if (buffer == NULL) {
        perror("failed to reallocate memory for user input buffer");
        exit(EXIT_FAILURE);
      }
    }

    *(buffer + i) = ch;
    i++;
  }
  buffer[i] = '\0';

  return buffer;
}

char **tokenize_string(char *string) {
  int current_max_tokens = INITIAL_MAX_ARGS_COUNT;
  char **tokens = malloc(sizeof(char *) * current_max_tokens);
  if (tokens == NULL) {
    perror("failed to allocate memory for tokenized arguments buffer");
    exit(EXIT_FAILURE);
  }

  int i = 0;
  char *token = strtok(string, " ");
  while (token != NULL) {
    if (i >= current_max_tokens) {
      current_max_tokens *= GROWTH_FACTOR;
      tokens = realloc(tokens, sizeof(char *) * current_max_tokens);
      if (tokens == NULL) {
        perror("failed to reallocate memory for tokenized arguments buffer");
        exit(EXIT_FAILURE);
      }
    }
    tokens[i] = token;
    token = strtok(NULL, " ");
    i++;
  }

  tokens[i] = NULL;
  return tokens;
}

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

void exec_built_in_commands(char **args) {
  const char *command = args[0];

  if (!strcmp(command, "exit")) {
    exit(EXIT_SUCCESS);
  }
}

int main(int argc, char *argv[]) {
  while (true) {
    printf("$ ");

    char *user_input = get_user_input();
    if (user_input == NULL) {
      return 1;
    }

    char **args = tokenize_string(user_input);

    if (args[0] != NULL) {
      exec_built_in_commands(args);
      free(args);
      free(user_input);
    }

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

    free(args);
    free(user_input);
  }

  return 0;
}
