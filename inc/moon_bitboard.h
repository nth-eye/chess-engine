#ifndef MOON_BITBOARD_H
#define MOON_BITBOARD_H

#include "moon_util.h"

namespace moon {

struct BitIter {
    constexpr BitIter(Bitboard bb) : bb{bb} {}
    constexpr BitIter begin()   { return bb; }
    constexpr BitIter end()     { return 0; }
    constexpr bool  operator!=(const BitIter &other) const  { return bb != other.bb; }
    constexpr void  operator++()                            { bb &= bb - 1; }
    constexpr auto  operator*() const                       { return lsb(bb); }
private:
    Bitboard bb;
};

struct Magic {
    constexpr Magic() = default;
    constexpr auto operator[](Bitboard block) const 
    {
        return attacks[((block & mask) * magic) >> shift];
    };
    Bitboard mask       = 0;
    Bitboard magic      = 0;
    Bitboard *attacks   = nullptr;
    int shift           = 0;
};

constexpr Bitboard rank_bb(Rank r)  { return 0x00000000000000ffULL << (r << 3); }
constexpr Bitboard file_bb(File f)  { return 0x0101010101010101ULL << f; }
constexpr bool same_line(Square s1, Square s2)
{
    return rank(s1) == rank(s2) || file(s1) == file(s2);
}
constexpr bool same_diag(Square s1, Square s2)
{
    return  rank(s2) - rank(s1) == file(s2) - file(s1) ||
            rank(s2) - rank(s1) +  file(s2) - file(s1) == 0;
}
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
    return 0;
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

}

#endif