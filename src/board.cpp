#include "board.h"
#include "log.h"
#include <cstring>
#include <cstdlib>

constexpr Bitboard CASTLE_MASK[2][3] = { 
    { 0x0000000000000060, 0x000000000000000e, 0x000000000000000c }, 
    { 0x6000000000000000, 0x0e00000000000000, 0x0c00000000000000 },
};
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
constexpr MoveFlag PROMOTIONS[] = { N_PROM, B_PROM, R_PROM, Q_PROM };
constexpr auto BETWEEN_BB   = betweens();
constexpr auto LINE_BB      = lines();
constexpr auto P_ATTACKS    = attacks_pawn();
constexpr auto N_ATTACKS    = attacks<KNIGHT>();
constexpr auto K_ATTACKS    = attacks<KING  >();
const auto B_ATTACKS        = attacks_magic<BISHOP>(B_MAGIC_NUM);
const auto R_ATTACKS        = attacks_magic<ROOK  >(R_MAGIC_NUM);

bool str_to_int(const char *str, long *val, int base, char **end)
{
    long res = strtol(str, end, base);

    if (*end > str) {
        *val = res;
        return true;
    }
    return false;
}

static void add_moves(Square src, Bitboard all_dst, MoveList &list)
{
    for (auto dst : BitIter(all_dst))
        list.save(src, dst);
}

void Board::print() const
{
    LOG("\n");
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
        LOG("\n");
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
        LOG("-");
    LOG("\n");
    LOG("half:  %u          \n", half_clk);
    LOG("full:  %u          \n", full_clk);
    LOG("side:  %c          \n", side_c(side));
    LOG("size:  %lu bytes   \n", sizeof(Board));
    LOG("\n");
}

void Board::clr_pos()
{
    memset(reinterpret_cast<void*>(this), 0, sizeof(Board));
}

bool Board::set_pos(const char *c)
{
    LOG("<%s>: %s \n", __func__, c);

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

    if (*c == '-') {
        ++c;
    } else {
        int i = 0;
        while (*c != ' ' && i < 4) {
            switch (*c++) {
                case 'K': castle |= WKCA; break;
                case 'Q': castle |= WQCA; break;
                case 'k': castle |= BKCA; break;
                case 'q': castle |= BQCA; break;
                default: return false;
            }
            ++i;
        }
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

    if (strlen(end))
        LOG("<%s>: remainder - [%s] \n", __func__, end);

    return true;
}

template<Color Side>
Bitboard Board::restricted() const
{
    Bitboard bb         = 0;
    Bitboard they       = pieces[~Side];
    Bitboard both       = all();
    Bitboard king       = bit(k_sq[Side]);
    Square they_king    = k_sq[~Side];
    // Pawn
    for (auto src : BitIter(they & pawns))
        bb |= P_ATTACKS[~Side][src];
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

template<Color Side>
bool Board::attacked(Square s) const 
{
    Bitboard they = pieces[Side];
    Bitboard king = bit(k_sq[Side]);
    Bitboard both = all();

    return  (P_ATTACKS[~Side][s]    & they & pawns)     |
            (B_ATTACKS[s][both]     & they & bishops)   |
            (R_ATTACKS[s][both]     & they & rooks)     |
            (N_ATTACKS[s]           & they & knights()) |
            (K_ATTACKS[s]           & king);
}

template<Color Side>
bool Board::legal(Move move) const 
{
    Board board = { *this, move };

    return !board.attacked<~Side>(board.k_sq[Side]);
}

bool Board::legal(Move move) const
{
    MoveList list;
    moves(list);

    for (auto m : list)
        if (m == move)
            return true;
    return false;
}

template<Color Side>
void Board::moves_all(MoveList &list) const
{
    Square king     = k_sq[Side];
    Bitboard us     = pieces[Side]; 
    Bitboard they   = pieces[~Side];
    Bitboard both   = all();
    Bitboard danger = restricted<Side>();
    Bitboard to_go  = K_ATTACKS[king] & ~(us | danger);

    add_moves(king, to_go, list);

    Bitboard checkers = 
        (N_ATTACKS[king]        & they & knights()) |
        (P_ATTACKS[Side][king]  & they & pawns)     |
        (B_ATTACKS[king][both]  & they & bishops)   |
        (R_ATTACKS[king][both]  & they & rooks);

    Bitboard pinned     = 0;
    Bitboard pinners    = 
        (B_ATTACKS[king][they]  & they & bishops) |
        (R_ATTACKS[king][they]  & they & rooks);
    
    for (auto s : BitIter(pinners)) {

        Bitboard pin = BETWEEN_BB[king][s] & us;

        if (cnt(pin) == 1)
            pinned |= pin;
    }

    Bitboard capture_mask;
    Bitboard quiet_mask;
    Bitboard legal_mask;

    switch (cnt(checkers)) {

        case 0: 
        {
            capture_mask    = they;
            quiet_mask      = ~both;
            legal_mask      = capture_mask | quiet_mask;

            auto ca_rights = castle >> (2 * Side); 

            if (ca_rights & WKCA && !((both | danger) & CASTLE_MASK[Side][0]))
                list.save(king, king + EAST + EAST, K_CAST);

            if (ca_rights   & WQCA && 
                !(both      & CASTLE_MASK[Side][1]) && 
                !(danger    & CASTLE_MASK[Side][2]))
                list.save(king, king + WEST + WEST, Q_CAST);

            moves_pawn<Side, true>(pinned & pawns, capture_mask, quiet_mask, list);

            for (auto src : BitIter(pinned & bishops))
                add_moves(src, B_ATTACKS[src][both] & legal_mask & LINE_BB[king][src], list);
            
            for (auto src : BitIter(pinned & rooks))
                add_moves(src, R_ATTACKS[src][both] & legal_mask & LINE_BB[king][src], list);
        }
        break;

        case 1:
        {
            capture_mask = checkers;

            Square checker_sq = lsb(checkers);
        
            if (get(they & (bishops | rooks), checker_sq))
                quiet_mask = BETWEEN_BB[king][checker_sq];
            else
                quiet_mask = 0;
            
            legal_mask = capture_mask | quiet_mask;
        }
        break;

        default: return;
    }
    Bitboard not_pinned = ~pinned;

    if (enps_sq) {

        Bitboard bb = P_ATTACKS[~Side][enps_sq] & pawns & us;

        for (auto src : BitIter(bb)) {
            Move move = mv(src, enps_sq, ENPASS);
            if (legal<Side>(move))
                list.save(move);
        }
    }
    moves_pawn<Side, false>(us & not_pinned & pawns, capture_mask, quiet_mask, list);
    
    for (auto src : BitIter(us & not_pinned & knights()))
        add_moves(src, N_ATTACKS[src]       & legal_mask, list);

    for (auto src : BitIter(us & not_pinned & bishops))
        add_moves(src, B_ATTACKS[src][both] & legal_mask, list);

    for (auto src : BitIter(us & not_pinned & rooks))
        add_moves(src, R_ATTACKS[src][both] & legal_mask, list);
}

template<Color Side, bool Pinned>
void Board::moves_pawn(Bitboard selected_pawns, Bitboard capture_mask, Bitboard quiet_mask, MoveList &list) const
{
    constexpr Direction dir = Side == WHITE ? NORTH : SOUTH;
    constexpr Rank prom_r   = Side == WHITE ? RANK_7 : RANK_2;
    constexpr Rank push_r   = Side == WHITE ? RANK_2 : RANK_7; //Rank(2 + side * 3);

    Bitboard both   = all();
    Square king     = k_sq[Side];

    for (auto src : BitIter(selected_pawns)) {

        if (rank(src) == prom_r) {

            Bitboard bb = P_ATTACKS[Side][src] & capture_mask;

            if (Pinned)
                bb &= LINE_BB[king][src];

            for (auto dst : BitIter(bb))
                for (auto type : PROMOTIONS)
                    list.save(src, dst, type);

            bb = bit(src + dir) & quiet_mask;

            if (Pinned)
                bb &= LINE_BB[king][src];

            for (auto dst : BitIter(bb))
                for (auto type : PROMOTIONS)
                    list.save(src, dst, type);
        } else {
            Bitboard bb = P_ATTACKS[Side][src] & capture_mask;
            
            if (Pinned)
                bb &= LINE_BB[king][src];
            
            add_moves(src, bb, list);

            bb = bit(src + dir) & quiet_mask;
            
            if (Pinned)
                bb &= LINE_BB[king][src];

            add_moves(src, bb, list);

            if (rank(src) == push_r && !get(both, src + dir)) {

                bb = bit(src + dir + dir) & quiet_mask;
                
                if (Pinned)
                    bb &= LINE_BB[king][src];

                for (auto dst : BitIter(bb))
                    list.save(src, dst, PUSH);
            }
        }
    }
}

void Board::moves(MoveList &list) const
{
    side == WHITE ? moves_all<WHITE>(list) : moves_all<BLACK>(list);
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
