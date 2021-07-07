#include "test.h"
#include "engine.h"
#include "log.h"
#include <cstring>

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

        if (!engine.set(words[0])) {
            printf("[%04lu]: set_pos failed \n", line_cnt);
            continue;
        }

        uint64_t nodes = engine.perft(depth);

        if (nodes != correct_nodes) {
            printf("[%04lu]: %lu != %lu in %s \n", line_cnt, nodes, correct_nodes, words[0]);
            fclose(fp);
            return;
        }
    }
    printf("<%s>: all tests successfully passed for depth %lu \n", __func__, depth);

    fclose(fp);
}