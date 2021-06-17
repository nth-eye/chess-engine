#ifndef BITBOARD_H
#define BITBOARD_H

#include "defs.h"
#include "misc.h"

template <typename T>
struct BitIter {

    constexpr BitIter(T val_) : val(val_) {}

    constexpr bool  operator!=(const BitIter &other)    { return val != other.val; }
    constexpr void  operator++()                        { val &= val - 1; }
    constexpr int   operator*() const                   { return lsb(val); }

    constexpr BitIter begin()   { return val; }
    constexpr BitIter end()     { return 0; }
private:
    T val;
};

// Structure holding all relevant magic parameters per square.
// template<size_t S>
struct Magic {
    
    constexpr Magic() = default;
    constexpr Bitboard operator[](Bitboard block) const 
    {
        return attacks[((block & mask) * magic) >> shift];
    };
    // static constexpr Bitboard attacks[S] = {};
    Bitboard mask       = 0;
    Bitboard magic      = 0;
    Bitboard *attacks   = nullptr;
    uint8_t shift       = 0;
};

constexpr Bitboard SQ_WHITE = 0x55aa55aa55aa55aaULL;
constexpr Bitboard SQ_BLACK = 0xaa55aa55aa55aa55ULL;
constexpr Bitboard SQ_EDGES = 0xff818181818181ffULL;

constexpr Bitboard RANK_BB = 0xffULL;
constexpr Bitboard FILE_BB = 0x0101010101010101ULL;

constexpr Bitboard rank_bb(Rank r)          { return RANK_BB << (8 * r); }
constexpr Bitboard rank_bb(Square s)        { return rank_bb(rank(s)); }
constexpr Bitboard file_bb(File f)          { return FILE_BB << f; }
constexpr Bitboard file_bb(Square s)        { return file_bb(file(s)); }

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
    const auto dir = P == BISHOP ? 
        std::array{ NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST } : 
        std::array{ NORTH,      EAST,       SOUTH,      WEST };

    Bitboard att = 0;
    Bitboard bb = bit(s);

    for (const auto d : dir) {
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
    const auto edges = 
        ((rank_bb(RANK_1) | rank_bb(RANK_8)) & ~rank_bb(s)) | 
        ((file_bb(FILE_A) | file_bb(FILE_H)) & ~file_bb(s));
    return attacks_sliding<P>(s, 0) & ~edges;
}

constexpr Bitboard attacks_occupancy(int idx, Bitboard mask)
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
    std::array<std::array<Bitboard, SQ_num>, 2> att = {};

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
inline auto attacks_magic(const Bitboard *magic_nums)
{
    static_assert(P == BISHOP || P == ROOK, "Only for bishop and rook");

    // static std::array<Magic<P == ROOK ? 102400 : 5248, 64>> magics;
    static std::array<Magic, 64> magics = {};
    static std::array<Bitboard, P == ROOK ? 102400 : 5248> table = {};

    size_t offset = 0;

    for (Square s = A1; s <= H8; ++s) {

        const auto mask     = attacks_mask<P>(s);
        const auto num_bits = cnt(magics[s].mask);

        magics[s].mask      = mask;
        magics[s].shift     = 64 - num_bits;
        magics[s].magic     = magic_nums[s];
        magics[s].attacks   = &table[offset];

        // Build square attacks table for every possible relevant occupancy.
        for (int idx = 0; idx < bit(num_bits); ++idx) {

            const auto occ = attacks_occupancy(idx, mask);
            const auto att = attacks_sliding<P>(s, occ);

            occ *=  magics[s].magic;
            occ >>= magics[s].shift;

            table[offset + occ] = attacks;
        }
        offset += bit(num_bits);
    }
    return std::pair{ &magics, &table };
}

#endif // BITBOARD_H