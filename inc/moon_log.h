#ifndef MOON_LOG_H
#define MOON_LOG_H

#include <cstdio>
#include "moon_board.h"

namespace moon {

#define out                 printf                                              // Print output
#define log(fmt, ...)       printf("<%s> " fmt "\n", __func__, ##__VA_ARGS__)   // Debug logging

#ifndef out
#define out(...)
#endif
#ifndef log
#define log(...)
#endif

constexpr char file_c(File f)   { return 'a' + f; }
constexpr char rank_c(Rank r)   { return '1' + r; }
constexpr char side_c(Side c)   { return c == WHITE ? 'w' : 'b'; }

void print_bb(Bitboard b);
void print_sq(Square s);
void print_mv(Move m);
void print_moves(const Moves &l);
void print_board(const Board &b);

}

#endif