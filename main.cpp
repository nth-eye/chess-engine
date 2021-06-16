#include <cstdio>
#include <ctime>
#include "bitboard.h"
#include "misc.h"

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
    Bitboard bb = 0xff00ff00ff00ff00;

    printf("Shift:  %3ld clock_t\n", 
        measure_time<100000, false>(bit, H8));

    print_bb(bb);
}
