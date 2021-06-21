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

constexpr void print_mv(Move m) 
{
    print_sq(from(m));
    print_sq(to(m));
    switch (flag(m)) {
        case N_PROM: LOGC('n'); break;
        case B_PROM: LOGC('b'); break;
        case R_PROM: LOGC('r'); break;
        case Q_PROM: LOGC('q'); break;
        default:;
    }
}

inline void print_moves(const MoveList &list) 
{
    // constexpr const char *bit_rep[16] = {
    //     "0000", "0001", "0010", "0011",
    //     "0100", "0101", "0110", "0111",
    //     "1000", "1001", "1010", "1011",
    //     "1100", "1101", "1110", "1111",
    // };
	LOG("MoveList: %lu \n", list.size());
	for (auto move : list) {
        LOG("\n move: %04x  ", move);
        // for (size_t i = 0; i < sizeof(Move) * 2; ++i)
        //     LOG("%s ", bit_rep[(move >> (12 - i * 4)) & 0xf]);
        print_mv(move);
    }
    LOGC('\n');
}

#endif // LOG_H
