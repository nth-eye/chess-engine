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
