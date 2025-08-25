#include "cache.h"
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

string_cache executable_cache;

bool is_executable_in_path(const char *command) {
  return is_in_cache(&executable_cache, command);
}

void init_cache(string_cache *cache) {
  cache->count = 0;
  cache->capacity = 128;
  cache->items = malloc(sizeof(char *) * cache->capacity);
  if (cache->items == NULL) {
    perror("failed to allocate memory for cache");
    exit(EXIT_FAILURE);
  }
}

void add_to_cache(string_cache *cache, const char *item) {
  if (cache->count >= cache->capacity) {
    cache->capacity *= 2;
    cache->items = realloc(cache->items, sizeof(char *) * cache->capacity);
    if (cache->items == NULL) {
      perror("failed to reallocate memory for cache");
      exit(EXIT_FAILURE);
    }
  }
  cache->items[cache->count] = strdup(item);
  cache->count++;
}

bool is_in_cache(string_cache *cache, const char *item) {
  for (int i = 0; i < cache->count; i++) {
    if (strcmp(cache->items[i], item) == 0) {
      return true;
    }
  }
  return false;
}

void free_cache(string_cache *cache) {
  for (int i = 0; i < cache->count; i++) {
    free(cache->items[i]);
  }
  free(cache->items);
}

void populate_executable_cache() {
  init_cache(&executable_cache);
  char *path = getenv("PATH");
  if (path == NULL) {
    return;
  }

  char *path_copy = strdup(path);
  if (path_copy == NULL) {
    return;
  }

  char *dir_name = strtok(path_copy, ":");
  while (dir_name != NULL) {
    DIR *dir = opendir(dir_name);
    if (dir != NULL) {
      struct dirent *entry;
      while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG || entry->d_type == DT_LNK) {
          char executable_path[1024];
          snprintf(executable_path, sizeof(executable_path), "%s/%s", dir_name,
                   entry->d_name);
          struct stat st;
          if (stat(executable_path, &st) == 0 && (st.st_mode & S_IXUSR)) {
            add_to_cache(&executable_cache, entry->d_name);
          }
        }
      }
      closedir(dir);
    }
    dir_name = strtok(NULL, ":");
  }
  free(path_copy);
}
