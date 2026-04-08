#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "types.h"

int check(color c);
int check_everywhere( gamePiece* army[]);
int mate(gamePiece* army[]);
int stalemate( gamePiece* army[]);
int check_threat( int r, int c, color attacking);
int enough_material_black();
int enough_material_white();

#endif