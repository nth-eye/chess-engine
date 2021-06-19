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

constexpr auto P_ATTACKS = attacks_pawn();
constexpr auto N_ATTACKS = attacks<KNIGHT>();
constexpr auto B_ATTACKS = attacks<BISHOP>();
constexpr auto R_ATTACKS = attacks<ROOK>();
constexpr auto Q_ATTACKS = attacks<QUEEN>();
constexpr auto K_ATTACKS = attacks<KING>();
const auto B_MAGICS = attacks_magic<BISHOP>(B_MAGIC_NUM);
const auto R_MAGICS = attacks_magic<ROOK>(R_MAGIC_NUM);

int main(int, char**) 
{
    Board board;

    if (board.set_pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq ")) // FEN_START
        board.print();
    else 
        LOG("set_pos: failed \n");

    // Square s        = A6;
    // Bitboard block  = bit(E6) | bit(A3) | bit(G1);

    // print_bb(R_ATTACKS[s]);
    // print_bb(block);
    // print_bb(R_MAGICS[s][block]);

    // printf("cnt:    %ld clock_t \n", measure_time<100000, false>(cnt, SQ_BLACK));

    // Bitboard mask = attacks_mask<ROOK>(A1);

    // for (int i = 0; i < 4095; ++i) {

    //     auto occ = attacks_occupancy(i, mask);

    //     print_bb(occ);
    //     getchar();
    // }

    // print_bb(SQ_EDGES);
    // print_bb(P_ATTACKS[WHITE][s]);
    // print_bb(P_ATTACKS[BLACK][s]);
    // print_bb(N_ATTACKS[s]);
    // print_bb(B_ATTACKS[s]);
    // print_bb(R_ATTACKS[s]);
    // print_bb(Q_ATTACKS[s]);
    // print_bb(K_ATTACKS[s]);

    // auto sliding = attacks_sliding<ROOK>(s, block);
    // auto occupancy = attacks_occupancy<ROOK>(s);

    // print_bb(sliding);
    // print_bb(occupancy);
}
