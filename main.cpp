#include <ctime>
#include <cstdio>
// #include "moon_bitboard.h"
#include "moon_board.h"

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

int main(int, char**) 
{
    printf("sizeof Board: %u", sizeof(moon::Board));
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
