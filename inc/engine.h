#ifndef ENGINE_H
#define ENGINE_H

#include "board.h"

struct Engine {

    Score negamax(Board board, int depth);
    Score evaluate(Board board);

private:

};

#endif // ENGINE_H