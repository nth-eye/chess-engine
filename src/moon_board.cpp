#include "moon_board.h"

namespace moon {

namespace {

// Bitboard MAGIC_TABLE[87988];

constexpr auto P_ATTACKS    = std::array{ attacks(PAWN), attacks(PAWN, BLACK) };
constexpr auto N_ATTACKS    = attacks(KNIGHT);
constexpr auto K_ATTACKS    = attacks(KING);

}



}