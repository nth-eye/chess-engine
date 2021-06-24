#include <ctime>
#include <chrono>
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
    // constexpr auto BETWEEN = betweens();

    // for (Square src = A1; src <= A1; ++src) {
    //     for (Square dst = A1; dst <= H8; ++dst) {
    //         print_sq(src);
    //         printf(" - ");
    //         print_sq(dst);
    //         printf("\n");
    //         print_bb(BETWEEN[src][dst]);
    //     }
    // }

    constexpr auto checker = "4k3/8/6n1/4R3/8/8/8/4K3 b - - 0 1";
    constexpr auto pinner = "4k3/8/4r3/8/8/4Q3/8/2K5 b - - 0 1";
    // "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

    Board board;

    if (board.set_pos(FEN_START))
        board.print();
    else 
        LOG("set_pos: failed \n");

    MoveList list;

    // board.moves_legal(list);

    auto depth = 1;

    LOG("\nStarting test to depth %d \n", depth);

    auto start = std::chrono::steady_clock::now();
    auto all_nodes = perft<true>(board, depth);
    auto time = std::chrono::steady_clock::now() - start;

    LOG("\nTest completed: %lu nodes visited in %f ms \n", all_nodes, time.count() / 1'000'000.0);

    // printf("test:   %lu clock_t \n", measure_time<10000>(perft, board, depth));
}
