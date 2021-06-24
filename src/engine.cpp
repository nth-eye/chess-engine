#include "engine.h"
#include <climits>

Score Engine::negamax(Board &board, int depth)
{
    if (!depth)
        return evaluate(board);

    Score max = INT_MIN;

    for (const auto move : gen_moves(board)) {

        Board tmp = { board, move };

        Score score = -negamax(tmp, depth - 1);

        if (max < score)
            max = score;
    }
    return max;
}

Score Engine::evaluate(Board &board)
{
    
}

MoveList Engine::gen_moves(Board &board)
{
    MoveList list;
    board.moves(list);
    return list;
}