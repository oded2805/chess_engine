#ifndef MAKE_MOVE_H
#define MAKE_MOVE_H
#include "types.h"


void commit_move(move play);
void reverse_move(move play);
move create_move_from_input(int fromR, int fromC, int toR, int toC, color side);


#endif