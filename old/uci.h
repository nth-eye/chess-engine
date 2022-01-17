#ifndef UCI_H
#define UCI_H

void uci_loop();

void uci_uci();
void uci_debug();
void uci_isready();
void uci_setoption();
void uci_register();
void uci_ucinewgame();
void uci_position();
void uci_go();
void uci_stop();
void uci_ponderhit();
void uci_quit();

void id();
void uciok();
void readyok();

#endif