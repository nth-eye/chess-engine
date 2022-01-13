#include <gtest/gtest.h>
#include "moon_bitboard.h"

using namespace moon;

TEST(Bitboard, RankAndFileBB)
{
    EXPECT_EQ(rank_bb(RANK_1), 0x00000000'000000ff);
    EXPECT_EQ(rank_bb(RANK_2), 0x00000000'0000ff00);
    EXPECT_EQ(rank_bb(RANK_7), 0x00ff0000'00000000);
    EXPECT_EQ(rank_bb(RANK_8), 0xff000000'00000000);

    EXPECT_EQ(file_bb(FILE_A), 0x01010101'01010101);
    EXPECT_EQ(file_bb(FILE_B), 0x02020202'02020202);
    EXPECT_EQ(file_bb(FILE_G), 0x40404040'40404040);
    EXPECT_EQ(file_bb(FILE_H), 0x80808080'80808080);
}

TEST(Bitboard, SameLine)
{
    EXPECT_EQ(same_line(A1, A1), true);
    EXPECT_EQ(same_line(A1, A8), true);
    EXPECT_EQ(same_line(G5, B5), true);
    EXPECT_EQ(same_line(H2, H7), true);

    EXPECT_EQ(same_line(A1, B2), false);
    EXPECT_EQ(same_line(A1, C8), false);
    EXPECT_EQ(same_line(G5, F4), false);
    EXPECT_EQ(same_line(H2, A1), false);
}

TEST(Bitboard, SameDiagonal)
{
    EXPECT_EQ(same_diag(A1, B2), true);
    EXPECT_EQ(same_diag(D2, H6), true);
    EXPECT_EQ(same_diag(B7, H1), true);
    EXPECT_EQ(same_diag(C1, A3), true);

    EXPECT_EQ(same_diag(A1, A8), false);
    EXPECT_EQ(same_diag(A1, H1), false);
    EXPECT_EQ(same_diag(G7, G6), false);
    EXPECT_EQ(same_diag(G7, B7), false);
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

TEST(Bitboard, ShiftMany)
{
    auto bb = bit(C3);
    
    EXPECT_EQ(shift(bb, std::array{NORTH, NORTH, EAST}), bit(D5));
    EXPECT_EQ(shift(bb, std::array{NORTH, NORTH, WEST}), bit(B5));
    EXPECT_EQ(shift(bb, std::array{NORTH, EAST, EAST}), bit(E4));
    EXPECT_EQ(shift(bb, std::array{NORTH, WEST, WEST}), bit(A4));
    EXPECT_EQ(shift(bb, std::array{SOUTH, EAST, EAST}), bit(E2));
    EXPECT_EQ(shift(bb, std::array{SOUTH, WEST, WEST}), bit(A2));
    EXPECT_EQ(shift(bb, std::array{SOUTH, SOUTH, EAST}), bit(D1));
    EXPECT_EQ(shift(bb, std::array{SOUTH, SOUTH, WEST}), bit(B1));
}

TEST(Bitboard, ShiftPack)
{
    
}