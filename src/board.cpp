#include "board.h"
#include "log.h"

void Board::print()
{
    LOGC('\n');
    for (Rank r = RANK_8; r >= RANK_1; --r) {
        LOG("%c   ", rank_c(r));
        for (File f = FILE_A; f <= FILE_H; ++f) {

            Square s    = sq(r, f);
            char c      = ' ';

            if (!get(all(), s)) {
                c = '.';
            } else if (get(kings, s)) {
                c = 'k';
            } else if (get(pawns(), s)) {
                c = 'p';
            } else if (get(bishops, s)) {
                if (get(rooks, s))
                    c = 'q';
                else
                    c = 'b';
            } else if (get(rooks, s)) {
                c = 'r';
            } else {
                c = 'n';
            }

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
    LOG("side:  %c          \n", side_c(side));
    LOG("size:  %lu bytes   \n", sizeof(Board));
    LOGC('\n');
}