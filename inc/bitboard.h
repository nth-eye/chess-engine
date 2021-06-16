#ifndef BITBOARD_H
#define BITBOARD_H

#include "defs.h"
#include "misc.h"

constexpr Bitboard SQUARES_WHITE    = 0x55AA55AA55AA55AAULL;
constexpr Bitboard SQUARES_BLACK    = 0xAA55AA55AA55AA55ULL;
constexpr Bitboard SQUARES_BORDER   = 0xFF818181818181FFULL;

constexpr Bitboard FILE_A_BB = 0x0101010101010101ULL;
constexpr Bitboard FILE_B_BB = FILE_A_BB << 1;
constexpr Bitboard FILE_C_BB = FILE_A_BB << 2;
constexpr Bitboard FILE_D_BB = FILE_A_BB << 3;
constexpr Bitboard FILE_E_BB = FILE_A_BB << 4;
constexpr Bitboard FILE_F_BB = FILE_A_BB << 5;
constexpr Bitboard FILE_G_BB = FILE_A_BB << 6;
constexpr Bitboard FILE_H_BB = FILE_A_BB << 7;

constexpr Bitboard RANK_1_BB = 0xFFULL;
constexpr Bitboard RANK_2_BB = RANK_1_BB << (8 * 1);
constexpr Bitboard RANK_3_BB = RANK_1_BB << (8 * 2);
constexpr Bitboard RANK_4_BB = RANK_1_BB << (8 * 3);
constexpr Bitboard RANK_5_BB = RANK_1_BB << (8 * 4);
constexpr Bitboard RANK_6_BB = RANK_1_BB << (8 * 5);
constexpr Bitboard RANK_7_BB = RANK_1_BB << (8 * 6);
constexpr Bitboard RANK_8_BB = RANK_1_BB << (8 * 7);

constexpr void print_bb(Bitboard bb)
{
    PUTC('\n');
    for (Rank r = RANK_8; r >= RANK_1; --r) {
        PRINT("%c   ", RANK_STR[r]);
        for (File f = FILE_A; f <= FILE_H; ++f) {
            PUTC(get(bb, sq(r, f)) ? 'X' : '-');
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

constexpr Bitboard shift(Bitboard b, Direction d)
{
    switch (d) {
        case NORTH:      return  b << 8;
        case SOUTH:      return  b >> 8;
        case EAST:       return (b & ~FILE_H_BB) << 1;
        case WEST:       return (b & ~FILE_A_BB) >> 1;
        case NORTH_EAST: return (b & ~FILE_H_BB) << 9;
        case NORTH_WEST: return (b & ~FILE_A_BB) << 7;
        case SOUTH_EAST: return (b & ~FILE_H_BB) >> 7;
        case SOUTH_WEST: return (b & ~FILE_A_BB) >> 9;
    }
}

template<class Arr>
constexpr Bitboard shift(Bitboard b, Arr dirs)
{
    for (auto d : dirs)
        b = shift(b, d);
    return b;
}

template<class T, class ...Args>
constexpr Bitboard shift(Bitboard b, T d, Args... dirs)
{
    return shift(b, d) | shift(b, dirs...);
}

constexpr auto attacks_pawn()
{
    std::array<std::array<Bitboard, SQ_num>, 2> att = {};

    for (Square s = A1; s <= H8; ++s) {

        auto bb = bit(s);

        att[WHITE][s] = shift(bb, NORTH_EAST, NORTH_WEST);
        att[BLACK][s] = shift(bb, SOUTH_EAST, SOUTH_WEST);  
    }
    return att;
}

template<Piece P>
constexpr auto attacks()
{
    static_assert(
        P != PAWN && 
        P < PIECE_num, 
        "Unsupported type");

    std::array<Bitboard, SQ_num> att = {};

    for (Square s = A1; s <= H8; ++s) {

        auto bb = bit(s);

        switch (P) {
            case KNIGHT:
                att[s] = shift(bb, 
                    std::array{ NORTH, NORTH, EAST}, 
                    std::array{ NORTH, NORTH, WEST},
                    std::array{ NORTH, EAST,  EAST},
                    std::array{ NORTH, WEST,  WEST},
                    std::array{ SOUTH, EAST,  EAST},
                    std::array{ SOUTH, WEST,  WEST},
                    std::array{ SOUTH, SOUTH, EAST},
                    std::array{ SOUTH, SOUTH, WEST});
                break;
            case BISHOP:
            case ROOK:
            case QUEEN:
            case KING:
            default: return att;
        }
    }
    return att;
}

#endif // BITBOARD_H