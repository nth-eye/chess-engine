#ifndef MOON_TYPE_H
#define MOON_TYPE_H

#include <cstdint>
#include <cstddef>

namespace moon {

using Bitboard = uint64_t;
using Move = uint16_t;

enum File   { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H };
enum Rank   { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8 };
enum Piece  { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };
enum Side   { WHITE, BLACK };
enum Square {  
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
    NO_SQ,
};
enum Direction {
    NORTH       = 8,
    EAST        = 1,
    SOUTH       = -NORTH,
    WEST        = -EAST,
    NORTH_EAST  = NORTH + EAST,
    SOUTH_EAST  = SOUTH + EAST,
    SOUTH_WEST  = SOUTH + WEST,
    NORTH_WEST  = NORTH + WEST,
};
enum Castle { 
    NO_CA, 
    WKCA        = 0b0001,
    WQCA        = 0b0010, 
    BKCA        = 0b0100, 
    BQCA        = 0b1000,
    KCA         = WKCA | BKCA,
    QCA         = WQCA | BQCA,
    WCA         = WKCA | WQCA,
    BCA         = BKCA | BQCA,
    ANY_CA      = WCA  | BCA,
};
enum Flag { 
    QUIET   = 0b0000'0000'0000'0000, 
    PUSH    = 0b0001'0000'0000'0000,
    K_CAST  = 0b0010'0000'0000'0000,
    Q_CAST  = 0b0011'0000'0000'0000,
    N_PROM  = 0b0100'0000'0000'0000,
    B_PROM  = 0b0101'0000'0000'0000,
    R_PROM  = 0b0110'0000'0000'0000,
    Q_PROM  = 0b0111'0000'0000'0000,
    ENPASS  = 0b1000'0000'0000'0000,
};

}

#endif