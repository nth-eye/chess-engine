#include "board.h"
#include "log.h"
#include <cstring>
#include <cstdlib>
#include <algorithm>

constexpr Square CASTLE_SQ[][2] = { { H1, A1 }, { H8, A8 } };
constexpr MoveFlag PROMOTIONS[] = { N_PROM, B_PROM, R_PROM, Q_PROM };
constexpr auto P_ATTACKS    = attacks_pawn();
constexpr auto N_ATTACKS    = attacks<KNIGHT>();
constexpr auto K_ATTACKS    = attacks<KING  >();
const auto B_ATTACKS        = attacks_magic<BISHOP>(B_MAGIC_NUM);
const auto R_ATTACKS        = attacks_magic<ROOK  >(R_MAGIC_NUM);

constexpr int CASTLE_PERM_FROM[64] = {
    0xD, 0xF, 0xF, 0xF, 0xC, 0xF, 0xF, 0xE,
    0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,
    0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,
    0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,
    0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,
    0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,
    0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,
    0x7, 0xF, 0xF, 0xF, 0x3, 0xF, 0xF, 0xB  
};

constexpr int CASTLE_PERM_TO[64] = {
    0xD, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xE,
    0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,
    0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,
    0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,
    0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,
    0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,
    0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF,
    0x7, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xB 
};

static bool str_to_int(const char *str, long *val, int base, char **end)
{
    long res = strtol(str, end, base);

    if (*end > str) {
        *val = res;
        return true;
    }
    return false;
}

void Board::print() const
{
    LOGC('\n');
    for (Rank r = RANK_8; r >= RANK_1; --r) {
        LOG("%c   ", rank_c(r));
        for (File f = FILE_A; f <= FILE_H; ++f) {

            Square s    = sq(r, f);
            char c      = ' ';

            if (!get(all(), s))
                c = '.';
            else if (get(kings(), s))
                c = 'k';
            else if (get(pawns, s))
                c = 'p';
            else if (get(bishops, s))
                c = get(rooks, s) ? 'q' : 'b';
            else if (get(rooks, s))
                c = 'r';
            else
                c = 'n';

            if (get(pieces[WHITE], s)) 
                c ^= bit(5);

            LOG("%c ", c);
        }
        LOGC('\n');
    }
    LOG("\n   ");
    for (File f = FILE_A; f <= FILE_H; ++f)
        LOG(" %c", file_c(f));
    LOG("\n\n");
    LOG("cast:  %c%c%c%c    \n", 
        castle & WKCA ? 'K' : '-', castle & WQCA ? 'Q' : '-', 
        castle & BKCA ? 'k' : '-', castle & BQCA ? 'q' : '-');
    LOG("enps:  ");
    if (enps_sq)
        print_sq(enps_sq);
    else
        LOG("- \n");
    LOG("half:  %u          \n", half_clk);
    LOG("full:  %u          \n", full_clk);
    LOG("side:  %c          \n", side_c(side));
    LOG("size:  %lu bytes   \n", sizeof(Board));
    LOGC('\n');
}

void Board::clr_pos()
{
    memset(reinterpret_cast<void*>(this), 0, sizeof(Board));
}

bool Board::set_pos(const char *c)
{
    LOG("%s: %s \n", __func__, c);

    clr_pos();

    Square s = A8;

    while (*c != ' ') {

        if (*c >= '1' && *c <= '8') {
            s += Direction(EAST  * (*c - '0'));
        } else if (*c == '/') {
            s += Direction(SOUTH * 2);
        } else {
            switch (*c) {
                case 'p':
                case 'P': set(pawns, s);    break;
                case 'n':
                case 'N':                   break;
                case 'b':
                case 'B': set(bishops, s);  break;
                case 'q':
                case 'Q': set(bishops, s);
                case 'r':
                case 'R': set(rooks, s);    break;
                case 'k': k_sq[BLACK] = s;  break;
                case 'K': k_sq[WHITE] = s;  break;
                default: return false;
            }
            if (*c & bit(5))
                set(pieces[BLACK], s);
            else
                set(pieces[WHITE], s);
            ++s;
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

    constexpr const char *castle_str = "KQkq";

    if (*c != '-') {
        for (int i = 0; i < 4; ++i, ++c) {
            if (*c != castle_str[i] &&
                *c != '-') 
            {
                return false;
            }
            if (*c != '-')
                castle |= 1 << i;
        }
    } else {
        ++c;
    }
    if (*c++ != ' ')
        return false;

    if (*c != '-') {

        File f = File(c[0] - 'a');
        Rank r = Rank(c[1] - '1');

        if (f < FILE_A || 
            f > FILE_H || 
            (side == WHITE && r != RANK_6) || 
            (side == BLACK && r != RANK_3))
        {
            return false;
        }
        enps_sq = sq(r, f);
        ++c;
    }
    ++c;

    if (*c++ != ' ')
        return false;

    long val;
    char *end = NULL;

    if (!str_to_int(c, &val, 10, &end) || val >= 100)
        return false; 
    half_clk = val;

    c = end;

    if (*c++ != ' ')
        return false;

    if (!str_to_int(c, &val, 10, &end) || val < 1)
        return false; 
    full_clk = (val - 1) * 2 + side;

    LOG("%s: remainder - [%s] \n", __func__, end);

    return true;
}

bool Board::attacked(Square s, Color att_clr) const 
{
    Bitboard they = pieces[att_clr];
    Bitboard king = bit(k_sq[att_clr]);

    // Bishops and queens
    if (B_ATTACKS[s][all()]     & they & bishops)
        return true;
    // Rooks and queens
    if (R_ATTACKS[s][all()]     & they & rooks)
        return true;
    // Pawns
    if (P_ATTACKS[~att_clr][s]  & they & pawns)
        return true;
    // Knights
    if (N_ATTACKS[s]            & they & knights())
        return true;
    // King
    if (K_ATTACKS[s]            & king)
        return true;

    return false;
}

MoveList Board::moves_pseudo(/*Move *list, size_t max*/) const
{
    Bitboard attacker = pieces[side];
    Bitboard both     = all();
    Square king       = k_sq[side];

    MoveList list;
    list.reserve(100);

    auto save = [&] (Square src, Square dst, MoveFlag flag) 
    {
        list.push_back(mv(src, dst, flag));
    };
    auto path_free = [&] (Square src, Square dst) 
    {
        while (src != dst) {
            if (get(both, src))
                return false;
            ++src;
        }
        return true;
    };
    auto path_attacked = [this] (Square src, Square dst) 
    {
        while (src != dst) {
            if (attacked(src, ~side))
                return true;
            ++src;
        }
        return false;
    };
    // Pawn
    for (auto src : BitIter(attacker & pawns)) {

        Direction dir   = side == WHITE ? NORTH : SOUTH;
        Square dst      = src + dir;
        Rank prom       = Rank((7 + side) & 7);
        Rank push       = Rank(2 + side * 3);
        
        if (!get(both, dst)) {

            if (rank(dst) != prom) {
                // Quiet
                save(src, dst, QUIET);
                // Double push
                if (rank(dst) == push && !get(both, dst + dir))
                    save(src, dst + dir, PUSH);
            } else {
                // Promotions
                for (auto type : PROMOTIONS)
                    save(src, dst, type);
            }
        }
        // Captures
        Bitboard attacked = P_ATTACKS[side][src] & pieces[~side];

        for (auto dst : BitIter(attacked)) {
            if (rank(dst) == prom) {
                // Promotions with capture
                for (auto type : PROMOTIONS)
                    save(src, dst, type);
            } else {
                save(src, dst, QUIET);
            }
        }
        // enPassant
        if (bit(enps_sq) & P_ATTACKS[side][src])
            save(src, enps_sq, ENPASS);
    }
    // Knight
    for (auto src : BitIter(attacker & knights())) {

        Bitboard attacked = N_ATTACKS[src]       & ~attacker;

        for (auto dst : BitIter(attacked))
            save(src, dst, QUIET);
    }
    // Bishop / queen
    for (auto src : BitIter(attacker & bishops)) {

        Bitboard attacked = B_ATTACKS[src][both] & ~attacker;

        for (auto dst : BitIter(attacked))
            save(src, dst, QUIET);
    }
    // Rook / queen
    for (auto src : BitIter(attacker & rooks)) {

        Bitboard attacked = R_ATTACKS[src][both] & ~attacker;

        for (auto dst : BitIter(attacked))
            save(src, dst, QUIET);
    }
    // King
    Bitboard attacked = K_ATTACKS[king] & ~attacker;

    for (const auto dst : BitIter(attacked)) 
        save(king, dst, QUIET);

    auto ca_rights = castle >> (2 * side); 

    if (ca_rights & WKCA) {
        auto rook = CASTLE_SQ[side][0];
        if (path_free(king + EAST, rook) && !path_attacked(king, king + Direction(EAST * 2)))
            save(king, king + Direction(EAST * 2), K_CAST);
    }
    if (ca_rights & WQCA) {
        auto rook = CASTLE_SQ[side][1];
        if (path_free(rook + EAST, king) && !path_attacked(king + WEST, king + EAST))
            save(king, king + Direction(WEST * 2), Q_CAST);
    }
    return list;
}

MoveList Board::moves(/*Move *list, size_t max*/) const
{
    MoveList list = moves_pseudo();
    list.erase(
        std::remove_if(	
            list.begin(), 
            list.end(), 
            [&](Move m) { return !legal(m); }), 
        list.end());
    return list;
}

bool Board::legal(Move move) const 
{
    Board board = *this;
    board.make_move(move);
    return !board.attacked(board.k_sq[side], ~side);
}

void Board::make_move(Move move) 
{
    const auto src  = from(move);
    const auto dst  = to(move);
    const auto f    = flag(move);

    // Remove en passant square, update castling rules and clocks
    ++half_clk;
    ++full_clk;
    enps_sq     = A1;
    castle      &= CASTLE_PERM_FROM[src];
    castle      &= CASTLE_PERM_TO[dst];

    auto do_castle = [this] (Square rook_src, Square rook_dst) 
    {
        clr(pieces[side],   rook_src);
        clr(rooks,          rook_src);
        set(pieces[side],   rook_dst);
        set(rooks,          rook_dst);
    };
    // Special moves already discovered during generation.
    switch (f) {
        default:
            break;
        case K_CAST:
            side == WHITE ? do_castle(H1, F1) : do_castle(H8, F8);
            break;
        case Q_CAST:
            side == WHITE ? do_castle(A1, D1) : do_castle(A8, D8);
            break;
        case PUSH:
            enps_sq = side == WHITE ? dst + SOUTH : dst + NORTH; 
            break;
        case ENPASS:
            auto trg = side == WHITE ? dst + SOUTH : dst + NORTH;
            clr(pawns,          trg);
            clr(pieces[~side],  trg);
            break;
    }
    // Remove captured piece.
    if (get(pieces[~side], dst)) {
        clr(pieces[~side], dst);
        clr(pawns,      dst);
        clr(rooks,      dst);
        clr(bishops,    dst);
        half_clk = 0;
    }
    // Move in our pieces.
    set(pieces[side], dst);
    clr(pieces[side], src);

    side = ~side;

    // If king moves (~side because we just changed it)
    if (k_sq[~side] == src) {
        k_sq[~side] = dst;
        return;
    }
    // Reset 50 move counter if pawn was moved
    if (get(pawns, src))
        half_clk = 0;

    // Promotion
    if (f & N_PROM) {
        switch (f) {
            case B_PROM: set(bishops, src); break;
            case Q_PROM: set(bishops, src);
            case R_PROM: set(rooks,   src); break;
            default:;
        }
        clr(pawns, src);
        return;
    }
    // Update Bitboards during regular move
    pawns   |= static_cast<Bitboard>(get(pawns,     src)) << dst;
    rooks   |= static_cast<Bitboard>(get(rooks,     src)) << dst;
    bishops |= static_cast<Bitboard>(get(bishops,   src)) << dst;

    // Remove from other Bitboards
    clr(pawns,      src);
    clr(rooks,      src);
    clr(bishops,    src);
}