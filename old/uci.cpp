#include <cstring>
#include <cstdlib>
#include "uci.h"
#include "engine.h"
#include "util.h"
#include "log.h"

#define VERSION "1.0"

using uci_cb = void(*)();

struct UCI_Cmd {
    const char      *str;
    const uci_cb    cb; 
};

static const UCI_Cmd UCI_COMMANDS[] = {
    { "uci",        uci_uci },
    { "debug",      uci_debug },
    { "isready",    uci_isready },
    { "setoption",  uci_setoption },
    { "register",   uci_register },
    { "ucinewgame", uci_ucinewgame },
    { "position",   uci_position },
    { "go",         uci_go },
    { "stop",       uci_stop },
    { "ponderhit",  uci_ponderhit },
    { "quit",       uci_quit },
};

static char line[1024];
static char *words[512];
static Engine engine;
static size_t cmd_words;

static Move str_to_move(const char *c)
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

static bool parse_moves(size_t token_idx)
{
    OUT("<%s>: tokens [%lu] - [%lu] \n", __func__, token_idx, cmd_words);

    for (size_t i = token_idx; i < cmd_words; ++i) {

        Move move = str_to_move(words[i]);

        if (move == NULL_MOVE) {
            OUT("<%s>: failure - got illegal move [%s] \n", __func__, words[i]);
            return false;
        }

        engine.make_move(move);
    }
    return true;
}

void uci_loop() 
{
    engine.reset();

    while (fgets(line, SIZE(line), stdin)) {

        line[strcspn(line, "\n")] = 0;

        cmd_words = split(line, words, " ", SIZE(words));

        if (!cmd_words)
            continue;
        
        size_t cmd = 0;

        for (; cmd < SIZE(UCI_COMMANDS); ++cmd) {

            if (!strcmp(words[0], UCI_COMMANDS[cmd].str)) {

                OUT("<%s>: success - [%s] \n", __func__, UCI_COMMANDS[cmd].str);

                UCI_COMMANDS[cmd].cb();

                break;
            }
        }

        if (cmd >= SIZE(UCI_COMMANDS))
            OUT("<%s>: failure - [%s] \n", __func__, words[0]);
    }
}

// SECTION GUI to Engine

void uci_uci()
{
    id();
    // all_options();
    uciok();
}

void uci_debug()
{

}

void uci_isready()
{

}

void uci_setoption()
{

}

void uci_register()
{

}

void uci_ucinewgame()
{

}

void uci_position()
{
    if (cmd_words < 2) {
        OUT("<%s>: failure - too few arguments \n", __func__);
        return;
    }

    size_t moves_token = 0;
    
    if (!strcmp(words[1], "startpos")) {

        // position startpos moves e2e4 e7e5

        engine.reset();

        if (cmd_words > 3 && !strcmp(words[2], "moves"))
            moves_token = 3;

    } else if (cmd_words > 7 && !strcmp(words[1], "fen")) {

        // position fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 moves e2e4 e7e5

        for (size_t i = 3; i < 8; ++i)
            *(words[i] - 1) = ' ';

        if (!engine.set(words[2])) {
            OUT("<%s>: failure - incorrect fen \n", __func__);
            return;
        }

        if (cmd_words > 9 && !strcmp(words[8], "moves"))
            moves_token = 9;

    } else {
        OUT("<%s>: failure - too few arguments \n", __func__);
        return;
    }

    if (moves_token)
        parse_moves(moves_token);

    engine.perft(1);
}

void uci_go()
{
    if (cmd_words < 2) {
        OUT("<%s>: failure - too few arguments \n", __func__);
        return;
    }

    Move best_move = NULL_MOVE;
    long depth = 0;

    if (cmd_words > 2 && !strcmp(words[1], "depth")) {

        char *end = NULL;

        if (!str_to_int(words[2], &depth, 10, &end) || depth > MAX_DEPTH)

            OUT("<%s>: failure - illegal depth - [%s] \n", __func__, words[2]);
    } else {
        depth = 6;
    }
    best_move = engine.search(depth);
    
    OUT("<%s>: success - best move [", __func__);
    print_mv(best_move);
    OUT("] \n");
}

void uci_stop()
{

}

void uci_ponderhit()
{

}

void uci_quit()
{
    exit(0);
}

// !SECTION GUI to Engine

// SECTION Engine to GUI

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

// !SECTION Engine to GUI