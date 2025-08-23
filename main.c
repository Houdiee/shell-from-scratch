#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int INITIAL_BUFF_SIZE = 4096;
const int GROWTH_FACTOR = 2;

char *get_user_input() {
  int current_buf_size = INITIAL_BUFF_SIZE;
  char *buffer = malloc(sizeof(char) * current_buf_size);
  if (buffer == NULL) {
    perror("failed to allocate memory for buffer");
    return NULL;
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
        perror("failed to reallocate memory for buffer");
        return NULL;
      }
    }

    *(buffer + i) = ch;
    i++;
  }
  buffer[i] = '\0';

  return buffer;
}

int main(int argc, char *argv[]) {
  char *user_input = get_user_input();
  if (user_input == NULL) {
    return 1;
  }
  printf("%s", user_input);
  free(user_input);

  return 0;
}
