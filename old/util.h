#ifndef UTIL_H
#define UTIL_H

#include "defs.h"

size_t  split(char *str, char *ptr_arr[], const char *delim, size_t max_n_toks);
bool    str_to_int(const char *str, long *val, int base, char **end);
void    test_perft(const char *file_name, size_t depth);

#endif