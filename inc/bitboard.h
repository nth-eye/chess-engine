#ifndef BITBOARD_H
#define BITBOARD_H

#include "defs.h"
#include "misc.h"
#include <array>

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

constexpr Bitboard R_MAGIC_NUM[64] = {
    0x088000102088C001ULL, 0x10C0200040001000ULL, 0x83001041000B2000ULL, 0x0680280080041000ULL, 
    0x488004000A080080ULL, 0x0100180400010002ULL, 0x040001C401021008ULL, 0x02000C04A980C302ULL, 
    0x0000800040082084ULL, 0x5020C00820025000ULL, 0x0001002001044012ULL, 0x0402001020400A00ULL,
    0x00C0800800040080ULL, 0x4028800200040080ULL, 0x00A0804200802500ULL, 0x8004800040802100ULL, 
    0x0080004000200040ULL, 0x1082810020400100ULL, 0x0020004010080040ULL, 0x2004818010042800ULL, 
    0x0601010008005004ULL, 0x4600808002001400ULL, 0x0010040009180210ULL, 0x020412000406C091ULL,
    0x040084228000C000ULL, 0x8000810100204000ULL, 0x0084110100402000ULL, 0x0046001A00204210ULL, 
    0x2001040080080081ULL, 0x0144020080800400ULL, 0x0840108400080229ULL, 0x0480308A0000410CULL, 
    0x0460324002800081ULL, 0x620080A001804000ULL, 0x2800802000801006ULL, 0x0002809000800800ULL,
    0x4C09040080802800ULL, 0x4808800C00800200ULL, 0x0200311004001802ULL, 0x0400008402002141ULL, 
    0x0410800140008020ULL, 0x000080C001050020ULL, 0x004080204A020010ULL, 0x0224201001010038ULL, 
    0x0109001108010004ULL, 0x0282004844020010ULL, 0x8228180110040082ULL, 0x0001000080C10002ULL,
    0x024000C120801080ULL, 0x0001406481060200ULL, 0x0101243200418600ULL, 0x0108800800100080ULL, 
    0x4022080100100D00ULL, 0x0000843040600801ULL, 0x8301000200CC0500ULL, 0x1000004500840200ULL, 
    0x1100104100800069ULL, 0x2001008440001021ULL, 0x2002008830204082ULL, 0x0010145000082101ULL,
    0x01A2001004200842ULL, 0x1007000608040041ULL, 0x000A08100203028CULL, 0x02D4048040290402ULL	};

constexpr Bitboard B_MAGIC_NUM[64] = {
    0x0008201802242020ULL, 0x0021040424806220ULL, 0x4006360602013080ULL, 0x0004410020408002ULL, 
    0x2102021009001140ULL, 0x08C2021004000001ULL, 0x6001031120200820ULL, 0x1018310402201410ULL, 
    0x401CE00210820484ULL, 0x001029D001004100ULL, 0x2C00101080810032ULL, 0x0000082581000010ULL,
    0x10000A0210110020ULL, 0x200002016C202000ULL, 0x0201018821901000ULL, 0x006A0300420A2100ULL, 
    0x0010014005450400ULL, 0x1008C12008028280ULL, 0x00010010004A0040ULL, 0x3000820802044020ULL, 
    0x0000800405A02820ULL, 0x8042004300420240ULL, 0x10060801210D2000ULL, 0x0210840500511061ULL,
    0x0008142118509020ULL, 0x0021109460040104ULL, 0x00A1480090019030ULL, 0x0102008808008020ULL, 
    0x884084000880E001ULL, 0x040041020A030100ULL, 0x3000810104110805ULL, 0x04040A2006808440ULL, 
    0x0044040404C01100ULL, 0x4122B80800245004ULL, 0x0044020502380046ULL, 0x0100400888020200ULL,
    0x01C0002060020080ULL, 0x4008811100021001ULL, 0x8208450441040609ULL, 0x0408004900008088ULL, 
    0x0294212051220882ULL, 0x000041080810E062ULL, 0x10480A018E005000ULL, 0x80400A0204201600ULL, 
    0x2800200204100682ULL, 0x0020200400204441ULL, 0x0A500600A5002400ULL, 0x801602004A010100ULL,
    0x0801841008040880ULL, 0x10010880C4200028ULL, 0x0400004424040000ULL, 0x0401000142022100ULL, 
    0x00A00010020A0002ULL, 0x1010400204010810ULL, 0x0829910400840000ULL, 0x0004235204010080ULL, 
    0x1002008143082000ULL, 0x11840044440C2080ULL, 0x2802A02104030440ULL, 0x6100000900840401ULL,
    0x1C20A15A90420200ULL, 0x0088414004480280ULL, 0x0000204242881100ULL, 0x0240080802809010ULL	};

constexpr Bitboard SQ_WHITE = 0x55aa55aa55aa55aaULL;
constexpr Bitboard SQ_BLACK = 0xaa55aa55aa55aa55ULL;
constexpr Bitboard SQ_EDGES = 0xff818181818181ffULL;
constexpr Bitboard SQ_ENPS  = 0x00000000000000ffULL;

constexpr Bitboard RANK_BB  = 0x00000000000000ffULL;
constexpr Bitboard FILE_BB  = 0x0101010101010101ULL;

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
        ((rank_bb(RANK_1) | rank_bb(RANK_8)) & ~rank_bb(s)) | 
        ((file_bb(FILE_A) | file_bb(FILE_H)) & ~file_bb(s));
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

#endif // BITBOARD_H