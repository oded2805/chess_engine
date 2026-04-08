#ifndef board_H
#define board_H
#include "types.h"
#include <stdint.h>
#define IN_BOUNDS(r,c) (r) >= 0 &&  (r) < 8 && (c) >= 0 && (c) < 8



extern gamePosition game;



void add_piece(color col, piecep p, int r, int c, int ind);
void add_pst_points(color col, piecep p, int r, int c);
int pst_index(color col, int r, int c);
void init_isolated_masks();
void init_passed_masks();
void init_FILE_FORWARD_masks();
int* fen_initialization(char* fen,int playerW[], int playerB[]);
void print_board();
#endif