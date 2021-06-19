#include "board.h"
#include "log.h"
#include <cstring>
#include <cctype>

void Board::print()
{
    LOGC('\n');
    for (Rank r = RANK_8; r >= RANK_1; --r) {
        LOG("%c   ", rank_c(r));
        for (File f = FILE_A; f <= FILE_H; ++f) {

            Square s    = sq(r, f);
            char c      = ' ';

            if (!get(all(), s))
                c = '.';
            else if (get(kings, s))
                c = 'k';
            else if (get(pawns(), s))
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
    LOG("enps:  %c          \n", enpass() ? file_c(File(enpass())) : '-');
    LOG("side:  %c          \n", side_c(side));
    LOG("size:  %lu bytes   \n", sizeof(Board));
    LOGC('\n');
}

void Board::clr_pos()
{
    memset(reinterpret_cast<void*>(this), 0, sizeof(Board));
}

bool Board::set_pos(const char *fen)
{
    LOG("%s: %s \n", __func__, fen);

    clr_pos();

    Square s    = A8;
    char c      = *fen;

    while (c != ' ') {

        if (c >= '1' && c <= '8') {
            s += EAST * (c - '0');
        } else if (c == '/') {
            s += SOUTH * 2;
        } else {
            switch (c) {
                case 'p':
                case 'P': set(pawns_en, s); break;
                case 'n':
                case 'N':                   break;
                case 'b':
                case 'B': set(bishops, s);  break;
                case 'q':
                case 'Q': set(bishops, s);
                case 'r':
                case 'R': set(rooks, s);    break;
                case 'k':
                case 'K': set(kings, s);    break;
                default: return false;
            }
            if (c & bit(5))
                set(pieces[BLACK], s);
            else
                set(pieces[WHITE], s);
            ++s;
        }
        c = *++fen;

        if (s < A1 || s > H8 + 1)
            return false;
    }
    c = *++fen;

    switch (c) {
        case 'w': side = WHITE; break;
        case 'b': side = BLACK; break;
        default: return false;
    }
    c = *++fen;

    if (c != ' ')
        return false;
    c = *++fen;

    constexpr const char *castle_str = "KQkq";

    for (int i = 0; i < 4; ++i) {
        if (c != castle_str[i] &&
            c != '-') 
        {
            return false;
        }
        if (c != '-')
            castle |= 1 << i;
        c = *++fen;
    }

    if (c != ' ')
        return false;
    c = *++fen;

    // while (c != ' ') {
    //     switch (c) {
    //         case 'k': castle |= BKCA; break;
    //         case 'K': castle |= WKCA; break;
    //         case 'q': castle |= BQCA; break;
    //         case 'Q': castle |= WQCA; break;
    //         case '-': break;
    //         default: return false;
    //     }
    //     c = *++fen;
    // }
    LOG("FINAL c: %c \n", c);

    return true;
}