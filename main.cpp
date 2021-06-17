#include <cstdio>
#include <ctime>
#include "bitboard.h"

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
constexpr auto B_ATTACKS = attacks<BISHOP>();
constexpr auto R_ATTACKS = attacks<ROOK>();
constexpr auto Q_ATTACKS = attacks<QUEEN>();
constexpr auto K_ATTACKS = attacks<KING>();

int main(int, char**) 
{
    Square s        = A6;
    Bitboard block  = bit(E6) | bit(A3);

    // printf("bit:    %ld clock_t\n", measure_time<100000, false>(bit, s));

    // print_bb(SQ_EDGES);
    // print_bb(P_ATTACKS[WHITE][s]);
    // print_bb(P_ATTACKS[BLACK][s]);
    // print_bb(N_ATTACKS[s]);
    // print_bb(B_ATTACKS[s]);
    // print_bb(R_ATTACKS[s]);
    // print_bb(Q_ATTACKS[s]);
    // print_bb(K_ATTACKS[s]);

    auto sliding = attacks_sliding<ROOK>(s, block);
    auto occupancy = attacks_occupancy<ROOK>(s);

    print_bb(sliding);
    print_bb(occupancy);
}
