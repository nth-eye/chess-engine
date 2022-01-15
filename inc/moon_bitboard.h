#ifndef MOON_BITBOARD_H
#define MOON_BITBOARD_H

#include "moon_util.h"

namespace moon {

template<Piece P>
struct BlackMagic {
    constexpr BlackMagic() = default;
    constexpr operator[](Bitboard block) const 
    { 
        return att[((block | mask) * magic) >> shift];
    }
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
            dst  = shift(dst, d);
            att |= dst;
        } while (dst && (dst & block) == 0);
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
            case BISHOP: att[s]  = attacks_sliding(BISHOP, s, 0); break;
            case QUEEN:  att[s]  = attacks_sliding(BISHOP, s, 0);
            case ROOK:   att[s] |= attacks_sliding(ROOK,   s, 0); break;
                break;
            case KING:
                att[s] = shift(bb, NORTH, EAST, SOUTH, WEST, NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST);   
                break;
            default: assert(false);
        }
    }
    return att;
}

// constexpr auto attacks_occupancy(Bitboard idx, Bitboard mask)
// {
//     int n = 0;
//     Bitboard occ = 0;
//     for (auto b : BitIter(mask)) {
//         if (bit(n++) & idx)
//             set(occ, b);
//     }
//     return occ;
// }

// template<Piece P>
// auto attacks_magic(const Bitboard *magic_nums)
// {
//     static Bitboard table[87988];

//     std::array<BlackMagic<P>, 64> magics = {};

//     size_t offset = 0;

//     for (auto s : Squares()) {

//         auto edges = 
//             ((rank_bb(RANK_1) | rank_bb(RANK_8)) & ~rank_bb(rank(s))) | 
//             ((file_bb(FILE_A) | file_bb(FILE_H)) & ~file_bb(file(s)));
//         auto mask = attacks_sliding(P, s, 0) & ~edges;

//         magics[s].mask      = ~mask;
//         magics[s].magic     = magic_nums[s];
//         magics[s].attacks   = &table[offset];

//         for (Bitboard idx = 0; idx < bit(64 - BlackMagic<P>::shift); ++idx) {

//             auto occ = attacks_occupancy(idx, mask);
//             auto att = attacks_sliding(P, s, occ);

//             occ = occ * magics[s].magic >> BlackMagic<P>::shift;

//             table[offset + occ] = att;
//         }
//     }
//     return magics;
// }

constexpr bool same_line(Square s1, Square s2)
{
    return rank(s1) == rank(s2) || file(s1) == file(s2);
}

constexpr bool same_diag(Square s1, Square s2)
{
    return  rank(s2) - rank(s1) == file(s2) - file(s1) ||
            rank(s2) - rank(s1) +  file(s2) - file(s1) == 0;
}

}

#endif