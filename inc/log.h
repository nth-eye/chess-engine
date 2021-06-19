#ifndef LOG_H
#define LOG_H

#include "defs.h"
#include "misc.h"
#include <cstdio>

#define LOG     printf
#define LOGC    putchar

#if !defined(LOG) || !defined(LOGC)
#define LOG(...)
#define LOGC(...)
#warning "To enable printing define LOG and LOGC as output mechanisms";
#endif

constexpr void print_sq(Square s)
{
    LOGC('a' + file(s));
    LOGC('1' + rank(s));
}

constexpr void print_bb(Bitboard bb)
{
    LOGC('\n');
    for (Rank r = RANK_8; r >= RANK_1; --r) {
        LOG("%c   ", rank_c(r));
        for (File f = FILE_A; f <= FILE_H; ++f) {
            LOGC(get(bb, sq(r, f)) ? 'X' : '-');
            LOGC(' ');
        }
        LOGC('\n');
    }
    LOG("\n   ");
    for (File f = FILE_A; f <= FILE_H; ++f)
        LOG(" %c", file_c(f));
    LOG("\n\n");
    LOG("hex:   %016lx \n", bb);
    LOGC('\n');
}

// constexpr void print_board()
// {

// }

#endif // LOG_H