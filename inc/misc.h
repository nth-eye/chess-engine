#ifndef MISC_H
#define MISC_H

#include <cstdio>
// #include <array>

#define PRINT   printf
#define PUTC    putchar

#if !defined(PRINT) || !defined(PUTC)
#define PRINT(...)
#define PUTC(...)
#warning "To enable printing define PRINT and PUTC as output mechanisms";
#endif

constexpr File& operator++(File &d)         { return d = File(int(d) + 1); }
constexpr File& operator--(File &d)         { return d = File(int(d) - 1); }
constexpr Rank& operator++(Rank &d)         { return d = Rank(int(d) + 1); }
constexpr Rank& operator--(Rank &d)         { return d = Rank(int(d) - 1); }
constexpr Square& operator++(Square &d)     { return d = Square(int(d) + 1); }
constexpr Square& operator--(Square &d)     { return d = Square(int(d) - 1); }

constexpr File file(Square s)               { return File(s & 7); }
constexpr Rank rank(Square s)               { return Rank(s >> 3); }
constexpr Square square(Rank r, File f)     { return Square((r << 3) + f); }
constexpr Bitboard bit(Square sq)           { return 1 << sq; }

constexpr void set(Bitboard &bb, Square sq) { bb |= bit(sq); }
constexpr void clr(Bitboard &bb, Square sq) { bb &= ~bit(sq); }
constexpr bool get(Bitboard bb, Square sq)  { return bb & bit(sq); }

#endif // MISC_H