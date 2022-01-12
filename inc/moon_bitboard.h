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
    constexpr Bitboard operator[](Bitboard block) const 
    {
        return attacks[((block & mask) * magic) >> shift];
    };
    Bitboard mask       = 0;
    Bitboard magic      = 0;
    Bitboard *attacks   = nullptr;
    int shift           = 0;
};

}

#endif