#ifndef ENGINE_H
#define ENGINE_H

#include "board.h"

struct Engine {

    void reset();
    Move search(int depth);
    Score alphabeta(Board &board, Score alpha, Score beta, int depth);
    Score negamax(Board &board, int depth);
    Score evaluate(Board &board);
private:
    Board position;
    uint64_t nodes;
};

#endif // ENGINE_H