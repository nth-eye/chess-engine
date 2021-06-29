#include "engine.h"
#include "log.h"
#include <climits>

void Engine::reset()
{
    position.set_pos(FEN_START);
}

Move Engine::search(int depth)
{
    nodes = 0;

    Move best_move      = NULL_MOVE;
    Score best_score    = INT_MIN + 1;

    MoveList list;

    position.moves(list);

    for (auto move : list) {

        Board tmp   = { position, move };
        Score score = alphabeta(tmp, INT_MIN + 1, INT_MAX - 1, depth - 1);

        if (best_score < score) {
            best_score = score;
            best_move  = move;
        }
    }
    LOG("nodes: %lu \n", nodes);
    return best_move;
}

Score Engine::alphabeta(Board &board, Score alpha, Score beta, int depth)
{
    if (!depth)
        return evaluate(board);

    ++nodes;

    Score score = INT_MIN + 1;

    MoveList list;

    board.moves(list);

    for (auto move : list) {

        Board tmp = { board, move };

        score = -alphabeta(tmp, -beta, -alpha, depth - 1);

        if (score >= beta)
            return beta;

        if (score > alpha)
            alpha = score;
    }
    return alpha;
}

Score Engine::evaluate(Board &board)
{
    Score score = 0;

    score += 1   * cnt(board.pawns     & board.pieces[WHITE]);
    score += 3   * cnt(board.knights() & board.pieces[WHITE]);
    score += 3   * cnt(board.bishops   & board.pieces[WHITE]);
    score += 5   * cnt(board.rooks     & board.pieces[WHITE]);
    // score += 9   * cnt(board.queens()  & board.pieces[WHITE]);
    score += 200 * cnt(board.kings()   & board.pieces[WHITE]);

    score -= 1   * cnt(board.pawns     & board.pieces[BLACK]);
    score -= 3   * cnt(board.knights() & board.pieces[BLACK]);
    score -= 3   * cnt(board.bishops   & board.pieces[BLACK]);
    score -= 5   * cnt(board.rooks     & board.pieces[BLACK]);
    // score -= 9   * cnt(board.queens()  & board.pieces[BLACK]);
    score -= 200 * cnt(board.kings()   & board.pieces[BLACK]);

    return score - (score * 2 * board.side);
}
