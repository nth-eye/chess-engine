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
        case N_PROM: out("n"); break;
        case B_PROM: out("b"); break;
        case R_PROM: out("r"); break;
        case Q_PROM: out("q"); break;
        default:;
    }
}

void print_moves(const Moves &l) 
{
	out("%lu moves\n", l.size());
	for (const auto move : l) {
        out("\n move: %04x  ", move);
        print_mv(move);
    }
}

void print_board(const Board &b)
{
    out("\n");
    for (auto r : RanksRev()) {
        out("%c   ", rank_c(r));
        for (auto f : Files()) {

            Square s    = sq(r, f);
            char c      = ' ';

            if (!get(b.all(), s))
                c = '.';
            else if (get(b.kings(), s))
                c = 'k';
            else if (get(b.pawns, s))
                c = 'p';
            else if (get(b.bishops, s))
                c = get(b.rooks, s) ? 'q' : 'b';
            else if (get(b.rooks, s))
                c = 'r';
            else
                c = 'n';

            if (get(b.pieces[WHITE], s)) 
                c ^= bit(5);

            out("%c ", c);
        }
        out("\n");
    }
    out("\n   ");
    for (auto f : Files())
        out(" %c", file_c(f));
    out("\n\n");
    out("cast:  %c%c%c%c    \n", 
        b.castle & WKCA ? 'K' : '-', b.castle & WQCA ? 'Q' : '-', 
        b.castle & BKCA ? 'k' : '-', b.castle & BQCA ? 'q' : '-');
    out("enps:  ");
    if (b.enps_sq)
        print_sq(b.enps_sq);
    else
        out("-");
    out("\n");
    out("half:  %u          \n", b.half_clk);
    out("full:  %u          \n", b.full_clk);
    out("side:  %c          \n", side_c(b.side));
    out("size:  %lu bytes   \n", sizeof(Board));
    out("\n");
}

}
