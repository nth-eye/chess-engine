#ifndef MOON_BITBOARD_H
#define MOON_BITBOARD_H

#include <array>
#include <cassert>
#include "moon_util.h"

namespace moon {

struct MagicInit {
    Bitboard num;
    int idx;
};

template<Piece P>
struct Magic {
    constexpr Magic() = default;
    constexpr auto operator[](Bitboard occ) const   { return att[idx(occ)]; }
    constexpr auto idx(Bitboard occ) const          { return ((occ & mask) * magic) >> shift; }
    Bitboard mask   = 0;
    Bitboard magic  = 0;
    Bitboard *att   = nullptr;
    static constexpr int shift = P == ROOK ? 52 : 55;
};

constexpr Bitboard shift(Bitboard b, Direction d)
{
    switch (d) {
        case NORTH:      return  b << 8;
        case SOUTH:      return  b >> 8;
        case EAST:       return (b & ~file_bb(FILE_H)) << 1;
        case WEST:       return (b & ~file_bb(FILE_A)) >> 1;
        case NORTH_EAST: return (b & ~file_bb(FILE_H)) << 9;
        case NORTH_WEST: return (b & ~file_bb(FILE_A)) << 7;
        case SOUTH_EAST: return (b & ~file_bb(FILE_H)) >> 7;
        case SOUTH_WEST: return (b & ~file_bb(FILE_A)) >> 9;
    }
    assert(false);
}

constexpr auto shift(Bitboard b, auto dirs)
{
    for (auto d : dirs)
        b = shift(b, d);
    return b;
}

constexpr auto shift(Bitboard b, auto d, auto... dirs)
{
    return shift(b, d) | shift(b, dirs...);
}

constexpr auto attacks_sliding(Piece p, Square s, Bitboard block)
{
    Bitboard att = 0;

    for (auto d : p == BISHOP ? 
        std::array{ NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST } : 
        std::array{ NORTH,      EAST,       SOUTH,      WEST }) 
    {
        auto dst = bit(s);
        do {
            att |= dst = shift(dst, d);
        } while (dst && !(dst & block));
    }
    return att;
}

constexpr auto attacks(Piece p, Color c = WHITE)
{
    std::array<Bitboard, 64> att = {};

    for (auto s : Squares()) {
        auto bb = bit(s);
        switch (p) {
            case PAWN:
                att[s] = c == WHITE ?
                    shift(bb, NORTH_EAST, NORTH_WEST): 
                    shift(bb, SOUTH_EAST, SOUTH_WEST); 
                break;
            case KNIGHT:
                att[s] = shift(bb, 
                    std::array{ NORTH, NORTH, EAST }, 
                    std::array{ NORTH, NORTH, WEST },
                    std::array{ NORTH, EAST,  EAST },
                    std::array{ NORTH, WEST,  WEST },
                    std::array{ SOUTH, EAST,  EAST },
                    std::array{ SOUTH, WEST,  WEST },
                    std::array{ SOUTH, SOUTH, EAST },
                    std::array{ SOUTH, SOUTH, WEST });
                break;
            case KING:
                att[s] = shift(bb, NORTH, EAST, SOUTH, WEST, NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST);   
                break;
            default: assert(false);
        }
    }
    return att;
}

template<Piece P>
auto attacks_magic(Bitboard *table, const MagicInit *init)
{
    std::array<Magic<P>, 64> magics;

    for (auto s : Squares()) {

        auto edges = 
            ((rank_bb(RANK_1) | rank_bb(RANK_8)) & ~rank_bb(rank(s))) | 
            ((file_bb(FILE_A) | file_bb(FILE_H)) & ~file_bb(file(s)));
        auto mask = attacks_sliding(P, s, 0) & ~edges;
        auto att = &table[init[s].idx];

        magics[s].mask  = mask;
        magics[s].magic = init[s].num;
        magics[s].att   = att;

        auto occ = 0; // Carry-Rippler method to iterate over all subsets of mask
        do {
            att[magics[s].idx(occ)] = attacks_sliding(P, s, occ);
            occ = (occ - mask) & mask;
        } while (occ);
    }
    return magics;
}

}

#endif