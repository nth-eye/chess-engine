#include <ctime>
#include "log.h"
#include "board.h"

// Measure execution time of a function.
template<size_t N = 1, bool Avg = true, class Fn, class ...Args>
clock_t measure_time(Fn &&fn, Args &&...args)
{
    clock_t begin = clock();
    for (size_t i = 0; i < N; ++i) 
        fn(args...);
    clock_t end = clock();
    clock_t res = end - begin;
    return Avg ? res / N : res;
}

int main(int, char**) 
{
    Board board;

    if (board.set_pos(FEN_START)) // "8/8/8/3p4/4P3/8/8/8 w - - 0 1"
        board.print();
    else 
        LOG("set_pos: failed \n");

    MoveList moves = board.moves_pseudo();

    print_moves(moves);

    // LOG("attacked: %d \n", board.attacked(E4, BLACK));

    // Square s        = A6;
    // Bitboard block  = bit(E6) | bit(A3) | bit(G1);

    // print_bb(R_ATTACKS[s]);
    // print_bb(block);
    // print_bb(R_MAGICS[s][block]);

    // Bitboard mask = attacks_mask<ROOK>(A1);

    // for (int i = 0; i < 4095; ++i) {

    //     auto occ = attacks_occupancy(i, mask);

    //     print_bb(occ);
    //     getchar();
    // }
}
