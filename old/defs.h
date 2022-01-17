#ifndef DEFS_H
#define DEFS_H

#include <cstdint>
#include <cstddef>

#define SIZE(x)     (sizeof(x) / sizeof(x[0]))
#define NULL_MOVE   0
#define MAX_MOVES   412
#define MAX_DEPTH   32

using Bitboard  = uint64_t;
using Move      = uint16_t;
using Score     = int;

enum Piece  { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, PIECE_num };
enum File   { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_num };
enum Rank   { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_num };
enum Color  : uint_fast8_t { WHITE, BLACK, COLOR_num };
enum Square : uint_fast8_t {  
    A1, B1, C1, D1, E1, F1, G1, H1, 
    A2, B2, C2, D2, E2, F2, G2, H2, 
    A3, B3, C3, D3, E3, F3, G3, H3, 
    A4, B4, C4, D4, E4, F4, G4, H4, 
    A5, B5, C5, D5, E5, F5, G5, H5, 
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8, 
    SQ_num,
};
enum Castle : uint_fast8_t { 
    CASTLE_NO, 
    WKCA        = 0b0001,
    WQCA        = 0b0010, 
    BKCA        = 0b0100, 
    BQCA        = 0b1000,
    KCA         = WKCA | BKCA,
    QCA         = WQCA | BQCA,
    WCA         = WKCA | WQCA,
    BCA         = BKCA | BQCA,
    ANY_CA      = WCA  | BCA,
    CASTLE_num,
};
enum Direction {
    NORTH       = 8,
    EAST        = 1,
    SOUTH       = -NORTH,
    WEST        = -EAST,
    NORTH_EAST  = NORTH + EAST,
    SOUTH_EAST  = SOUTH + EAST,
    SOUTH_WEST  = SOUTH + WEST,
    NORTH_WEST  = NORTH + WEST,
};
enum MoveFlag { 
    QUIET   = 0b0000'0000'0000'0000, 
    PUSH    = 0b0001'0000'0000'0000,
    K_CAST  = 0b0010'0000'0000'0000,
    Q_CAST  = 0b0011'0000'0000'0000,
    N_PROM  = 0b0100'0000'0000'0000,
    B_PROM  = 0b0101'0000'0000'0000,
    R_PROM  = 0b0110'0000'0000'0000,
    Q_PROM  = 0b0111'0000'0000'0000,
    ENPASS  = 0b1000'0000'0000'0000,
};

#define ENABLE_INC(T)       \
constexpr T& operator++(T &t)       { return t = T(t + 1); }        \
constexpr T& operator--(T &t)       { return t = T(t - 1); }        \
constexpr T operator++(T &t, int)   { T old = t; ++t; return old; } \
constexpr T operator--(T &t, int)   { T old = t; --t; return old; }

#define ENABLE_MATH(T, D)   \
constexpr T& operator+=(T &t, D d)  { return t = T(t + d); }    \
constexpr T& operator-=(T &t, D d)  { return t = T(t - d); }    \
constexpr T operator+(T t, D d)     { return T(int(t) + d); }   \
constexpr T operator-(T t, D d)     { return T(int(t) - d); }    

#define ENABLE_BITWISE(T, D)    \
constexpr T& operator|=(T &t, D d)  { return t = T(t | d); }    \
constexpr T& operator&=(T &t, D d)  { return t = T(t & d); }    \
constexpr T& operator^=(T &t, D d)  { return t = T(t ^ d); }    \
constexpr T operator|(T t, D d)     { return T(int(t) | d); }   \
constexpr T operator&(T t, D d)     { return T(int(t) & d); }   \
constexpr T operator^(T t, D d)     { return T(int(t) ^ d); }   

ENABLE_INC(File)
ENABLE_INC(Rank)
ENABLE_INC(Square)
ENABLE_MATH(Square, Direction)
ENABLE_BITWISE(Castle, int)
ENABLE_BITWISE(MoveFlag, MoveFlag)

// NOTE: Could take Square for type safety, but there are functions which
// realy use int. Casting them explicitly to Square would be misleading. 
constexpr Bitboard bit(int i)               { return 1ul << i; }
constexpr File file(Square s)               { return File(s & 7); }
constexpr Rank rank(Square s)               { return Rank(s >> 3); }
constexpr Square sq(Rank r, File f)         { return Square((r << 3) + f); }
constexpr Square from(Move m)               { return Square(m & 0x3f); }
constexpr Square to(Move m)                 { return from(m >> 6); }
constexpr MoveFlag flag(Move m)             { return MoveFlag(m & 0xf000); }
constexpr Move mv(Square src, Square dst)   { return (dst << 6) | src; }
constexpr Move mv(Square src, Square dst, MoveFlag flag)   
{ 
    return flag | (dst << 6) | src; 
}
constexpr void set(Bitboard &bb, Square s)  { bb |= bit(s); }
constexpr void clr(Bitboard &bb, Square s)  { bb &= ~bit(s); }
constexpr bool get(Bitboard bb, Square s)   { return bb & bit(s); }
constexpr int cnt(Bitboard bb)
{ 
    bb -= (bb >> 1) & 0x5555555555555555;
    bb  = (bb & 0x3333333333333333) + ((bb >> 2) & 0x3333333333333333);
    return (((bb + (bb >> 4)) & 0xF0F0F0F0F0F0F0F) * 0x101010101010101) >> 56;
}
constexpr Square lsb(Bitboard bb)           { return Square(cnt((bb & -bb) - 1));  }
constexpr Color operator~(Color c)          { return Color(c ^ BLACK); }

struct MoveList {
    constexpr const Move* begin() const                         { return &list[0]; }
    constexpr const Move* end() const                           { return &list[len]; }
    constexpr Move* begin()                                     { return &list[0]; }
    constexpr Move* end()                                       { return &list[len]; }
    constexpr size_t size() const                               { return len; }
    constexpr void erase(size_t idx)                            { list[idx] = list[--len]; }
    constexpr void save(Move m)                                 { list[len++] = m; }
    constexpr void save(Square src, Square dst)                 { save(mv(src, dst)); }
    constexpr void save(Square src, Square dst, MoveFlag flag)  { save(mv(src, dst, flag)); }
private:
    Move list[MAX_MOVES];
    size_t len = 0;
};

#endif