#ifndef MOON_UTIL_H
#define MOON_UTIL_H

#if __cplusplus >= 202002L
#include <bit>
#endif
#include "moon_type.h"

namespace moon {

constexpr Bitboard bit(int i)               { return 1ul << i; }
constexpr auto file(Square s)               { return File(s & 7); }
constexpr auto rank(Square s)               { return Rank(s >> 3); }
constexpr auto sq(Rank r, File f)           { return Square((r << 3) + f); }

constexpr bool get(Bitboard bb, Square s)   { return bb & bit(s); }
constexpr void set(Bitboard &bb, Square s)  { bb |=  bit(s); }
constexpr void clr(Bitboard &bb, Square s)  { bb &= ~bit(s); }

#if __cplusplus >= 202002L
constexpr auto cnt(Bitboard bb)             { return std::popcount(bb); }
constexpr auto lsb(Bitboard bb)             { return std::countr_zero(bb); }
#else
constexpr auto cnt(Bitboard bb)
{ 
    bb -= (bb >> 1) & 0x5555555555555555;
    bb  = (bb & 0x3333333333333333) + ((bb >> 2) & 0x3333333333333333);
    return (((bb + (bb >> 4)) & 0xF0F0F0F0F0F0F0F) * 0x101010101010101) >> 56;
}
constexpr auto lsb(Bitboard bb)
{ 
    return Square(cnt((bb & -bb) - 1));  
}
#endif

template<class T, T Head, T Tail>
struct EnumIter {

    constexpr EnumIter() = default;
    constexpr EnumIter(int val) : val{val} {}

    constexpr EnumIter begin()  { return Head; }
    constexpr EnumIter end()    { return Tail + 1; }

    constexpr bool  operator!=(const EnumIter &i) const { return val != i.val; }
    constexpr void  operator++()                        { ++val; }
    constexpr T     operator*() const                   { return T(val); }
private:
    int val = Head;
};

using Squares = EnumIter<Square, A1, H8>;

}

#endif