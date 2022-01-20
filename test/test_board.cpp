#include <gtest/gtest.h>
#include "moon_board.h"

using namespace moon;

TEST(Board, MoveList)
{
    // TODO
}

TEST(Board, SetNullptrFEN)
{
    Board b;

    EXPECT_FALSE(b.init(nullptr));
}

TEST(Board, SetEmptyFEN)
{
    Board b;

    EXPECT_TRUE(b.init(fen::EMPTY));
    EXPECT_EQ(b.p_whites(), 0);
    EXPECT_EQ(b.p_blacks(), 0);
    EXPECT_EQ(b.p_all(), 0);
    EXPECT_EQ(b.p_pawns(), 0);
    EXPECT_EQ(b.p_knights(), 0);
    EXPECT_EQ(b.p_bishops(), 0);
    EXPECT_EQ(b.p_rooks(), 0);
    EXPECT_EQ(b.p_queens(), 0);
    EXPECT_EQ(b.p_kings(), 0);
    EXPECT_EQ(b.sq_kingw(), NO_SQ);
    EXPECT_EQ(b.sq_kingb(), NO_SQ);
    EXPECT_EQ(b.sq_enps(), NO_SQ);
    EXPECT_EQ(b.ca_rights(), NO_CA);
    EXPECT_EQ(b.turn(), WHITE);
    EXPECT_EQ(b.ply50(), 0);
    EXPECT_EQ(b.ply(), 0);
}

TEST(Board, SetStartFEN)
{
    Board b;

    EXPECT_TRUE(b.init(fen::START));
    EXPECT_EQ(b.p_whites(), 0x00000000'0000ffff);
    EXPECT_EQ(b.p_blacks(), 0xffff0000'00000000);
    EXPECT_EQ(b.p_all(), 0xffff0000'0000ffff);
    EXPECT_EQ(b.p_pawns(), 0x00ff0000'0000ff00);
    EXPECT_EQ(b.p_knights(), bit(B1, G1, B8, G8));
    EXPECT_EQ(b.p_bishops(), bit(C1, F1, C8, F8));
    EXPECT_EQ(b.p_rooks(), bit(A1, H1, A8, H8));
    EXPECT_EQ(b.p_queens(), bit(D1, D8));
    EXPECT_EQ(b.p_kings(), bit(E1, E8));
    EXPECT_EQ(b.sq_kingw(), E1);
    EXPECT_EQ(b.sq_kingb(), E8);
    EXPECT_EQ(b.sq_enps(), NO_SQ);
    EXPECT_EQ(b.ca_rights(), ANY_CA);
    EXPECT_EQ(b.turn(), WHITE);
    EXPECT_EQ(b.ply50(), 0);
    EXPECT_EQ(b.ply(), 0);
}

TEST(Board, SetMalformedFEN)
{
    Board b;

    EXPECT_FALSE(b.init("not/a/fen/666 666  test"));
}

TEST(Board, SetTrickyFEN)
{
    Board b;

    EXPECT_TRUE(b.init(fen::TRICKY));
    EXPECT_EQ(b.p_whites(), bit(D5, E5, E4, C3, F3, A2, B2, C2, D2, E2, F2, G2, H2, A1, E1, H1));
    EXPECT_EQ(b.p_blacks(), bit(A8, E8, H8, A7, C7, D7, E7, F7, G7, A6, B6, E6, F6, G6, B4, H3));
    EXPECT_EQ(b.p_all(), bit(
        D5, E5, E4, C3, F3, A2, B2, C2, D2, E2, F2, G2, H2, A1, E1, H1, 
        A8, E8, H8, A7, C7, D7, E7, F7, G7, A6, B6, E6, F6, G6, B4, H3));
    EXPECT_EQ(b.p_pawns(), bit(A7, C7, D7, F7, E6, G6, D5, B4, E4, H3, A2, B2, C2, F2, G2, H2));
    EXPECT_EQ(b.p_knights(), bit(B6, F6, E5, C3));
    EXPECT_EQ(b.p_bishops(), bit(G7, A6, D2, E2));
    EXPECT_EQ(b.p_rooks(), bit(A8, H8, A1, H1));
    EXPECT_EQ(b.p_queens(), bit(E7, F3));
    EXPECT_EQ(b.p_kings(), bit(E1, E8));
    EXPECT_EQ(b.sq_kingw(), E1);
    EXPECT_EQ(b.sq_kingb(), E8);
    EXPECT_EQ(b.sq_enps(), NO_SQ);
    EXPECT_EQ(b.ca_rights(), ANY_CA);
    EXPECT_EQ(b.turn(), WHITE);
    EXPECT_EQ(b.ply50(), 0);
    EXPECT_EQ(b.ply(), 0);
}

TEST(Board, GetFEN)
{
    // TODO
}