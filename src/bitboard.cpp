#include "bitboard.h"
#include "misc.h"

void print_bb(Bitboard bb)
{
    for (Rank rank = RANK_8; rank >= RANK_1; --rank) {
        PUTC('|');
        for (File file = FILE_A; file <= FILE_H; ++file) {
            PUTC(get(bb, square(rank, file)) ? 'X' : '-');
            PUTC('|');
        }
        PUTC('\n');
    }
}