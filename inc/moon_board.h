#ifndef MOON_BOARD_H
#define MOON_BOARD_H

#include "moon_bitboard.h"

namespace moon {

struct Moves {
    const Move* begin() const                       { return &list[0]; }
    const Move* end() const                         { return &list[len]; }
    Move* begin()                                   { return &list[0]; }
    Move* end()                                     { return &list[len]; }
    size_t size() const                             { return len; }
    void erase(size_t idx)                          { list[idx] = list[--len]; }
    void save(Move m)                               { list[len++] = m; }
    void save(Square src, Square dst)               { save(mv(src, dst)); }
    void save(Square src, Square dst, Flag flag)    { save(mv(src, dst, flag)); }
private:
    Move list[412];
    size_t len = 0;
};

struct Board {

    Board() = default;
    Board(const Board &b) = default;
    // Board(const Board &b, Move m) { *this = b; make(m); }

    bool init(const char *fen);
    // void make(Move move);

    Bitboard all() const        { return pieces[WHITE] | pieces[BLACK]; }
    Bitboard kings() const      { return bit(k_sq[WHITE]) | bit(k_sq[BLACK]); }

    Bitboard pieces[2]          = {};
    Bitboard pawns              = 0;
    Bitboard knights            = 0;
    Bitboard bishops            = 0;
    Bitboard rooks              = 0;
    Square   k_sq[2]            = {};
    Square   enps_sq            = A1;
    Castle   castle             = NO_CA;
    Side     side               = WHITE;
    uint8_t  half_clk           = 0;
    uint16_t full_clk           = 0;
};

}

#endif