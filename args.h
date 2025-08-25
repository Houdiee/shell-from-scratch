#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>

bool args_is_expected_len(char **args, int expected_len);
bool arg_is_valid_number(char *arg);

#endif
