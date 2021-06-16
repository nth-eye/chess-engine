#include "bitboard.h"
#include "misc.h"

constexpr const char *RANK_STR = "12345678";
constexpr const char *FILE_STR = "abcdefgh";

void print_bb(Bitboard bb)
{
    PUTC('\n');
    for (Rank r = RANK_8; r >= RANK_1; --r) {
        PRINT("%c   ", RANK_STR[r]);
        for (File f = FILE_A; f <= FILE_H; ++f) {
            PUTC(get(bb, square(r, f)) ? 'X' : '-');
            PUTC(' ');
        }
        PUTC('\n');
    }
    PRINT("\n   ");
    for (File f = FILE_A; f <= FILE_H; ++f)
        PRINT(" %c", FILE_STR[f]);
    PRINT("\n\n");
    PRINT("Hex: %016lX\n", bb);
    PUTC('\n');
}