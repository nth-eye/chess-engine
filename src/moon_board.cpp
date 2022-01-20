#include <cstring>
#include <cstdlib>
#include "moon_board.h"

namespace moon {
namespace {

Bitboard magic_table[88772];

constexpr auto p_attacks    = std::array{ attacks(PAWN), attacks(PAWN, BLACK) };
constexpr auto n_attacks    = attacks(KNIGHT);
constexpr auto k_attacks    = attacks(KING);
const auto b_attacks        = attacks_magic<BISHOP>(magic_table);
const auto r_attacks        = attacks_magic<ROOK>(magic_table);

}

bool Board::init(const char *c)
{
    if (!c)
        return false;
    *this = {};

    Square s = A8;

    while (*c != ' ') {

        if (*c >= '1' && *c <= '8') {
            s = Square(s + EAST  * (*c - '0'));
        } else if (*c == '/') {
            s = Square(s + SOUTH * 2);
        } else {
            switch (*c) {
                case 'p':                   [[fallthrough]];
                case 'P': set(pawns, s);    break;
                case 'n':                   [[fallthrough]];
                case 'N': set(knights, s);  break;
                case 'b':                   [[fallthrough]];
                case 'B': set(bishops, s);  break;
                case 'q':                   [[fallthrough]];
                case 'Q': set(bishops, s);  [[fallthrough]];
                case 'r':                   [[fallthrough]];
                case 'R': set(rooks, s);    break;
                case 'k': k_sq[BLACK] = s;  break;
                case 'K': k_sq[WHITE] = s;  break;
                default: return false;
            }
            if (*c & bit(5))
                set(pieces[BLACK], s);
            else
                set(pieces[WHITE], s);
            s = Square(s + 1);
        }
        ++c;
        if (s < A1 || s > H8 + 1)
            return false;
    }
    ++c;

    switch (*c++) {
        case 'w': side = WHITE; break;
        case 'b': side = BLACK; break;
        default: return false;
    }
    if (*c++ != ' ')
        return false;

    if (*c == '-') {
        ++c;
    } else {
        int i = 0;
        while (*c != ' ' && i < 4) {
            switch (*c++) {
                case 'K': castle = Castle(castle | WKCA); break;
                case 'Q': castle = Castle(castle | WQCA); break;
                case 'k': castle = Castle(castle | BKCA); break;
                case 'q': castle = Castle(castle | BQCA); break;
                default: return false;
            }
            ++i;
        }
    }
    if (*c++ != ' ')
        return false;

    if (*c != '-') {

        File f = File(*c++ - 'a');
        Rank r = Rank(*c - '1');

        if (f < FILE_A || 
            f > FILE_H || 
            (side == WHITE && r != RANK_6) || 
            (side == BLACK && r != RANK_3))
        {
            return false;
        }
        enps_sq = sq(r, f);
    }
    ++c;
    if (*c++ != ' ')
        return true;

    auto val = strtoul(c, const_cast<char**>(&c), 10); // I fucking hate C std lib for const-incorrectness

    if (val >= 100)
        return true; 
    half_clk = val;

    if (*c++ != ' ')
        return true;

    if ((val = strtoul(c, const_cast<char**>(&c), 10)) < 1) // Another C const-nonsense
        return true; 
    full_clk = (val - 1) * 2 + side;

    return true;
}

// void Board::make(Move move) 
// {

// }

}