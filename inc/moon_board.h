#ifndef MOON_BOARD_H
#define MOON_BOARD_H

#include "moon_bitboard.h"

namespace moon {
namespace fen {
inline constexpr auto EMPTY = "8/8/8/8/8/8/8/8 w - - 0 1";
inline constexpr auto START = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
inline constexpr auto TRICKY = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
inline constexpr auto ENPS_CAPTURE_ILLEGAL_W = "8/5bk1/8/2Pp4/8/1K6/8/8 w - d6 0 1"; // perft 6 = 824064
inline constexpr auto ENPS_CAPTURE_ILLEGAL_B = "8/8/1k6/8/2pP4/8/5BK1/8 b - d3 0 1"; // perft 6 = 824064
inline constexpr auto ENPS_CAPTURE_CHECK_W = "8/5k2/8/2Pp4/2B5/1K6/8/8 w - d6 0 1"; // perft 6 = 1440467
inline constexpr auto ENPS_CAPTURE_CHECK_B = "8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1"; // perft 6 = 1440467
inline constexpr auto SHORT_CASTLE_CHECK_W = "5k2/8/8/8/8/8/8/4K2R w K - 0 1"; // perft 6 = 661072
inline constexpr auto SHORT_CASTLE_CHECK_B = "4k2r/8/8/8/8/8/8/5K2 b k - 0 1"; // perft 6 = 661072
inline constexpr auto LONG_CASTLE_CHECK_W = "3k4/8/8/8/8/8/8/R3K3 w Q - 0 1"; // perft 6 = 803711
inline constexpr auto LONG_CASTLE_CHECK_B = "r3k3/8/8/8/8/8/8/3K4 b q - 0 1"; // perft 6 = 803711
inline constexpr auto LOSING_CASTLE_RIGHT_W = "r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1"; // perft 4 = 1274206
inline constexpr auto LOSING_CASTLE_RIGHT_B = "r3k2r/7b/8/8/8/8/1B4BQ/R3K2R b KQkq - 0 1"; // perft 4 = 1274206
inline constexpr auto CASTLE_PREVENTED_W = "r3k2r/8/5Q2/8/8/3q4/8/R3K2R w KQkq - 0 1"; // perft 4 = 1720476
inline constexpr auto CASTLE_PREVENTED_B = "r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1"; // perft 4 = 1720476
inline constexpr auto PROM_OUT_OF_CHECK_W = "2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1"; // perft 6 = 3821001
inline constexpr auto PROM_OUT_OF_CHECK_B = "3K4/8/8/8/8/8/4p3/2k2R2 b - - 0 1"; // perft 6 = 3821001
inline constexpr auto DISCOVERED_CHECK_W = "5K2/8/1Q6/2N5/8/1p2k3/8/8 w - - 0 1"; // perft 5 = 1004658
inline constexpr auto DISCOVERED_CHECK_B = "8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1"; // perft 5 = 1004658
inline constexpr auto PROM_TO_GIVE_CHECK_W = "4k3/1P6/8/8/8/8/K7/8 w - - 0 1"; // perft 6 = 217342
inline constexpr auto PROM_TO_GIVE_CHECK_B = "8/k7/8/8/8/8/1p6/4K3 b - - 0 1"; // perft 6 = 217342
inline constexpr auto UNDERPROM_TO_CHECK_W = "8/P1k5/K7/8/8/8/8/8 w - - 0 1"; // perft 6 = 92683
inline constexpr auto UNDERPROM_TO_CHECK_B = "8/8/8/8/8/k7/p1K5/8 b - - 0 1"; // perft 6 = 92683
inline constexpr auto SELF_STALEMATE_W = "K1k5/8/P7/8/8/8/8/8 w - - 0 1"; // perft 6 = 2217
inline constexpr auto SELF_STALEMATE_B = "8/8/8/8/8/p7/8/k1K5 b - - 0 1"; // perft 6 = 2217
inline constexpr auto STALE_AND_CHECKMATE_W = "8/k1P5/8/1K6/8/8/8/8 w - - 0 1"; // perft 7 = 567584
inline constexpr auto STALE_AND_CHECKMATE_B = "8/8/8/8/1k6/8/K1p5/8 b - - 0 1"; // perft 7 = 567584
inline constexpr auto DOUBLE_CHECK_W = "8/5k2/8/5N2/5Q2/2K5/8/8 w - - 0 1"; // perft 4 = 23527
inline constexpr auto DOUBLE_CHECK_B = "8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1"; // perft 4 = 23527
}

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
    // Board(const Board &b, Move m) : Board(b) { make(m); }

    bool set_fen(const char *fen);
    bool get_fen(char *fen);

    bool attacked(Color c, Square s) const;
    bool attacked(Color c, Square s, auto... more) const        { return attacked(c, s) && attacked(c, more...); }
    bool not_attacked(Color c, Square s) const                  { return !attacked(c, s); }
    bool not_attacked(Color c, Square s, auto... more) const    { return not_attacked(c, s) && not_attacked(c, more...); }

    void moves_pawn_quiet(Moves &list) const;
    void moves_pawn_captures(Moves &list);
    void moves_all(Moves &list);

    void make(Move move);

    Bitboard restricted() const;
    Bitboard pinned() const;
    Bitboard checkers() const;

    Bitboard p_all() const      { return pieces[WHITE] | pieces[BLACK]; }
    Bitboard p_whites() const   { return pieces[WHITE]; }
    Bitboard p_blacks() const   { return pieces[BLACK]; }
    Bitboard p_pawns() const    { return pawns; }
    Bitboard p_knights() const  { return knights; }
    Bitboard p_bishops() const  { return bishops & ~rooks; }
    Bitboard p_rooks() const    { return rooks & ~bishops; }
    Bitboard p_queens() const   { return rooks & bishops; }
    Bitboard p_kings() const    { return p_all() & (bit(k_sq[WHITE]) | bit(k_sq[BLACK])); }
    Square sq_kingw() const     { return k_sq[WHITE]; }
    Square sq_kingb() const     { return k_sq[BLACK]; }
    Square sq_enps() const      { return enps_sq; }
    Castle ca_rights() const    { return castle; }
    uint8_t ply50() const       { return half_clk; }
    uint16_t ply() const        { return full_clk; }
    Color turn() const          { return side; }
// private:
    Bitboard pieces[2]          = {};
    Bitboard pawns              = 0;
    Bitboard knights            = 0;
    Bitboard bishops            = 0;
    Bitboard rooks              = 0;
    Square   k_sq[2]            = {NO_SQ, NO_SQ};
    Square   enps_sq            = NO_SQ;
    Castle   castle             = NO_CA;
    Color     side               = WHITE;
    uint8_t  half_clk           = 0;
    uint16_t full_clk           = 0;
};

}

#endif