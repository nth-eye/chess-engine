#include "board.h"
#include "log.h"
#include <cstring>
#include <cstdlib>

constexpr Square CASTLE_SQ[][2] = { { H1, A1 }, { H8, A8 } };
constexpr MoveFlag PROMOTIONS[] = { N_PROM, B_PROM, R_PROM, Q_PROM };
constexpr auto BETWEEN_BB   = squares_between();
constexpr auto P_ATTACKS    = attacks_pawn();
constexpr auto N_ATTACKS    = attacks<KNIGHT>();
constexpr auto K_ATTACKS    = attacks<KING  >();
const auto B_ATTACKS        = attacks_magic<BISHOP>(B_MAGIC_NUM);
const auto R_ATTACKS        = attacks_magic<ROOK  >(R_MAGIC_NUM);

constexpr Castle CASTLE_FROM[64] = {
    BCA | WKCA, ANY_CA, ANY_CA, ANY_CA, BCA,    ANY_CA, ANY_CA, BCA | WQCA,
    ANY_CA,     ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA,
    ANY_CA,     ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA,
    ANY_CA,     ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA,
    ANY_CA,     ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA,
    ANY_CA,     ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA,
    ANY_CA,     ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA,
    WCA | BKCA, ANY_CA, ANY_CA, ANY_CA, WCA,    ANY_CA, ANY_CA, WCA | BQCA,
};

constexpr Castle CASTLE_TO[64] = {
    BCA | WKCA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, BCA | WQCA,
    ANY_CA,     ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA,
    ANY_CA,     ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA,
    ANY_CA,     ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA,
    ANY_CA,     ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA,
    ANY_CA,     ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA,
    ANY_CA,     ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA,
    WCA | BKCA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, ANY_CA, WCA | BQCA,
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
    Bitboard both = all();

    return  (P_ATTACKS[~att_clr][s] & they & pawns)     |
            (B_ATTACKS[s][both]     & they & bishops)   |
            (R_ATTACKS[s][both]     & they & rooks)     |
            (N_ATTACKS[s]           & they & knights()) |
            (K_ATTACKS[s]           & king);
}

void Board::moves_pseudo(MoveList &list) const
{
    Bitboard attacker = pieces[side];
    Bitboard both     = all();
    Square king       = k_sq[side];

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
                list.save(src, dst);
                // Double push
                if (rank(dst) == push && !get(both, dst + dir))
                    list.save(src, dst + dir, PUSH);
            } else {
                // Promotions
                for (auto type : PROMOTIONS)
                    list.save(src, dst, type);
            }
        }
        // Captures
        Bitboard attacked = P_ATTACKS[side][src] & pieces[~side];

        for (auto dst : BitIter(attacked)) {
            if (rank(dst) == prom) {
                // Promotions with capture
                for (auto type : PROMOTIONS)
                    list.save(src, dst, type);
            } else {
                list.save(src, dst);
            }
        }
        // enPassant
        if (bit(enps_sq) & P_ATTACKS[side][src])
            list.save(src, enps_sq, ENPASS);
    }
    // Knight
    for (auto src : BitIter(attacker & knights())) {

        Bitboard attacked = N_ATTACKS[src]       & ~attacker;

        for (auto dst : BitIter(attacked))
            list.save(src, dst);
    }
    // Bishop / queen
    for (auto src : BitIter(attacker & bishops)) {

        Bitboard attacked = B_ATTACKS[src][both] & ~attacker;

        for (auto dst : BitIter(attacked))
            list.save(src, dst);
    }
    // Rook / queen
    for (auto src : BitIter(attacker & rooks)) {

        Bitboard attacked = R_ATTACKS[src][both] & ~attacker;

        for (auto dst : BitIter(attacked))
            list.save(src, dst);
    }
    // King
    Bitboard attacked = K_ATTACKS[king] & ~attacker;

    for (const auto dst : BitIter(attacked)) 
        list.save(king, dst);

    auto ca_rights = castle >> (2 * side); 

    if (ca_rights & WKCA) {
        auto rook = CASTLE_SQ[side][0];
        if (path_free(king + EAST, rook) && !path_attacked(king, king + EAST + EAST))
            list.save(king, king + EAST + EAST, K_CAST);
    }
    if (ca_rights & WQCA) {
        auto rook = CASTLE_SQ[side][1];
        if (path_free(rook + EAST, king) && !path_attacked(king + WEST, king + EAST))
            list.save(king, king + WEST + WEST, Q_CAST);
    }
}

void Board::moves_legal(MoveList &list) const
{
    Bitboard us     = pieces[side];
    Bitboard they   = pieces[~side];
    Bitboard both   = all();
    Square king     = k_sq[side];

    // ANCHOR King

    Bitboard to_go = K_ATTACKS[king] & ~(us | danger());

    for (const auto dst : BitIter(to_go)) 
        list.save(king, dst);

    Bitboard checkers = 
        (N_ATTACKS[king]        & they & knights()) |
        (P_ATTACKS[side][king]  & they & pawns)     |
        (B_ATTACKS[king][both]  & they & bishops)   |
        (R_ATTACKS[king][both]  & they & rooks);

    Bitboard capture_mask; //   = 0xffffffffffffffff;
    Bitboard quiet_mask; //     = 0xffffffffffffffff;

    switch (cnt(checkers)) {

        case 0: 
            capture_mask = they;
            quiet_mask = ~both;
        break;

        case 1:
        {
            capture_mask = checkers;

            Square checker_sq = lsb(checkers);
        
            if (get(they & bishops & rooks, checker_sq))
                quiet_mask = BETWEEN_BB[king][checker_sq];
            else
                quiet_mask = 0;
        }
        break;

        default: return;
    }
    Bitboard pinned     = 0;
    Bitboard candidates = 
        (B_ATTACKS[king][they]  & they & bishops) |
        (R_ATTACKS[king][they]  & they & rooks);

    for (auto s : BitIter(candidates)) {

        Bitboard pin = BETWEEN_BB[king][s] & us;

        if (cnt(pin) == 1)
            pinned |= pin;
    }

    // auto path_free = [&] (Square src, Square dst) 
    // {
    //     while (src != dst) {
    //         if (get(both, src))
    //             return false;
    //         ++src;
    //     }
    //     return true;
    // };
    // auto path_attacked = [this] (Square src, Square dst) 
    // {
    //     while (src != dst) {
    //         if (attacked(src, ~side))
    //             return true;
    //         ++src;
    //     }
    //     return false;
    // };
    // auto ca_rights = castle >> (2 * side); 

    // if (ca_rights & WKCA) {
    //     auto rook = CASTLE_SQ[side][0];
    //     if (path_free(king + EAST, rook) && !path_attacked(king, king + EAST + EAST))
    //         list.save(king, king + EAST + EAST, K_CAST);
    // }
    // if (ca_rights & WQCA) {
    //     auto rook = CASTLE_SQ[side][1];
    //     if (path_free(rook + EAST, king) && !path_attacked(king + WEST, king + EAST))
    //         list.save(king, king + WEST + WEST, Q_CAST);
    // }
    // // Pawn
    // for (auto src : BitIter(us & pawns)) {

    //     Direction dir   = side == WHITE ? NORTH : SOUTH;
    //     Square dst      = src + dir;
    //     Rank prom       = Rank((7 + side) & 7);
    //     Rank push       = Rank(2 + side * 3);
        
    //     if (!get(both, dst)) {

    //         if (rank(dst) != prom) {
    //             // Quiet
    //             list.save(src, dst);
    //             // Double push
    //             if (rank(dst) == push && !get(both, dst + dir))
    //                 list.save(src, dst + dir, PUSH);
    //         } else {
    //             // Promotions
    //             for (auto type : PROMOTIONS)
    //                 list.save(src, dst, type);
    //         }
    //     }
    //     // Captures
    //     Bitboard to_go = P_ATTACKS[side][src] & they;

    //     for (auto dst : BitIter(to_go)) {
    //         if (rank(dst) == prom) {
    //             // Promotions with capture
    //             for (auto type : PROMOTIONS)
    //                 list.save(src, dst, type);
    //         } else {
    //             list.save(src, dst);
    //         }
    //     }
    //     // enPassant
    //     if (bit(enps_sq) & P_ATTACKS[side][src])
    //         list.save(src, enps_sq, ENPASS);
    // }
    // // Knight
    // for (auto src : BitIter(us & knights())) {

    //     Bitboard to_go = N_ATTACKS[src] & ~us;

    //     for (auto dst : BitIter(to_go))
    //         list.save(src, dst);
    // }
    // // Bishop / queen
    // for (auto src : BitIter(us & bishops)) {

    //     Bitboard to_go = B_ATTACKS[src][both] & ~us;

    //     for (auto dst : BitIter(to_go))
    //         list.save(src, dst);
    // }
    // // Rook / queen
    // for (auto src : BitIter(us & rooks)) {

    //     Bitboard to_go = R_ATTACKS[src][both] & ~us;

    //     for (auto dst : BitIter(to_go))
    //         list.save(src, dst);
    // }
}

Bitboard Board::danger() const
{
    Bitboard bb         = 0;
    Bitboard they       = pieces[~side];
    Bitboard both       = all();
    Bitboard king       = bit(k_sq[side]);
    Square they_king    = k_sq[~side];

    // Pawn
    for (auto src : BitIter(they & pawns))
        bb |= P_ATTACKS[~side][src];
    // Knight
    for (auto src : BitIter(they & knights()))
        bb |= N_ATTACKS[src];
    // Bishop / queen
    for (auto src : BitIter(they & bishops))
        bb |= B_ATTACKS[src][both ^ king];
    // Rook / queen
    for (auto src : BitIter(they & rooks))
        bb |= R_ATTACKS[src][both ^ king];
    // King
    bb |= K_ATTACKS[they_king];

    return bb;
}

void Board::moves(MoveList &list) const
{
    moves_pseudo(list);

    Move *i = list.begin();

    while (i != list.end())
        legal(*i) ? void(++i) : list.erase(i);
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
    castle      &= CASTLE_FROM[src];
    castle      &= CASTLE_TO[dst];

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
            case B_PROM: set(bishops, dst); break;
            case Q_PROM: set(bishops, dst);
            case R_PROM: set(rooks,   dst); break;
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