#include <climits>
#include <ctime>
#include "engine.h"
#include "log.h"

template<bool Root = true>
uint64_t perft_(Board board, int depth) 
{
    Board tmp           = board;
    uint64_t nodes      = 0;
    uint64_t cnt;

    MoveList list;

    board.moves(list);

    for (auto move : list) {

        if (Root && depth <= 1) {
            cnt = 1;
            ++nodes;
        } else {
            tmp.make_move(move);

            if (depth == 2) {
                MoveList leaf_moves;
                tmp.moves(leaf_moves);
                cnt = leaf_moves.size();
                // cnt = tmp.moves().size();
            } else {
                cnt = perft_<false>(tmp, depth - 1);
            }
            nodes += cnt;
            tmp = board;
        }
        if (Root) {
            print_mv(move); 
            LOG("\t %lu \n", cnt);
        }
    }
    return nodes;
}

bool Engine::set(const char *fen)
{
    return position.set_pos(fen);
}

void Engine::reset()
{
    set(FEN_START);
}

Move Engine::search(int depth)
{
    nodes = 0;

    Move best_move      = NULL_MOVE;
    Score best_score    = INT_MIN + 1;

    for (auto move : moves()) {

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

MoveList Engine::moves()
{
    MoveList list;

    position.moves(list);

    return list;
}

uint64_t Engine::perft(int depth)
{
    clock_t begin = clock();

    uint64_t all_nodes = perft_<true>(position, depth);

    clock_t end = clock();
    clock_t ticks = end - begin;

    LOG("Perft to depth [%d]: %lu nodes visited in %f seconds \n", 
        depth,
        all_nodes, 
        ticks / (double) CLOCKS_PER_SEC);

    return nodes;
}
