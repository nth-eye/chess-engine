#include <ctime>
#include <cstdio>
#include "moon_board.h"
#include "moon_log.h"

using namespace moon;

template<size_t N = 1, class Fn, class ...Args>
clock_t measure_time(Fn &&fn, Args &&...args)
{
    clock_t begin = clock();
    for (size_t i = 0; i < N; ++i) 
        fn(args...);
    clock_t end = clock();

    return (end - begin); // / N;
}

// template<bool Root = true>
// uint64_t perft_(Board board, int depth) 
// {
//     Board tmp           = board;
//     uint64_t nodes      = 0;
//     uint64_t cnt;

//     Moves list;

//     board.moves(list);

//     for (auto move : list) {

//         if (Root && depth <= 1) {
//             cnt = 1;
//             ++nodes;
//         } else {
//             tmp.make(move);

//             if (depth == 2) {
//                 Moves leaf_moves;
//                 tmp.moves(leaf_moves);
//                 cnt = leaf_moves.size();
//             } else {
//                 cnt = perft_<false>(tmp, depth - 1);
//             }
//             nodes += cnt;
//             tmp = board;
//         }
//         if (Root) {
//             print_mv(move); 
//             out("\t %lu \n", cnt);
//         }
//     }
//     return nodes;
// }

// uint64_t perft(Board position, int depth)
// {
//     position.print();

//     clock_t begin = clock();
//     uint64_t all_nodes = perft_<true>(position, depth);
//     clock_t end = clock();
//     clock_t ticks = end - begin;

//     out("Perft to depth %d: %lu nodes visited in %f seconds \n", 
//         depth,
//         all_nodes, 
//         ticks / (double) CLOCKS_PER_SEC);

//     return all_nodes;
// }

// constexpr auto FEN_START    = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

int main(int, char**) 
{
    printf("sizeof Board: %lu \n", sizeof(moon::Board));

    // Board board;

    // board.init(FEN_START);
    // board.print();

    // perft(board, 5);
    // const auto occ = bit(D3);
    // const auto s = D4;

    // print_bb(r_attacks[s][occ]);

    // uci_loop(); 

    // test_perft("../perft.txt", 4);

    // for (auto sq : RanksRev())
    //     printf("%d \n", sq);

    // printf("1: %lu clock_t \n", measure_time<100000000>(check_1));
    // printf("1: %lu clock_t \n", measure_time<100000000>(check_1));
    // printf("1: %lu clock_t \n", measure_time<100000000>(check_1));

    // printf("2: %lu clock_t \n", measure_time<100000000>(check_2));
    // printf("2: %lu clock_t \n", measure_time<100000000>(check_2));
    // printf("2: %lu clock_t \n", measure_time<100000000>(check_2));
}
