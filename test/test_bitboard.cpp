#include <gtest/gtest.h>
#include "moon_bitboard.h"

using namespace moon;

namespace {

Bitboard magic_table[88772];

}

TEST(Bitboard, Shift)
{
    auto bb = bit(B2);

    EXPECT_EQ(shift(bb, NORTH), bit(B3));
    EXPECT_EQ(shift(bb, SOUTH), bit(B1));
    EXPECT_EQ(shift(bb, EAST), bit(C2));
    EXPECT_EQ(shift(bb, WEST), bit(A2));
    EXPECT_EQ(shift(bb, NORTH_EAST), bit(C3));
    EXPECT_EQ(shift(bb, NORTH_WEST), bit(A3));
    EXPECT_EQ(shift(bb, SOUTH_EAST), bit(C1));
    EXPECT_EQ(shift(bb, SOUTH_WEST), bit(A1));

    bb = bit(A4, H4, D1, D8); 

    EXPECT_EQ(shift(bb, NORTH), bit(A5, H5, D2));
    EXPECT_EQ(shift(bb, SOUTH), bit(A3, H3, D7));
    EXPECT_EQ(shift(bb, EAST), bit(B4, E1, E8));
    EXPECT_EQ(shift(bb, WEST), bit(G4, C1, C8));
    EXPECT_EQ(shift(bb, NORTH_EAST), bit(B5, E2));
    EXPECT_EQ(shift(bb, NORTH_WEST), bit(G5, C2));
    EXPECT_EQ(shift(bb, SOUTH_EAST), bit(B3, E7));
    EXPECT_EQ(shift(bb, SOUTH_WEST), bit(G3, C7));
}

// TEST(Bitboard, ShiftMany)
// {
//     auto bb = bit(C3);
    
//     EXPECT_EQ(shift(bb, std::array{NORTH, NORTH, EAST}), bit(D5));
//     EXPECT_EQ(shift(bb, std::array{NORTH, NORTH, WEST}), bit(B5));
//     EXPECT_EQ(shift(bb, std::array{NORTH, EAST, EAST}), bit(E4));
//     EXPECT_EQ(shift(bb, std::array{NORTH, WEST, WEST}), bit(A4));
//     EXPECT_EQ(shift(bb, std::array{SOUTH, EAST, EAST}), bit(E2));
//     EXPECT_EQ(shift(bb, std::array{SOUTH, WEST, WEST}), bit(A2));
//     EXPECT_EQ(shift(bb, std::array{SOUTH, SOUTH, EAST}), bit(D1));
//     EXPECT_EQ(shift(bb, std::array{SOUTH, SOUTH, WEST}), bit(B1));
// }

TEST(Bitboard, ShiftPack)
{
    auto bb = bit(C3);

    EXPECT_EQ(shift(bb, NORTH, EAST, SOUTH, WEST), bit(C4, D3, C2, B3));
    EXPECT_EQ(shift(bb, NORTH_EAST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST), bit(D4, B4, D2, B2)); 
}

TEST(Bitboard, SlidingAttacksNoBlock)
{
    EXPECT_EQ(attacks_sliding(ROOK, A1, 0), bit(A2, A3, A4, A5, A6, A7, A8, B1, C1, D1, E1, F1, G1, H1));
    EXPECT_EQ(attacks_sliding(ROOK, G1, 0), bit(G2, G3, G4, G5, G6, G7, G8, A1, B1, C1, D1, E1, F1, H1));
    EXPECT_EQ(attacks_sliding(ROOK, H8, 0), bit(H1, H2, H3, H4, H5, H6, H7, A8, B8, C8, D8, E8, F8, G8));
    EXPECT_EQ(attacks_sliding(ROOK, B8, 0), bit(B1, B2, B3, B4, B5, B6, B7, A8, C8, D8, E8, F8, G8, H8));
    EXPECT_EQ(attacks_sliding(ROOK, D4, 0), bit(D1, D2, D3, D5, D6, D7, D8, A4, B4, C4, E4, F4, G4, H4));

    EXPECT_EQ(attacks_sliding(BISHOP, A1, 0), bit(B2, C3, D4, E5, F6, G7, H8));
    EXPECT_EQ(attacks_sliding(BISHOP, G1, 0), bit(H2, F2, E3, D4, C5, B6, A7));
    EXPECT_EQ(attacks_sliding(BISHOP, H8, 0), bit(A1, B2, C3, D4, E5, F6, G7));
    EXPECT_EQ(attacks_sliding(BISHOP, B8, 0), bit(A7, C7, D6, E5, F4, G3, H2));
    EXPECT_EQ(attacks_sliding(BISHOP, D4, 0), bit(A1, B2, C3, E5, F6, G7, H8, A7, B6, C5, E3, F2, G1));
}

TEST(Bitboard, SlidingAttacksWithBlock)
{
    EXPECT_EQ(attacks_sliding(ROOK, D4, bit(D1, A4, H4, D8)), bit(D1, D2, D3, D5, D6, D7, D8, A4, B4, C4, E4, F4, G4, H4));
    EXPECT_EQ(attacks_sliding(ROOK, D4, bit(D2, B4, G4, D7)), bit(D2, D3, D5, D6, D7, B4, C4, E4, F4, G4));
    EXPECT_EQ(attacks_sliding(ROOK, D4, bit(D1, D2, A4, B4, H4, G4, D7, D8)), bit(D2, D3, D5, D6, D7, B4, C4, E4, F4, G4));
    EXPECT_EQ(attacks_sliding(ROOK, D4, bit(D3, C4, D5, E4)), bit(D3, D5, C4, E4));

    EXPECT_EQ(attacks_sliding(BISHOP, D4, bit(A1, A7, G1, H8)), bit(A1, B2, C3, E5, F6, G7, H8, A7, B6, C5, E3, F2, G1));
    EXPECT_EQ(attacks_sliding(BISHOP, D4, bit(B2, B6, F2, G7)), bit(B2, C3, E5, F6, G7, B6, C5, E3, F2));
    EXPECT_EQ(attacks_sliding(BISHOP, D4, bit(A1, B2, A7, B6, G1, F2, H8, G7)), bit(B2, C3, E5, F6, G7, B6, C5, E3, F2));
    EXPECT_EQ(attacks_sliding(BISHOP, D4, bit(C3, C5, E3, E5)), bit(C3, C5, E3, E5));
}

TEST(Bitboard, PawnAttacks)
{
    auto att = std::array { 
        attacks(PAWN, WHITE), 
        attacks(PAWN, BLACK) 
    };

    EXPECT_EQ(att[WHITE][A7], bit(B8));
    EXPECT_EQ(att[WHITE][B7], bit(A8, C8));
    EXPECT_EQ(att[WHITE][H7], bit(G8));
    EXPECT_EQ(att[WHITE][H8], 0);

    EXPECT_EQ(att[BLACK][A2], bit(B1));
    EXPECT_EQ(att[BLACK][B2], bit(A1, C1));
    EXPECT_EQ(att[BLACK][H2], bit(G1));
    EXPECT_EQ(att[BLACK][H1], 0);
}

TEST(Bitboard, KnightAttacks)
{
    auto att = attacks(KNIGHT);

    EXPECT_EQ(att[A1], bit(B3, C2));
    EXPECT_EQ(att[B1], bit(A3, C3, D2));
    EXPECT_EQ(att[C1], bit(A2, B3, D3, E2));
    EXPECT_EQ(att[G1], bit(E2, F3, H3));
    EXPECT_EQ(att[H1], bit(F2, G3));
    EXPECT_EQ(att[A8], bit(B6, C7));
    EXPECT_EQ(att[B8], bit(A6, C6, D7));
    EXPECT_EQ(att[C8], bit(A7, B6, D6, E7));
    EXPECT_EQ(att[G8], bit(E7, F6, H6));
    EXPECT_EQ(att[H8], bit(F7, G6));
    EXPECT_EQ(att[C3], bit(A2, A4, B5, D5, E4, E2, D1, B1));
}

TEST(Bitboard, KingAttacks)
{
    auto att = attacks(KING);

    EXPECT_EQ(att[A1], bit(A2, B2, B1));
    EXPECT_EQ(att[B1], bit(A1, A2, B2, C2, C1));
    EXPECT_EQ(att[A2], bit(A3, B3, B2, B1, A1));
    EXPECT_EQ(att[B2], bit(A1, A2, A3, B3, C3, C2, C1, B1));
    EXPECT_EQ(att[H2], bit(H3, G3, G2, G1, H1));
    EXPECT_EQ(att[G1], bit(F1, F2, G2, H2, H1));
    EXPECT_EQ(att[H1], bit(G1, G2, H2));
}

TEST(Bitboard, BishopMagicAttacks)
{
    auto att = attacks_magic<BISHOP>(magic_table);
    for (auto s : Squares()) {
        auto edges = 
            ((rank_bb(RANK_1) | rank_bb(RANK_8)) & ~rank_bb(rank(s))) | 
            ((file_bb(FILE_A) | file_bb(FILE_H)) & ~file_bb(file(s)));
        auto mask = attacks_sliding(BISHOP, s, 0) & ~edges;
        Bitboard occ = 0;
        do {
            EXPECT_EQ(att[s][occ], attacks_sliding(BISHOP, s, occ));
        } while ((occ = (occ - mask) & mask));
    }
}

TEST(Bitboard, RookMagicAttacks)
{
    auto att = attacks_magic<ROOK>(magic_table);
    for (auto s : Squares()) {
        auto edges = 
            ((rank_bb(RANK_1) | rank_bb(RANK_8)) & ~rank_bb(rank(s))) | 
            ((file_bb(FILE_A) | file_bb(FILE_H)) & ~file_bb(file(s)));
        auto mask = attacks_sliding(ROOK, s, 0) & ~edges;
        Bitboard occ = 0;
        do {
            EXPECT_EQ(att[s][occ], attacks_sliding(ROOK, s, occ));
        } while ((occ = (occ - mask) & mask));
    }
}