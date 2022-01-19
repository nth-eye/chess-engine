#include "moon_log.h"

namespace moon {

void print_bb(Bitboard b)
{
    out("\n");
    for (auto r : RanksRev()) {
        out("%c   ", rank_c(r));
        for (auto f : Files())
            out("%c ", get(b, sq(r, f)) ? 'X' : '-');
        out("\n");
    }
    out("\n   ");
    for (auto f : Files())
        out(" %c", file_c(f));
    out("\n\nhex:   %016lx \n\n", b);
}

void print_sq(Square s)
{
    out("%c%c", 'a' + file(s), '1' + rank(s));
}

void print_mv(Move m) 
{
    print_sq(from(m));
    print_sq(to(m));
    switch (flag(m)) {
        case N_PROM: OUT("n"); break;
        case B_PROM: OUT("b"); break;
        case R_PROM: OUT("r"); break;
        case Q_PROM: OUT("q"); break;
        default:;
    }
}

void print_moves(const Moves &list) 
{
	out("%lu moves\n", list.size());
	for (auto move : list) {
        out("\n move: %04x  ", move);
        print_mv(move);
    }
}
