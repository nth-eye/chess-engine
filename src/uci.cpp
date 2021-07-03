#include <cstring>
#include <cstdlib>
#include "uci.h"
#include "log.h"

#define VERSION "1.0"
#define SIZE(x) (sizeof(x) / sizeof(x[0]))

using uci_cb = void(*)();

struct UCI_Cmd {
    const char      *str;
    const uci_cb    cb; 
};

static char line[1024];
static char *words[512];

static const UCI_Cmd UCI_COMMANDS[] = {
    { "quit", quit },
};

// Splits string by given delimiter and saves pointers to "ptr_arr". Modifies string.
static size_t split(char *str, char *ptr_arr[], const char *delim, size_t max_n_lines)
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

void UCI::loop() 
{
    engine.reset();

    id();

    while (fgets(line, SIZE(line), stdin)) {

        line[strcspn(line, "\n")] = 0;

        size_t n_words = split(line, words, " ", SIZE(words));

        for (size_t i = 0; i < n_words; ++i)
            puts(words[i]);

        if (!n_words)
            continue;

        for (size_t i = 0; i < SIZE(UCI_COMMANDS); ++i) {

            if (!strcmp(words[0], UCI_COMMANDS[i].str)) {

                printf("found command: %s \n", UCI_COMMANDS[i].str);

                UCI_COMMANDS[i].cb();
            } else {
                printf("unkown command: %s \n", words[0]);
            }
        }
    }
}

Move UCI::move(const char *c)
{
    Square src = sq(Rank(c[1] - '1'), File(c[0] - 'a'));
    Square dst = sq(Rank(c[3] - '1'), File(c[2] - 'a'));

    for (auto move : engine.moves()) {

        if (from(move) == src && to(move) == dst) {

            MoveFlag f = flag(move);

            if (f & N_PROM) {

                if (c[4] == 'n' && f == N_PROM)
                    return move;

                if (c[4] == 'b' && f == B_PROM)
                    return move;

                if (c[4] == 'r' && f == R_PROM)
                    return move;

                if (c[4] == 'q' && f == Q_PROM)
                    return move;

                continue;
            }
            if (!c[4])
                return move;
        }
    }
    return NULL_MOVE;
}

void quit()
{
    exit(0);
}

void id()
{
    printf("id name nth-eye %s %s \nid author Ilya Makarov\n", VERSION, __DATE__);
}

void uciok()
{
    puts("uciok");
}

void readyok()
{
    puts("readyok");
}