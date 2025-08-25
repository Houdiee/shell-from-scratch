#include "input.h"
#include "builtin.h"
#include "colors.h"
#include "terminal.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int INITIAL_BUFF_SIZE = 4096;
const int INITIAL_MAX_ARGS_COUNT = 128;
const int GROWTH_FACTOR = 2;

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

    char *temp_buffer = malloc(current_buf_size);
    if (temp_buffer == NULL) {
      perror("failed to allocate memory for temp buffer");
      exit(EXIT_FAILURE);
    }
    strncpy(temp_buffer, buffer, current_buf_size);
    char *first_word = strtok(temp_buffer, " ");

    if (first_word != NULL && is_built_in(first_word)) {
      int first_word_len = strlen(first_word);
      printf("\r%s%s%.*s%s%s", PROMPT, BOLD_GREEN, first_word_len, buffer,
             RESET, buffer + first_word_len);
    } else {
      printf("\r%s%s%s", PROMPT, RESET, buffer);
    }
    free(temp_buffer);
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
