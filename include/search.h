#ifndef SEARCH_H
#define SEARCH_H
#include "types.h"
int fast_eval(move m);

int same_move(move a, move b);
int move_ordering(move options[], int size, move ordered[], color col);
int search_best_score(int depth, color col, int alpha, int beta);
move find_best_move(color col,int depth);
void ordering(move opt[], int size);
void move_to_entry( TTEntry *TT, int ind, move* play, int flag, int depth, int score, uint64_t key);
move recover_move(TTEntry en);
extern uint8_t current_TT_age;
int Quiesce( int alpha, int beta, int depth, color col);


#endif