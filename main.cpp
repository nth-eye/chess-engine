#include <cstdio>
#include "bitboard.h"

int main(int, char**) 
{
    Bitboard bb = 0xff00ff00ff00ff00;

    print_bb(bb);
}
