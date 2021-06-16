#ifndef MISC_H
#define MISC_H

#include <cstdio>
#include <array>

#define PRINT   printf
#define PUTC    putchar

#if !defined(PRINT) || !defined(PUTC)
#define PRINT(...)
#define PUTC(...)
#warning "To enable printing define PRINT and PUTC as output mechanisms";
#endif

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

constexpr const char *RANK_STR = "12345678";
constexpr const char *FILE_STR = "abcdefgh";

#endif // MISC_H