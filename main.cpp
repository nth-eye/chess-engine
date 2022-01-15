// #include "uci.h"
// #include "util.h"

#include <ctime>
#include <cstdio>
#include "moon_util.h"

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

// #define OUT                 printf     

// constexpr char file_c(File f)   { return 'a' + f; }
// constexpr char rank_c(Rank r)   { return '1' + r; }
// constexpr char side_c(Color c)  { return c == WHITE ? 'w' : 'b'; }

// void print_bb(Bitboard bb)
// {
//     OUT("\n");
//     for (auto r : RanksRev()) {
//         OUT("%c   ", rank_c(r));
//         for (auto f : Files())
//             OUT("%c ", get(bb, sq(r, f)) ? 'X' : '-');
//         OUT("\n");
//     }
//     OUT("\n   ");
//     for (auto f : Files())
//         OUT(" %c", file_c(f));
//     OUT("\n\nhex:   %016lx \n\n", bb);
// }

int main(int, char**) 
{
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
