#ifndef BITBOARD_H
#define BITBOARD_H

#include <array>
#include "defs.h"

struct BitIter {

    constexpr BitIter(Bitboard val_) : val(val_) {}

    constexpr bool  operator!=(const BitIter &other) const  { return val != other.val; }
    constexpr void  operator++()                            { val &= val - 1; }
    constexpr auto  operator*() const                       { return lsb(val); }

    constexpr BitIter begin()   { return val; }
    constexpr BitIter end()     { return 0; }
private:
    Bitboard val;
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
    uint8_t shift       = 0;
};

constexpr Bitboard SQ_WHITE = 0x55aa55aa55aa55aaULL;
constexpr Bitboard SQ_BLACK = 0xaa55aa55aa55aa55ULL;
constexpr Bitboard SQ_EDGES = 0xff818181818181ffULL;
constexpr Bitboard SQ_ENPS  = 0x00000000000000ffULL;

constexpr Bitboard RANK_BB  = 0x00000000000000ffULL;
constexpr Bitboard FILE_BB  = 0x0101010101010101ULL;

constexpr Bitboard rank_bb(Rank r)          { return RANK_BB << (8 * r); }
constexpr Bitboard file_bb(File f)          { return FILE_BB << f; }

constexpr bool same_line(Square s1, Square s2)
{
    return rank(s1) == rank(s2) || file(s1) == file(s2);
}

constexpr bool same_diag(Square s1, Square s2)
{
    return  (rank(s2) - rank(s1)) == (file(s2) - file(s1)) ||
            (rank(s2) - rank(s1)) +  (file(s2) - file(s1)) == 0;
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

template<class Arr>
constexpr Bitboard shift(Bitboard b, Arr dirs)
{
    for (auto d : dirs)
        b = shift(b, d);
    return b;
}

template<class T, class ...Args>
constexpr Bitboard shift(Bitboard b, T d, Args... dirs)
{
    return shift(b, d) | shift(b, dirs...);
}

template<Piece P>
constexpr Bitboard attacks_sliding(Square s, Bitboard block)
{
    auto dir = P == BISHOP ? 
        std::array{ NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST } : 
        std::array{ NORTH,      EAST,       SOUTH,      WEST };

    Bitboard att = 0;
    Bitboard bb = bit(s);

    for (auto d : dir) {
        auto dst = bb;
        while ((dst = shift(dst, d))) {
            att |= dst;
            if (dst & block)
                break;
        }
    }
    return att;
}

template<Piece P>
constexpr Bitboard attacks_mask(Square s)
{
    Bitboard edges = 
        ((rank_bb(RANK_1) | rank_bb(RANK_8)) & ~rank_bb(rank(s))) | 
        ((file_bb(FILE_A) | file_bb(FILE_H)) & ~file_bb(file(s)));
    return attacks_sliding<P>(s, 0) & ~edges;
}

constexpr Bitboard attacks_occupancy(Bitboard idx, Bitboard mask)
{
    int n           = 0;
    Bitboard occ    = 0;
    for (auto b : BitIter(mask)) {
        if (bit(n++) & idx)
            set(occ, b);
    }
    return occ;
}

constexpr auto attacks_pawn()
{
    std::array<Bitboard[SQ_num], 2> att = {};

    for (Square s = A1; s <= H8; ++s) {

        auto bb = bit(s);

        att[WHITE][s] = shift(bb, NORTH_EAST, NORTH_WEST);
        att[BLACK][s] = shift(bb, SOUTH_EAST, SOUTH_WEST);  
    }
    return att;
}

template<Piece P>
constexpr auto attacks()
{
    static_assert(
        P != PAWN && 
        P < PIECE_num, 
        "Unsupported piece");

    std::array<Bitboard, SQ_num> att = {};

    for (Square s = A1; s <= H8; ++s) {

        auto bb = bit(s);

        switch (P) {
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
            case BISHOP: att[s] = attacks_sliding<BISHOP>(s, 0);    break;
            case ROOK:   att[s] = attacks_sliding<ROOK  >(s, 0);    break;
            case QUEEN:  att[s] = attacks_sliding<BISHOP>(s, 0) | 
                                  attacks_sliding<ROOK  >(s, 0);
                break;
            case KING:
                att[s] = shift(bb, NORTH, EAST, SOUTH, WEST, NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST);   
                break;
        }
    }
    return att;
}

template<Piece P>
auto attacks_magic(const Bitboard *magic_nums)
{
    static_assert(P == BISHOP || P == ROOK, "Only for bishop and rook");

    static Bitboard table[P == ROOK ? 102400 : 5248];
    std::array<Magic, 64> magics = {};

    size_t offset = 0;

    for (Square s = A1; s <= H8; ++s) {

        const auto mask     = attacks_mask<P>(s);
        const auto n_bits   = cnt(mask);

        magics[s].mask      = mask;
        magics[s].shift     = 64 - n_bits;
        magics[s].magic     = magic_nums[s];
        magics[s].attacks   = &table[offset];

        // Build square attacks table for every possible relevant occupancy.
        for (Bitboard idx = 0; idx < bit(n_bits); ++idx) {

            auto occ = attacks_occupancy(idx, mask);
            auto att = attacks_sliding<P>(s, occ);

            occ *=  magics[s].magic;
            occ >>= magics[s].shift;

            table[offset + occ] = att;
        }
        offset += bit(n_bits);
    }
    return magics;
}

constexpr auto betweens()
{
    std::array<Bitboard[SQ_num], SQ_num> bb = {};

    for (Square src = A1; src <= H8; ++src) {
        for (Square dst = A1; dst <= H8; ++dst) {

            if (same_line(src, dst)) {
                bb[src][dst] =  attacks_sliding<ROOK>(src, bit(dst)) & 
                                attacks_sliding<ROOK>(dst, bit(src));
                // set(bb[src][dst], dst);
            } else 
            if (same_diag(src, dst)) {
                bb[src][dst] =  attacks_sliding<BISHOP>(src, bit(dst)) & 
                                attacks_sliding<BISHOP>(dst, bit(src));
                // set(bb[src][dst], dst);
            }
        }
    }
    return bb;
}

constexpr auto lines()
{
    std::array<Bitboard[SQ_num], SQ_num> bb = {};

    for (Square src = A1; src <= H8; ++src) {
        for (Square dst = A1; dst <= H8; ++dst) {

            if (same_line(src, dst)) {
                bb[src][dst] =  attacks_sliding<ROOK>(src, 0) & 
                                attacks_sliding<ROOK>(dst, 0);
                set(bb[src][dst], src);
                set(bb[src][dst], dst);
            } else 
            if (same_diag(src, dst)) {
                bb[src][dst] =  attacks_sliding<BISHOP>(src, 0) & 
                                attacks_sliding<BISHOP>(dst, 0);
                set(bb[src][dst], src);
                set(bb[src][dst], dst);
            }
        }
    }

    return bb;
}

#endif