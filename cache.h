#ifndef CACHE_H
#define CACHE_H

#include <stdbool.h>

typedef struct {
  char **items;
  int count;
  int capacity;
} string_cache;

extern string_cache executable_cache;

void init_cache(string_cache *cache);
void add_to_cache(string_cache *cache, const char *item);
bool is_in_cache(string_cache *cache, const char *item);
void populate_executable_cache();

#endif
