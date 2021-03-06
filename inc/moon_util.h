#ifndef MOON_UTIL_H
#define MOON_UTIL_H

#include <bit>
#include "moon_type.h"

namespace moon {

constexpr Bitboard bit(int i)               { return 1ul << i; }
constexpr Bitboard bit(int i, auto... more) { return bit(i) | bit(more...); }
constexpr Bitboard rank_bb(Rank r)          { return 0x00000000000000ffULL << (r << 3); }
constexpr Bitboard file_bb(File f)          { return 0x0101010101010101ULL << f; }
constexpr auto operator~(Color c)           { return Color(c ^ BLACK); }
constexpr auto file(Square s)               { return File(s & 7); }
constexpr auto rank(Square s)               { return Rank(s >> 3); }
constexpr auto sq(Rank r, File f)           { return Square((r << 3) + f); }
constexpr auto from(Move m)                 { return Square(m & 0x3f); }
constexpr auto to(Move m)                   { return from(m >> 6); }
constexpr auto flag(Move m)                 { return m & 0xf000; }
constexpr bool get(Bitboard bb, Square s)   { return bb & bit(s); }
constexpr void set(Bitboard &bb, Square s)  { bb |=  bit(s); }
constexpr void clr(Bitboard &bb, Square s)  { bb &= ~bit(s); }
constexpr auto cnt(Bitboard bb)             { return std::popcount(bb); }
constexpr auto lsb(Bitboard bb)             { return std::countr_zero(bb); }

constexpr Move mv(Square s, Square d)
{ 
    return (d << 6) | s; 
}

constexpr Move mv(Square s, Square d, Flag flag) 
{ 
    return (d << 6) | s | flag; 
}

constexpr bool same_line(Square s1, Square s2)
{
    return rank(s1) == rank(s2) || file(s1) == file(s2);
}

constexpr bool same_diag(Square s1, Square s2)
{
    return  rank(s2) - rank(s1) == file(s2) - file(s1) ||
            rank(s2) - rank(s1) +  file(s2) - file(s1) == 0;
}

constexpr auto operator+(Square s, Direction d) 
{
    return Square(int(s) + d);
}

constexpr auto operator-(Square s, Direction d) 
{ 
    return Square(int(s) - d); 
}

struct BitIter {
    constexpr BitIter(Bitboard bb) : bb{bb} {}
    constexpr BitIter begin()           { return bb; }
    constexpr BitIter end()             { return 0; }
    constexpr auto operator*() const    { return Square(lsb(bb)); }
    constexpr auto operator++()         { bb &= bb - 1; }
    constexpr bool operator!=(const BitIter&) const = default;
    constexpr bool operator==(const BitIter&) const = default;
private:
    Bitboard bb;
};

template<class T, T Head, T Tail>
struct EnumIter {
    constexpr EnumIter() = default;
    constexpr EnumIter(int val) : val{val} {}
    constexpr EnumIter begin()          { return Head; }
    constexpr EnumIter end()            { return Tail + 1; }
    constexpr auto operator*() const    { return T(val); }
    constexpr auto operator++()         { ++val; }
    constexpr bool operator!=(const EnumIter &i) const = default;
    constexpr bool operator==(const EnumIter &i) const = default;
protected:
    int val = Head;
};

template<class T, T Head, T Tail>
struct EnumIterRev : EnumIter<T, Tail, Head> {
    constexpr EnumIterRev() = default;
    constexpr EnumIterRev(int val) : EnumIter<T, Tail, Head>{val} {}
    constexpr EnumIterRev begin()   { return Tail; }
    constexpr EnumIterRev end()     { return Head - 1; }
    constexpr void operator++()     { --this->val; }
};

using Squares   = EnumIter<Square, A1, H8>;
using Files     = EnumIter<File, FILE_A, FILE_H>;
using Ranks     = EnumIter<Rank, RANK_1, RANK_8>;
using RanksRev  = EnumIterRev<Rank, RANK_1, RANK_8>;

}

#endif