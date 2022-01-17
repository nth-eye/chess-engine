#ifndef UTIL_H
#define UTIL_H

#include "defs.h"

constexpr char file_c(File f)   { return 'a' + f; }
constexpr char rank_c(Rank r)   { return '1' + r; }
constexpr char side_c(Color c)  { return c == WHITE ? 'w' : 'b'; }

size_t  split(char *str, char *ptr_arr[], const char *delim, size_t max_n_toks);
bool    str_to_int(const char *str, long *val, int base, char **end);
void    print_sq(Square s);
void    print_bb(Bitboard bb);
void    print_mv(Move m);
void    print_moves(const MoveList &list);

void    test_perft(const char *file_name, size_t depth);

#endif