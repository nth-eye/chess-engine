#ifndef ENGINE_H
#define ENGINE_H

#include "board.h"

struct Engine {

    bool set(const char *fen);
    void reset();
    Move search(int depth);
    MoveList moves();
    uint64_t perft(int depth);
private:
    Score alphabeta(Board &board, Score alpha, Score beta, int depth);
    Score negamax(Board &board, int depth);
    Score evaluate(Board &board);
    
    Board position;
    uint64_t nodes;
};

#endif // ENGINE_H