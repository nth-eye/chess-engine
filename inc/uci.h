#ifndef UCI_H
#define UCI_H

#include "engine.h"

struct UCI {

    void loop();
    Move move(const char *c);

    Engine engine;
};

void quit();

void id();
void uciok();
void readyok();

#endif // UCI_H