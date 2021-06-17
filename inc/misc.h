#ifndef MISC_H
#define MISC_H

#include <array>

#define ENABLE_INC(T)    \
constexpr T& operator++(T &d)   { return d = T(int(d) + 1); }    \
constexpr T& operator--(T &d)   { return d = T(int(d) - 1); }

ENABLE_INC(File)
ENABLE_INC(Rank)
ENABLE_INC(Square)

constexpr File file(Square s)               { return File(s & 7); }
constexpr Rank rank(Square s)               { return Rank(s >> 3); }
constexpr Square sq(Rank r, File f)         { return Square((r << 3) + f); }
constexpr Bitboard bit(Square s)            { return 1ul << s; }

constexpr void set(Bitboard &bb, Square s)  { bb |= bit(s); }
constexpr void clr(Bitboard &bb, Square s)  { bb &= ~bit(s); }
constexpr bool get(Bitboard bb, Square s)   { return bb & bit(s); }

constexpr int cnt(Bitboard bb)
{ 
    bb -= (bb >> 1) & 0x5555555555555555;
    bb  = (bb & 0x3333333333333333) + ((bb >> 2) & 0x3333333333333333);
    return (((bb + (bb >> 4)) & 0xF0F0F0F0F0F0F0F) * 0x101010101010101) >> 56;
}
constexpr int lsb(Bitboard bb)              { return cnt((bb & -bb) - 1);  }

constexpr char file_c(File f)               { return 'a' + f; }
constexpr char rank_c(Rank r)               { return '1' + r; }

#endif // MISC_H