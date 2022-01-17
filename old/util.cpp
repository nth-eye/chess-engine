#include <cstdlib>
#include <cstring>
#include "util.h"
#include "log.h"
#include "engine.h"

// Splits string by given delimiter and saves pointers to "ptr_arr". Modifies string.
size_t split(char *str, char *ptr_arr[], const char *delim, size_t max_n_toks)
{  
    char *split = strtok(str, delim);
    size_t i = 0;

    while (split && i < max_n_toks) {
        ptr_arr[i++] = split;
        split = strtok(NULL, delim);
    }
    ptr_arr[i] = 0;

    return i;
}

bool str_to_int(const char *str, long *val, int base, char **end)
{
    long res = strtol(str, end, base);

    if (*end > str) {
        *val = res;
        return true;
    }
    return false;
}

void print_sq(Square s)
{
    OUT("%c%c", 'a' + file(s), '1' + rank(s));
}

void print_bb(Bitboard bb)
{
    OUT("\n");
    for (Rank r = RANK_8; r >= RANK_1; --r) {
        OUT("%c   ", rank_c(r));
        for (File f = FILE_A; f <= FILE_H; ++f)
            OUT("%c ", get(bb, sq(r, f)) ? 'X' : '-');
        OUT("\n");
    }
    OUT("\n   ");
    for (File f = FILE_A; f <= FILE_H; ++f)
        OUT(" %c", file_c(f));
    OUT("\n\n");
    OUT("hex:   %016lx \n", bb);
    OUT("\n");
}

void print_mv(Move m) 
{
    print_sq(from(m));
    print_sq(to(m));
    switch (flag(m)) {
        case N_PROM: OUT("n"); break;
        case B_PROM: OUT("b"); break;
        case R_PROM: OUT("r"); break;
        case Q_PROM: OUT("q"); break;
        default:;
    }
}

void print_moves(const MoveList &list) 
{
    // constexpr const char *bit_rep[16] = {
    //     "0000", "0001", "0010", "0011",
    //     "0100", "0101", "0110", "0111",
    //     "1000", "1001", "1010", "1011",
    //     "1100", "1101", "1110", "1111",
    // };
	OUT("MoveList: %lu \n", list.size());
	for (auto move : list) {
        OUT("\n move: %04x  ", move);
        // for (size_t i = 0; i < sizeof(Move) * 2; ++i)
        //     OUT("%s ", bit_rep[(move >> (12 - i * 4)) & 0xf]);
        print_mv(move);
    }
    OUT("\n");
}

void test_perft(const char *file_name, size_t depth)
{
    Engine engine;

    uint64_t line_cnt       = 0;
    uint64_t correct_nodes  = 0;

    char line[256];
    char *words[16];

    FILE *fp = fopen(file_name, "r");

    while (fgets(line, SIZE(line), fp)) {

        ++line_cnt;

        OUT("[%04lu]: %s", line_cnt, line);

        size_t n_words = split(line, words, ",", SIZE(words));

        if (n_words < depth + 1) {
            OUT("[%04lu]: not enough arguments \n", line_cnt);
            continue;
        }

        char *end = NULL;

        if (!str_to_int(words[depth], (long*) &correct_nodes, 10, &end)) {
            OUT("[%04lu]: illegal number of nodes \n", line_cnt);
            continue;
        }

        if (!engine.set(words[0])) {
            OUT("[%04lu]: set_pos failed \n", line_cnt);
            continue;
        }

        uint64_t nodes = engine.perft(depth);

        if (nodes != correct_nodes) {
            OUT("[%04lu]: %lu != %lu in %s \n", line_cnt, nodes, correct_nodes, words[0]);
            fclose(fp);
            return;
        }
    }
    OUT("all tests successfully passed for depth %lu \n", depth);

    fclose(fp);
}
