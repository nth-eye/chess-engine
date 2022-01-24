#ifndef MOON_BITBOARD_H
#define MOON_BITBOARD_H

#include <array>
#include <cassert>
#include "moon_util.h"

namespace moon {

constexpr Bitboard shift(Bitboard b, Direction d)
{
    switch (d) {
        case NORTH:             return  b << 8;
        case SOUTH:             return  b >> 8;
        case EAST:              return (b & ~file_bb(FILE_H)) << 1;
        case WEST:              return (b & ~file_bb(FILE_A)) >> 1;
        case NORTH_EAST:        return (b & ~file_bb(FILE_H)) << 9;
        case NORTH_WEST:        return (b & ~file_bb(FILE_A)) << 7;
        case SOUTH_EAST:        return (b & ~file_bb(FILE_H)) >> 7;
        case SOUTH_WEST:        return (b & ~file_bb(FILE_A)) >> 9;
        case NORTH_NORTH_EAST:  return (b & ~file_bb(FILE_H)) << 17;
        case NORTH_NORTH_WEST:  return (b & ~file_bb(FILE_A)) << 15;
        case SOUTH_SOUTH_EAST:  return (b & ~file_bb(FILE_H)) >> 15;
        case SOUTH_SOUTH_WEST:  return (b & ~file_bb(FILE_A)) >> 17;
        case NORTH_EAST_EAST:   return (b & ~(file_bb(FILE_H) | file_bb(FILE_G))) << 10;
        case NORTH_WEST_WEST:   return (b & ~(file_bb(FILE_A) | file_bb(FILE_B))) << 6;
        case SOUTH_EAST_EAST:   return (b & ~(file_bb(FILE_H) | file_bb(FILE_G))) >> 6;
        case SOUTH_WEST_WEST:   return (b & ~(file_bb(FILE_A) | file_bb(FILE_B))) >> 10;
    }
    assert(false);
}

constexpr auto shift(Bitboard b, auto d, auto... dirs)
{
    return shift(b, d) | shift(b, dirs...);
}

constexpr auto attacks_sliding(Piece p, Square s, Bitboard block)
{
    Bitboard att = 0;

    for (auto d : p == BISHOP ? 
        std::array{ NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST } : 
        std::array{ NORTH,      EAST,       SOUTH,      WEST }) 
    {
        auto dst = bit(s);
        do {
            att |= dst = shift(dst, d);
        } while (dst && !(dst & block));
    }
    return att;
}

constexpr auto attacks(Piece p, Color c = WHITE)
{
    std::array<Bitboard, 64> att = {};

    for (auto s : Squares()) {
        auto bb = bit(s);
        switch (p) {
            case PAWN:
                att[s] = c == WHITE ?
                    shift(bb, NORTH_EAST, NORTH_WEST): 
                    shift(bb, SOUTH_EAST, SOUTH_WEST); 
                break;
            case KNIGHT:
                att[s] = shift(bb,
                    NORTH_NORTH_EAST,
                    NORTH_NORTH_WEST,
                    NORTH_EAST_EAST,
                    NORTH_WEST_WEST,
                    SOUTH_SOUTH_EAST,
                    SOUTH_SOUTH_WEST,
                    SOUTH_EAST_EAST,
                    SOUTH_WEST_WEST);
                break;
            case KING:
                att[s] = shift(bb, NORTH, EAST, SOUTH, WEST, NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST);   
                break;
            default: assert(false);
        }
    }
    return att;
}

template<Piece P>
struct Magic {
    constexpr Magic() = default;
    constexpr auto operator[](Bitboard occ) const   { return att[idx(occ)]; }
    constexpr auto idx(Bitboard occ) const          { return ((occ & mask) * magic) >> shift; }
    Bitboard magic  = 0;
    Bitboard mask   = 0;
    Bitboard *att   = nullptr;
    static constexpr int shift = P == ROOK ? 52 : 55;
};

template<Piece P>
auto attacks_magic(Bitboard *table)
{
    struct MagicInit {
        Bitboard num;
        int idx;
    };
    constexpr MagicInit magic_init_bishop[64] = {
        { 0x007fbfbfbfbfbfff,   5378 }, { 0x0000a060401007fc,   4093 }, { 0x0001004008020000,   4314 }, { 0x0000806004000000,   6587 },
        { 0x0000100400000000,   6491 }, { 0x000021c100b20000,   6330 }, { 0x0000040041008000,   5609 }, { 0x00000fb0203fff80,  22236 },
        { 0x0000040100401004,   6106 }, { 0x0000020080200802,   5625 }, { 0x0000004010202000,  16785 }, { 0x0000008060040000,  16817 },
        { 0x0000004402000000,   6842 }, { 0x0000000801008000,   7003 }, { 0x000007efe0bfff80,   4197 }, { 0x0000000820820020,   7356 },
        { 0x0000400080808080,   4602 }, { 0x00021f0100400808,   4538 }, { 0x00018000c06f3fff,  29531 }, { 0x0000258200801000,  45393 },
        { 0x0000240080840000,  12420 }, { 0x000018000c03fff8,  15763 }, { 0x00000a5840208020,   5050 }, { 0x0000020008208020,   4346 },
        { 0x0000804000810100,   6074 }, { 0x0001011900802008,   7866 }, { 0x0000804000810100,  32139 }, { 0x000100403c0403ff,  57673 },
        { 0x00078402a8802000,  55365 }, { 0x0000101000804400,  15818 }, { 0x0000080800104100,   5562 }, { 0x00004004c0082008,   6390 },
        { 0x0001010120008020,   7930 }, { 0x000080809a004010,  13329 }, { 0x0007fefe08810010,   7170 }, { 0x0003ff0f833fc080,  27267 },
        { 0x007fe08019003042,  53787 }, { 0x003fffefea003000,   5097 }, { 0x0000101010002080,   6643 }, { 0x0000802005080804,   6138 },
        { 0x0000808080a80040,   7418 }, { 0x0000104100200040,   7898 }, { 0x0003ffdf7f833fc0,  42012 }, { 0x0000008840450020,  57350 },
        { 0x00007ffc80180030,  22813 }, { 0x007fffdd80140028,  56693 }, { 0x00020080200a0004,   5818 }, { 0x0000101010100020,   7098 },
        { 0x0007ffdfc1805000,   4451 }, { 0x0003ffefe0c02200,   4709 }, { 0x0000000820806000,   4794 }, { 0x0000000008403000,  13364 },
        { 0x0000000100202000,   4570 }, { 0x0000004040802000,   4282 }, { 0x0004010040100400,  14964 }, { 0x00006020601803f4,   4026 },
        { 0x0003ffdfdfc28048,   4826 }, { 0x0000000820820020,   7354 }, { 0x0000000008208060,   4848 }, { 0x0000000000808020,  15946 },
        { 0x0000000001002020,  14932 }, { 0x0000000401002008,  16588 }, { 0x0000004040404040,   6905 }, { 0x007fff9fdf7ff813,  16076 },
    };
    constexpr MagicInit magic_init_rook[64] = {
        { 0x00280077ffebfffe,  26304 }, { 0x2004010201097fff,  35520 }, { 0x0010020010053fff,  38592 }, { 0x0040040008004002,   8026 },
        { 0x7fd00441ffffd003,  22196 }, { 0x4020008887dffffe,  80870 }, { 0x004000888847ffff,  76747 }, { 0x006800fbff75fffd,  30400 },
        { 0x000028010113ffff,  11115 }, { 0x0020040201fcffff,  18205 }, { 0x007fe80042ffffe8,  53577 }, { 0x00001800217fffe8,  62724 },
        { 0x00001800073fffe8,  34282 }, { 0x00001800e05fffe8,  29196 }, { 0x00001800602fffe8,  23806 }, { 0x000030002fffffa0,  49481 },
        { 0x00300018010bffff,   2410 }, { 0x0003000c0085fffb,  36498 }, { 0x0004000802010008,  24478 }, { 0x0004002020020004,  10074 },
        { 0x0001002002002001,  79315 }, { 0x0001001000801040,  51779 }, { 0x0000004040008001,  13586 }, { 0x0000006800cdfff4,  19323 },
        { 0x0040200010080010,  70612 }, { 0x0000080010040010,  83652 }, { 0x0004010008020008,  63110 }, { 0x0000040020200200,  34496 },
        { 0x0002008010100100,  84966 }, { 0x0000008020010020,  54341 }, { 0x0000008020200040,  60421 }, { 0x0000820020004020,  86402 },
        { 0x00fffd1800300030,  50245 }, { 0x007fff7fbfd40020,  76622 }, { 0x003fffbd00180018,  84676 }, { 0x001fffde80180018,  78757 },
        { 0x000fffe0bfe80018,  37346 }, { 0x0001000080202001,    370 }, { 0x0003fffbff980180,  42182 }, { 0x0001fffdff9000e0,  45385 },
        { 0x00fffefeebffd800,  61659 }, { 0x007ffff7ffc01400,  12790 }, { 0x003fffbfe4ffe800,  16762 }, { 0x001ffff01fc03000,      0 },
        { 0x000fffe7f8bfe800,  38380 }, { 0x0007ffdfdf3ff808,  11098 }, { 0x0003fff85fffa804,  21803 }, { 0x0001fffd75ffa802,  39189 },
        { 0x00ffffd7ffebffd8,  58628 }, { 0x007fff75ff7fbfd8,  44116 }, { 0x003fff863fbf7fd8,  78357 }, { 0x001fffbfdfd7ffd8,  44481 },
        { 0x000ffff810280028,  64134 }, { 0x0007ffd7f7feffd8,  41759 }, { 0x0003fffc0c480048,   1394 }, { 0x0001ffffafd7ffd8,  40910 },
        { 0x00ffffe4ffdfa3ba,  66516 }, { 0x007fffef7ff3d3da,   3897 }, { 0x003fffbfdfeff7fa,   3930 }, { 0x001fffeff7fbfc22,  72934 },
        { 0x0000020408001001,  72662 }, { 0x0007fffeffff77fd,  56325 }, { 0x0003ffffbf7dfeec,  66501 }, { 0x0001ffff9dffa333,  14826 },
    };
    std::array<Magic<P>, 64> magics;

    auto init = P == ROOK ? magic_init_rook : magic_init_bishop;

    for (auto s : Squares()) {

        auto edges = 
            ((rank_bb(RANK_1) | rank_bb(RANK_8)) & ~rank_bb(rank(s))) | 
            ((file_bb(FILE_A) | file_bb(FILE_H)) & ~file_bb(file(s)));
        auto mask = attacks_sliding(P, s, 0) & ~edges;
        auto att = &table[init[s].idx];

        magics[s].magic = init[s].num;
        magics[s].mask  = mask;
        magics[s].att   = att;

        Bitboard occ = 0; // NOTE: Don't use auto
        do {
            att[magics[s].idx(occ)] = attacks_sliding(P, s, occ);
        } while ((occ = (occ - mask) & mask));  // Carry-Rippler method to iterate over all subsets of mask
    }
    return magics;
}

constexpr auto betweens()
{
    std::array<Bitboard[64], 64> bb = {};

    for (auto src : Squares()) {
        for (auto dst : Squares()) {
            if (same_line(src, dst)) {
                bb[src][dst] =  attacks_sliding(ROOK, src, bit(dst)) & 
                                attacks_sliding(ROOK, dst, bit(src));
            } else 
            if (same_diag(src, dst)) {
                bb[src][dst] =  attacks_sliding(BISHOP, src, bit(dst)) & 
                                attacks_sliding(BISHOP, dst, bit(src));
            }
        }
    }
    return bb;
}

constexpr auto lines()
{
    std::array<Bitboard[64], 64> bb = {};

    for (auto src : Squares()) {
        for (auto dst : Squares()) {
            if (same_line(src, dst)) {
                bb[src][dst] =  attacks_sliding(ROOK, src, 0) & 
                                attacks_sliding(ROOK, dst, 0);
                set(bb[src][dst], src);
                set(bb[src][dst], dst);
            } else 
            if (same_diag(src, dst)) {
                bb[src][dst] =  attacks_sliding(BISHOP, src, 0) & 
                                attacks_sliding(BISHOP, dst, 0);
                set(bb[src][dst], src);
                set(bb[src][dst], dst);
            }
        }
    }
    return bb;
}

}

#endif