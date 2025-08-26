#include "input.h"
#include "builtin.h"
#include "colors.h"
#include "terminal.h"
#include "util.h"
#include <libgen.h>
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
  const char *start = buffer;
  while (*start == ' ') {
    start++;
  }

  char *space = strchr(start, ' ');
  int first_word_len;

  if (space != NULL) {
    first_word_len = space - start;
  } else {
    first_word_len = strlen(start);
  }

  char *first_word = malloc(first_word_len + 1);
  if (first_word == NULL) {
    printf("%s", buffer);
    return;
  }

  strncpy(first_word, start, first_word_len);
  first_word[first_word_len] = '\0';

  if (is_built_in(first_word) || is_executable_in_path(first_word)) {
    int leading_spaces = start - buffer;
    for (int i = 0; i < leading_spaces; i++) {
      printf(" ");
    }
    printf("%s%.*s%s%s", BOLD_GREEN, first_word_len, start, RESET,
           start + first_word_len);
  } else {
    printf("%s", buffer);
  }

  free(first_word);
}

void print_prompt_and_buffer(const char *prompt, const char *buffer) {
  printf("\r");
  int width = get_terminal_width();
  for (int i = 0; i < width; i++) {
    printf(" ");
  }
  printf("\r");
  printf("%s", prompt);
  handle_highlighting(buffer);
  fflush(stdout);
}

char *get_user_input() {
  char *cwd = get_current_directory();
  char prompt[1024];
  sprintf(prompt, "(%s)> ", cwd);
  free(cwd);

  enable_raw_mode();

  int current_buf_size = INITIAL_BUFF_SIZE;
  char *buffer = malloc(sizeof(char) * current_buf_size);
  if (buffer == NULL) {
    perror("failed to allocate memory for user input buffer");
    exit(EXIT_FAILURE);
  }

  int i = 0;
  buffer[i] = '\0';

  print_prompt_and_buffer(prompt, buffer);

  while (true) {
    int ch = getchar();

    if (ch == 127 || ch == 8) {
      if (i > 0) {
        i--;
        buffer[i] = '\0';
      }
    } else if (ch == EOF || ch == '\n') {
      break;
    } else if (ch == '	') {
      printf("TAB");
      print_prompt_and_buffer(prompt, buffer);
      continue;
    } else if (ch >= 32 && ch <= 126) {
      if (i >= current_buf_size - 1) {
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
    }

    print_prompt_and_buffer(prompt, buffer);
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
