#ifndef MOVE_LIST_H
#define MOVE_LIST_H

#include "defs.h"
#include "misc.h"

constexpr size_t MAX_MOVES = 412;
constexpr Move NULL_MOVE = 0;

// struct MoveList {
//     constexpr const Move* begin() const { return list; }
//     constexpr const Move* end() const   { return last; }
//     constexpr Move* begin()             { return list; }
//     constexpr Move* end()               { return last; }
//     constexpr size_t size() const       { return last - list; }
//     constexpr void erase(Move *it)      { *it = *--last; }
//     constexpr void save(Move m)         { *last++ = m; }
//     constexpr void save(Square src, Square dst)           
//     { 
//         save(mv(src, dst)); 
//     }
//     constexpr void save(Square src, Square dst, MoveFlag flag)           
//     { 
//         save(mv(src, dst, flag)); 
//     }
// private:
//     Move list[MAX_MOVES];
//     Move *last = list;
// };

struct MoveList {
    constexpr const Move* begin() const { return &list[0]; }
    constexpr const Move* end() const   { return &list[len]; }
    constexpr Move* begin()             { return &list[0]; }
    constexpr Move* end()               { return &list[len];; }
    constexpr size_t size() const       { return len; }
    constexpr void erase(size_t idx)    { list[idx] = list[--len]; }
    constexpr void save(Move m)         { list[len++] = m; }
    constexpr void save(Square src, Square dst)           
    { 
        save(mv(src, dst)); 
    }
    constexpr void save(Square src, Square dst, MoveFlag flag)           
    { 
        save(mv(src, dst, flag)); 
    }
private:
    Move list[MAX_MOVES];
    size_t len = 0;
};

#endif // MOVE_LIST_H