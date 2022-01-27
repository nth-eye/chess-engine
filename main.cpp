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

#define PERFT_LOG true

template<bool Root = true>
uint64_t perft_(Board board, int depth) 
{
    Board tmp           = board;
    uint64_t nodes      = 0;
    uint64_t cnt;

    Moves list;

    board.moves_all(list);

    for (auto move : list) {

        if (Root && depth <= 1) {
            cnt = 1;
            ++nodes;
        } else {
            tmp.make(move);

            if (depth == 2) {
                Moves leaf_moves;
                tmp.moves_all(leaf_moves);
                cnt = leaf_moves.size();
            } else {
                cnt = perft_<false>(tmp, depth - 1);
            }
            nodes += cnt;
            tmp = board;
        }
#if PERFT_LOG
        if (Root) {
            print_mv(move); 
            out("\t %lu \n", cnt);
        }
#endif
    }
    return nodes;
}

uint64_t perft(Board position, int depth)
{
#if PERFT_LOG
    print_board(position);
#endif
    clock_t begin = clock();
    uint64_t all_nodes = perft_<true>(position, depth);
    clock_t end = clock();
    clock_t ticks = end - begin;
#if PERFT_LOG
    out("Perft to depth %d: %lu nodes visited in %f seconds \n", 
        depth,
        all_nodes, 
        ticks / (double) CLOCKS_PER_SEC);

    uint64_t nps = double(all_nodes) / (ticks / double(CLOCKS_PER_SEC));

    out("%lu nodes / sec \n", nps);
#endif
    return all_nodes;
}

#include <cstdlib>
#include <cstring>

#define SIZE(x) (sizeof(x) / sizeof(x[0]))

bool str_to_int(const char *str, long *val, int base, char **end)
{
    long res = strtol(str, end, base);

    if (*end > str) {
        *val = res;
        return true;
    }
    return false;
}

size_t split(char *str, char *ptr_arr[], const char *delim, size_t max_n_lines)
{  
    char *split = strtok(str, delim);
    size_t i = 0;

    while (split && i < max_n_lines) {
        ptr_arr[i++] = split;
        split = strtok(NULL, delim);
    }
    ptr_arr[i] = 0;

    return i;
}

void test_perft(const char *file_name, size_t depth)
{
    Board board;

    uint64_t line_cnt       = 0;
    uint64_t correct_nodes  = 0;

    char line[256];
    char *words[16];

    FILE *fp = fopen(file_name, "r");

    while (fgets(line, SIZE(line), fp)) {

        ++line_cnt;

        printf("[%04lu]: %s", line_cnt, line);

        size_t n_words = split(line, words, ",", SIZE(words));

        if (n_words < depth + 1) {
            printf("[%04lu]: not enough arguments \n", line_cnt);
            continue;
        }

        char *end = NULL;

        if (!str_to_int(words[depth], (long*) &correct_nodes, 10, &end)) {
            printf("[%04lu]: illegal number of nodes \n", line_cnt);
            continue;
        }

        if (!board.set_fen(words[0])) {
            printf("[%04lu]: set_pos failed \n", line_cnt);
            continue;
        }

        uint64_t nodes = perft(board, depth);

        if (nodes != correct_nodes) {
            printf("[%04lu]: %lu != %lu in %s \n", line_cnt, nodes, correct_nodes, words[0]);
            fclose(fp);
            return;
        }
    }
    printf("<%s>: all tests successfully passed for depth %lu \n", __func__, depth);

    fclose(fp);
}

int main(int, char**) 
{
    printf("sizeof Board: %lu \n", sizeof(moon::Board));

    // constexpr auto test =  "8/8/8/8/2r1b2k/2PP4/2K5/8 w - - 0 1";
    
    // "8/8/2r5/8/1b5k/2P5/2K5/8 w - - 0 1";
    // "8/8/8/8/1n1b3k/2P5/2K5/8 w - - 0 1"; 
    // "8/8/8/8/1n5k/2P5/2K5/8 w - - 0 1";
    // "8/8/2r5/8/1n5k/2P5/2K5/8 w - - 0 1";
    // "7k/8/8/8/8/3b4/2P5/1K6 w - - 0 1";
    // "7k/8/8/1r6/8/2b5/1P6/1K6 w - - 0 1"; 
    // "2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1"; 
    // "7k/2r5/8/2R5/8/2K5/8/8 w - - 0 1";
    // "7k/2r3b1/8/2R5/8/2K5/8/8 w - - 0 1";
    // "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; 
    
    Moves list;
    Board board;

    board.set_fen(fen::START);
    board.moves_all(list);
    
    // print_board(board);
    // print_moves(list);
    perft(board, 7);
    // test_perft("../perft.txt", 5);

    // printf("2: %lu clock_t \n", measure_time<1000000000>(&Board::restricted_2, &board));
    // printf("2: %lu clock_t \n", measure_time<1000000000>(&Board::restricted_2, &board));
    // printf("2: %lu clock_t \n", measure_time<1000000000>(&Board::restricted_2, &board));

    // printf("1: %lu clock_t \n", measure_time<1000000000>(&Board::restricted, &board));
    // printf("1: %lu clock_t \n", measure_time<1000000000>(&Board::restricted, &board));
    // printf("1: %lu clock_t \n", measure_time<1000000000>(&Board::restricted, &board));

    // printf("1: %lu clock_t \n", measure_time<100000000>(check_1));
    // printf("1: %lu clock_t \n", measure_time<100000000>(check_1));
    // printf("1: %lu clock_t \n", measure_time<100000000>(check_1));

    // printf("2: %lu clock_t \n", measure_time<100000000>(check_2));
    // printf("2: %lu clock_t \n", measure_time<100000000>(check_2));
    // printf("2: %lu clock_t \n", measure_time<100000000>(check_2));
}
