#include "input.h"
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
