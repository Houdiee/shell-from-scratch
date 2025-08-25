#include "input.h"
#include "builtin.h"
#include "colors.h"
#include "terminal.h"
#include "util.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

const int INITIAL_BUFF_SIZE = 4096;
const int INITIAL_MAX_ARGS_COUNT = 128;
const int GROWTH_FACTOR = 2;

void handle_highlighting(const char *buffer) {
  const char *PROMPT = "$ ";
  char *space = strchr(buffer, ' ');
  int first_word_len;

  if (space != NULL) {
    first_word_len = space - buffer;
  } else {
    first_word_len = strlen(buffer);
  }

  char *first_word = malloc(first_word_len + 1);
  if (first_word == NULL) {
    printf("\r%s%s%s", PROMPT, RESET, buffer);
    return;
  }

  strncpy(first_word, buffer, first_word_len);
  first_word[first_word_len] = '\0';

  if (is_built_in(first_word) || is_executable_in_path(first_word)) {
    printf("\r%s%s%.*s%s%s", PROMPT, BOLD_GREEN, first_word_len, buffer, RESET,
           buffer + first_word_len);
  } else {
    printf("\r%s%s%s", PROMPT, RESET, buffer);
  }

  free(first_word);
}

char *get_user_input() {
  const char *PROMPT = "$ ";
  printf("%s", PROMPT);
  fflush(stdout);
  enable_raw_mode();

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
    buffer[i] = '\0';

    handle_highlighting(buffer);
  }
  disable_raw_mode();
  printf("\n");

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
