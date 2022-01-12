#include <gtest/gtest.h>
#include "moon_util.h"

using namespace moon;

TEST(Util, Bit)
{
    EXPECT_EQ(bit(0), 0x1);
    EXPECT_EQ(bit(1), 0x2);
    EXPECT_EQ(bit(7), 0x80);
    EXPECT_EQ(bit(63), 0x80000000'00000000);
}

TEST(Util, BitGet)
{
    Bitboard bb = 0x80000000'00000201;

    EXPECT_EQ(get(bb, A1), true);
    EXPECT_EQ(get(bb, B2), true);
    EXPECT_EQ(get(bb, H8), true);
    EXPECT_EQ(get(bb, A2), false);
    EXPECT_EQ(get(bb, B3), false);
    EXPECT_EQ(get(bb, G8), false);
}

TEST(Util, BitSet)
{
    Bitboard bb = 0;

    set(bb, A1);
    set(bb, C3);
    set(bb, E5);
    set(bb, G7);

    EXPECT_EQ(bb, 0x00400010'00040001);
}

TEST(Util, BitClear)
{
    Bitboard bb = 0x00400010'00040001;

    clr(bb, A1);
    clr(bb, G7);

    EXPECT_EQ(bb, 0x00000010'00040000);
}

TEST(Util, BitCount)
{
    // TODO
}

TEST(Util, BitLeastSignificant)
{
    // TODO
}

TEST(Util, File)
{
    EXPECT_EQ(file(A1), FILE_A);
    EXPECT_EQ(file(B2), FILE_B);
    EXPECT_EQ(file(C3), FILE_C);
    EXPECT_EQ(file(D4), FILE_D);
    EXPECT_EQ(file(E5), FILE_E);
    EXPECT_EQ(file(F6), FILE_F);
    EXPECT_EQ(file(G7), FILE_G);
    EXPECT_EQ(file(H8), FILE_H);
}

TEST(Util, Rank)
{
    EXPECT_EQ(rank(A1), RANK_1);
    EXPECT_EQ(rank(B2), RANK_2);
    EXPECT_EQ(rank(C3), RANK_3);
    EXPECT_EQ(rank(D4), RANK_4);
    EXPECT_EQ(rank(E5), RANK_5);
    EXPECT_EQ(rank(F6), RANK_6);
    EXPECT_EQ(rank(G7), RANK_7);
    EXPECT_EQ(rank(H8), RANK_8);
}

TEST(Util, Square)
{
    EXPECT_EQ(A1, 0);
    EXPECT_EQ(H1, 7);
    EXPECT_EQ(A8, 56);
    EXPECT_EQ(H8, 63);
    EXPECT_EQ(sq(RANK_1, FILE_A), A1);
    EXPECT_EQ(sq(RANK_1, FILE_H), H1);
    EXPECT_EQ(sq(RANK_8, FILE_A), A8);
    EXPECT_EQ(sq(RANK_8, FILE_H), H8);
}
