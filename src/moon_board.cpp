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



}