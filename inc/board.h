#ifndef BOARD_H
#define BOARD_H

#include "bitboard.h"

constexpr auto FEN_EMPTY    = "8/8/8/8/8/8/8/8 w - - ";
constexpr auto FEN_START    = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
constexpr auto FEN_TEST_1   = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ";
constexpr auto FEN_TEST_2   = "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1";
constexpr auto FEN_TEST_3   = "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 ";

struct Board {

    Board() = default;

    void print();

    Bitboard all() const        { return pieces[WHITE] | pieces[BLACK]; }
    Bitboard pawns() const      { return pawns_en & SQ_ENPS; }
    Bitboard knights() const    { return all() & ~rooks & ~bishops & ~pawns_en & ~kings; }
    Bitboard queens() const     { return rooks & bishops; }
private:
    Bitboard pieces[COLOR_num];
    Bitboard pawns_en;
    Bitboard bishops;
    Bitboard rooks;
    Bitboard kings;
    Castle   castle;
    Color    side;
};

#endif // BOARD_H