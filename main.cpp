#include <ctime>
#include "log.h"
#include "board.h"
#include "test.h"

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

    if (board.set_pos(FEN_START))
        board.print();
    else 
        printf("set_pos: failed \n");

    constexpr int depth = 4;

    printf("Test to depth [%d]: %lu nodes visited in %f seconds \n", 
        depth,
        perft<true>(board, depth), 
        measure_time<1000>(perft<true>, board, depth) / (double) CLOCKS_PER_SEC);

    // test_perft("../perft.txt");
}
