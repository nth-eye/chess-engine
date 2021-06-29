#ifndef BOARD_H
#define BOARD_H

#include "bitboard.h"
#include "move_list.h"

constexpr auto FEN_EMPTY    = "8/8/8/8/8/8/8/8 w - - 0 1";
constexpr auto FEN_START    = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

struct Board {

    constexpr Board() = default;
    constexpr Board(const Board &board, Move move) { *this = board; make_move(move); }

    template<Color Side>    
    Bitboard restricted() const;
    template<Color Side>    
    bool attacked(Square s) const;
    template<Color Side>    
    bool legal(Move move) const;
    template<Color Side>    
    void moves_all(MoveList &list) const;
    template<Color Side, bool Pinned> 
    void moves_pawn(Bitboard selected_pawns, Bitboard capture_mask, Bitboard quiet_mask, MoveList &list) const;
    void moves(MoveList &list) const;

    void print() const;
    void clr_pos();
    bool set_pos(const char *fen);
    void make_move(Move move);

    Bitboard all() const        { return pieces[WHITE] | pieces[BLACK]; }
    Bitboard knights() const    { return all() & ~(rooks | bishops | pawns | kings()); }
    Bitboard queens() const     { return rooks & bishops; }
    Bitboard kings() const      { return bit(k_sq[WHITE]) | bit(k_sq[BLACK]); }

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