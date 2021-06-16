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

constexpr auto P_ATTACKS = attacks_pawn();
constexpr auto N_ATTACKS = attacks<KNIGHT>();

int main(int, char**) 
{
    printf("bit:    %ld clock_t\n", 
        measure_time<100000, false>(bit, H8));

    Bitboard bb = 0xff00ff00ff00ff00;

    print_bb(bb);
    print_bb(P_ATTACKS[WHITE][A1]);
    print_bb(P_ATTACKS[BLACK][E2]);
    print_bb(N_ATTACKS[F7]);
}
