#include "test.h"
#include "board.h"
#include "log.h"
#include <fstream>
#include <sstream>

void test_perft(const char *file_name)
{
    std::ifstream ifs{file_name};
    std::string line;
    std::string word;

    const char *position = NULL;
    uint64_t line_cnt = 0;
    uint64_t correct_nodes = 0;
    int depth = 1;

    Board board;

    while (std::getline(ifs, line)) {

        ++line_cnt;

        // printf("[%04d]: %s \n", line_cnt, line.c_str());

        std::stringstream ss{line};
        std::vector<std::string> words;

        while (std::getline(ss, word, ','))
            words.push_back(word);

        if (words.size() < 2) {
            printf("[%04lu]: not enough arguments \n", line_cnt);
            continue;
        }

        position        = words[0].c_str();
        correct_nodes   = std::stoi(words[1]);

        // printf("[%04lu]:            \n\
        //     position        = %s    \n\
        //     correct_nodes   = %lu   \n", 
        //     line_cnt, position, correct_nodes);

        if (!board.set_pos(position)) {
            printf("[%04lu]: set_pos failed \n", line_cnt);
            continue;
        }

        uint64_t nodes = perft<true>(board, depth);

        if (nodes != correct_nodes) {
            printf("[%04lu]: %lu != %lu in %s \n", line_cnt, nodes, correct_nodes, position);
            return;
        }

        // if (line_cnt >= 10)
        //     return;
    }
    printf("ALL TESTS successfully finished for depth %d \n", depth);
}