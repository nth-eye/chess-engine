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

            if (!get(b.p_all(), s))
                c = '.';
            else if (get(b.p_pawns(), s))
                c = 'p';
            else if (get(b.p_knights(), s))
                c = 'n';
            else if (get(b.p_bishops(), s))
                c = 'b';
            else if (get(b.p_rooks(), s))
                c = 'r';
            else if (get(b.p_queens(), s))
                c = 'q';
            else if (get(b.p_kings(), s))
                c = 'k';
            else
                c = 'x';

            if (get(b.p_whites(), s)) 
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
        b.ca_rights() & WKCA ? 'K' : '-', b.ca_rights() & WQCA ? 'Q' : '-', 
        b.ca_rights() & BKCA ? 'k' : '-', b.ca_rights() & BQCA ? 'q' : '-');
    out("enps:  ");
    if (b.sq_enps() != NO_SQ)
        print_sq(b.sq_enps());
    else
        out("-");
    out("\n");
    out("half:  %u          \n", b.ply50());
    out("full:  %u          \n", b.ply() / 2 + 1);
    out("side:  %c          \n", side_c(b.turn()));
    out("size:  %lu bytes   \n", sizeof(Board));
    out("\n");
}

}
