#include <ctime>
#include <cstdio>
#include "moon_board.h"
#include "moon_log.h"

using namespace moon;

template<size_t N = 1, class Fn, class Ptr, class ...Args>
clock_t measure_time(Fn &&fn, Ptr *ptr, Args &&...args)
{
    clock_t begin = clock();
    for (size_t i = 0; i < N; ++i) 
        (ptr->*fn)(args...);
    clock_t end = clock();

    return (end - begin);// / N;
}

// template<size_t N = 1, class Fn, class ...Args>
// clock_t measure_time(Fn &&fn, Args &&...args)
// {
//     clock_t begin = clock();
//     for (size_t i = 0; i < N; ++i) 
//         fn(args...);
//     clock_t end = clock();

//     return (end - begin); // / N;
// }

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

    constexpr auto test = "7k/2r5/8/2R5/8/2K5/8/8 w - - 0 1";
    constexpr auto a = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; // "7k/2r3b1/8/2R5/8/2K5/8/8 w - - 0 1";
    
    Moves list;
    Board board;

    board.set_fen(fen::START);
    board.moves_all(list);
    
    print_board(board);
    print_moves(list);

    // auto them_p = board.pieces[~board.side] & board.pawns;
    // auto pawn_att = shift()

    // print_bb(them_p);
    // print_bb(board.restricted());

    // printf("2: %lu clock_t \n", measure_time<1000000000>(&Board::restricted_2, &board));
    // printf("2: %lu clock_t \n", measure_time<1000000000>(&Board::restricted_2, &board));
    // printf("2: %lu clock_t \n", measure_time<1000000000>(&Board::restricted_2, &board));

    // printf("1: %lu clock_t \n", measure_time<1000000000>(&Board::restricted, &board));
    // printf("1: %lu clock_t \n", measure_time<1000000000>(&Board::restricted, &board));
    // printf("1: %lu clock_t \n", measure_time<1000000000>(&Board::restricted, &board));

    // print_bb(board.pinned());
    // print_bb(board.checkers());
    // print_bb(board.restricted());


    // board.moves_pawn_quiet(list);
    // print_moves(list);

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
