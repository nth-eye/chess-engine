#ifndef BOARD_H
#define BOARD_H

#include "bitboard.h"
#include "move_list.h"

constexpr auto FEN_EMPTY    = "8/8/8/8/8/8/8/8 w - - 0 1";
constexpr auto FEN_START    = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
constexpr auto FEN_TEST_1   = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
constexpr auto FEN_TEST_2   = "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1";
constexpr auto FEN_TEST_3   = "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9";

struct Board {

    constexpr Board() = default;
    constexpr Board(const Board &board, Move move) { *this = board; make_move(move); }

    void print() const;
    void clr_pos();
    bool set_pos(const char *fen);
    bool legal(Move move) const;
    bool attacked(Square s, Color att_clr) const;
    void moves(MoveList &list) const;
    void moves_pseudo(MoveList &list) const;
    void moves_legal(MoveList &list) const;
    void make_move(Move move);

    Bitboard danger() const;

    Bitboard all() const        { return pieces[WHITE] | pieces[BLACK]; }
    Bitboard knights() const    { return all() & ~(rooks | bishops | pawns | kings()); }
    Bitboard queens() const     { return rooks & bishops; }
    Bitboard kings() const      { return bit(k_sq[WHITE]) | bit(k_sq[BLACK]); }
private:
    Bitboard pieces[COLOR_num]  = {};
    Bitboard pawns              = 0;
    Bitboard bishops            = 0;
    Bitboard rooks              = 0;
    Square   k_sq[COLOR_num]    = {};
    Square   enps_sq            = A1;
    Castle   castle             = CASTLE_NO;
    Color    side               = WHITE;
    uint8_t  half_clk           = 0;
    uint16_t full_clk           = 0;
};

#endif // BOARD_H