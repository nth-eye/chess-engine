#include <cstring>
#include <cstdlib>
#include "moon_board.h"

namespace moon {
namespace {

Bitboard magic_table[88772];

constexpr auto line         = lines();
constexpr auto between      = betweens();
constexpr auto p_attacks    = std::array{ attacks(PAWN), attacks(PAWN, BLACK) };
constexpr auto n_attacks    = attacks(KNIGHT);
constexpr auto k_attacks    = attacks(KING);
const auto b_attacks        = attacks_magic<BISHOP>(magic_table);
const auto r_attacks        = attacks_magic<ROOK>(magic_table);

Bitboard pawns_attacks(Bitboard pawns, Color side)
{
    // Bitboard bb = 0;
    // for (auto src : BitIter(pawns))
    //     bb |= p_attacks[side][src];
    // return bb;
    return side == WHITE ? 
        shift(pawns, SOUTH_EAST, SOUTH_WEST):
        shift(pawns, NORTH_EAST, NORTH_WEST);
}

Bitboard knights_attacks(Bitboard knights)
{
    // Bitboard bb = 0;
    // for (auto src : BitIter(knights))
    //     bb |= n_attacks[src];
    // return bb;
    Bitboard l1 = (knights >> 1) & 0x7f7f7f7f7f7f7f7f;
    Bitboard l2 = (knights >> 2) & 0x3f3f3f3f3f3f3f3f;
    Bitboard r1 = (knights << 1) & 0xfefefefefefefefe;
    Bitboard r2 = (knights << 2) & 0xfcfcfcfcfcfcfcfc;
    Bitboard h1 = l1 | r1;
    Bitboard h2 = l2 | r2;
    return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
}

Bitboard bishops_attacks(Bitboard bishops, Bitboard occ)
{
    Bitboard bb = 0;
    for (auto src : BitIter(bishops))
        bb |= b_attacks[src][occ]; 
    return bb;
}

Bitboard rooks_attacks(Bitboard rooks, Bitboard occ)
{
    Bitboard bb = 0;
    for (auto src : BitIter(rooks))
        bb |= r_attacks[src][occ]; 
    return bb;
}

Bitboard king_attacks(Square sq)
{
    return k_attacks[sq];
}

void add_moves(Square src, Bitboard all_dst, Moves &list)
{
    for (auto dst : BitIter(all_dst))
        list.save(src, dst);
}

void pawns_quiet_moves(Bitboard pawns, Bitboard legal, Color side, Moves &list)
{
    auto dir        = side == WHITE ? NORTH : SOUTH;
    auto rank       = side == WHITE ? rank_bb(RANK_4) : rank_bb(RANK_5);
    auto pushed     = shift(pawns, dir) & legal;
    auto pushed_dbl = shift(pushed, dir) & legal & rank;

    for (auto dst : BitIter(pushed))
        list.save(dst - dir, dst);

    for (auto dst : BitIter(pushed_dbl))
        list.save(dst - dir - dir, dst);
}

}

bool Board::set_fen(const char *c)
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

bool Board::get_fen(char *fen)
{
    // TODO
    return false;
}

bool Board::attacked(Color c, Square s) const
{
    Bitboard they = pieces[c];
    Bitboard king = bit(k_sq[c]);
    Bitboard both = p_all();

    return  (p_attacks[~c][s]   & they & pawns)     |
            (b_attacks[s][both] & they & bishops)   |
            (r_attacks[s][both] & they & rooks)     |
            (n_attacks[s]       & they & knights)   |
            (k_attacks[s]       & king);
} 

void Board::moves_pawn_quiet(Moves &list) const
{
    auto dir        = side == WHITE ? NORTH : SOUTH;
    auto rank       = side == WHITE ? rank_bb(RANK_4) : rank_bb(RANK_5);
    auto empty      = ~p_all();
    auto pushed     = shift(pawns, dir) & empty;
    auto pushed_dbl = shift(pushed, dir) & empty & rank;

    for (auto dst : BitIter(pushed))
        list.save(dst - dir, dst);

    for (auto dst : BitIter(pushed_dbl))
        list.save(dst - dir - dir, dst);
}

void Board::moves_pawn_captures(Moves &list)
{
    // auto them_p = pawns & pieces[~side];
    // auto ret    = shift(them_p, 
    //     side == WHITE ? 
    //     NORTH_EAST, NORTH_WEST :
    //     SOUTH_EAST, SOUTH_WEST);
}

void Board::moves_all(Moves &list)
{
    Square king         = k_sq[side];
    Bitboard us         = pieces[side]; 
    Bitboard them       = pieces[~side];
    Bitboard both       = p_all();
    Bitboard empty      = ~p_all();
    Bitboard danger     = restricted();
    Bitboard king_dst   = king_attacks(king) & ~us & ~danger;

    add_moves(king, king_dst, list);

    Bitboard legal_mask;
    Bitboard pins;
    Bitboard checker = checkers();

    switch (cnt(checker)) {

        default: return;

        case 1:
            pins = pinned();
            legal_mask = checker;

            if (checker & (bishops | rooks))
                legal_mask |= between[king][lsb(checker)];
        break;

        case 0:
            pins = pinned();
            legal_mask = ~us;

            pawns_quiet_moves(pins & pawns, empty & file_bb(file(king)), side, list);
            // pawns_capture_moves();
            // pawns_promotion_moves();

            for (auto src : BitIter(pins & bishops))
                add_moves(src, b_attacks[src][both] & legal_mask & line[king][src], list);
            
            for (auto src : BitIter(pins & rooks))
                add_moves(src, r_attacks[src][both] & legal_mask & line[king][src], list);
        break;
    }
    Bitboard not_pins = us & ~pins;

    pawns_quiet_moves(not_pins & pawns, empty, side, list);
    // pawns_capture_moves();
    // pawns_promotion_moves();

    for (auto src : BitIter(not_pins & knights))
        add_moves(src, n_attacks[src]       & legal_mask, list);

    for (auto src : BitIter(not_pins & bishops))
        add_moves(src, b_attacks[src][both] & legal_mask, list);

    for (auto src : BitIter(not_pins & rooks))
        add_moves(src, r_attacks[src][both] & legal_mask, list);
}

// void Board::make(Move move) 
// {

// }

Bitboard Board::restricted() const
{
    Bitboard bb         = 0;
    Bitboard them       = pieces[~side];
    Bitboard both       = p_all();
    Bitboard us_king    = bit(k_sq[side]);

    bb |= king_attacks(k_sq[~side]);
    bb |= pawns_attacks(them & pawns, side);
    bb |= rooks_attacks(them & rooks, both ^ us_king);
    bb |= knights_attacks(them & knights);
    bb |= bishops_attacks(them & bishops, both ^ us_king);

    return bb;
}

Bitboard Board::pinned() const
{
    Square king     = k_sq[side];
    Bitboard us     = pieces[side]; 
    Bitboard them   = pieces[~side];
    Bitboard both   = p_all();
    Bitboard them_r = them & rooks;
    Bitboard them_b = them & bishops;

    Bitboard pinned         = 0;
    Bitboard pinned_maybe   = r_attacks[king][both] & us;
    Bitboard pinner         = r_attacks[king][both ^ pinned_maybe] & them_r;
    for (auto s : BitIter(pinner))
        pinned  |= r_attacks[s][both] & pinned_maybe;
    pinned_maybe    = b_attacks[king][both] & us;
    pinner          = b_attacks[king][both ^ pinned_maybe] & them_b;
    for (auto s : BitIter(pinner))
        pinned |= b_attacks[s][both] & pinned_maybe;

    return pinned;
}

Bitboard Board::checkers() const 
{
    Square king     = k_sq[side];
    Bitboard them   = pieces[~side];
    Bitboard both   = p_all();

    return 
        (n_attacks[king]        & them & knights)   |
        (p_attacks[side][king]  & them & pawns)     |
        (b_attacks[king][both]  & them & bishops)   |
        (r_attacks[king][both]  & them & rooks);
}

}