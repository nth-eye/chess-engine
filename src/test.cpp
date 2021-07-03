#include "test.h"
#include "engine.h"
#include "log.h"
#include <fstream>
#include <sstream>
#include <vector>

void test_perft(const char *file_name)
{
    constexpr int depth     = 5;
    const char *position    = NULL;
    uint64_t line_cnt       = 0;
    uint64_t correct_nodes  = 0;
    Engine engine;

    std::ifstream ifs{file_name};
    std::string line;
    std::string word;

    while (std::getline(ifs, line)) {

        ++line_cnt;

        printf("[%04lu]: %s \n", line_cnt, line.c_str());

        std::stringstream ss{line};
        std::vector<std::string> words;

        while (std::getline(ss, word, ','))
            words.push_back(word);

        if (words.size() < depth + 1) {
            printf("[%04lu]: not enough arguments \n", line_cnt);
            continue;
        }

        position        = words[0].c_str();
        correct_nodes   = std::stoi(words[depth]);

        if (!engine.set(position)) {
            printf("[%04lu]: set_pos failed \n", line_cnt);
            continue;
        }

        uint64_t nodes = engine.perft(depth);

        if (nodes != correct_nodes) {
            printf("[%04lu]: %lu != %lu in %s \n", line_cnt, nodes, correct_nodes, position);
            return;
        }
    }
    printf("ALL TESTS successfully finished for depth %d \n", depth);
}