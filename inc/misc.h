#ifndef MISC_H
#define MISC_H

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

constexpr bool same_line(Square s1, Square s2)
{
    return rank(s1) == rank(s2) || file(s1) == file(s2);
}
constexpr bool same_diag(Square s1, Square s2)
{
    return  (rank(s2) - rank(s1)) == (file(s2) - file(s1)) ||
            (rank(s2) - rank(s1)) +  (file(s2) - file(s1)) == 0;
}

constexpr char file_c(File f)               { return 'a' + f; }
constexpr char rank_c(Rank r)               { return '1' + r; }
constexpr char side_c(Color c)              { return c == WHITE ? 'w' : 'b'; }

#endif // MISC_H