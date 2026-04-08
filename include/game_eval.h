#ifndef GAME_EVAL_H
#define GAME_EVAL_H
#include "types.h"



#define PAWN_VALUE 100
#define KNIGHT_VALUE 320
#define BISHOP_VALUE 330
#define ROOK_VALUE 500
#define QUEEN_VALUE 900
#define KING_VALUE 20000
#define pst_index_black(r,c) ((8*r)+(c))
#define pst_index_white(r,c) (((8*r)+(c))^56)
#define PASSED_PAWN_VAL (50)
#define ISOLATED_PAWN_VAL (-20)
#define DOUBLED_PAWNS (-20)
#define CONNECTED_PAWNS (20)
#define KNIGHT_MOBILITY 4
#define QUEEN_MOBILITY 1
#define ROOK_MOBILITY 2
#define BISHOP_MOBILITY 5
#define SUM_VALUES KNIGHT_VALUE+BISHOP_VALUE+QUEEN_VALUE+ROOK_VALUE
#define pst_index_black(r,c) ((8*r)+(c))
#define pst_index_white(r,c) (((8*r)+(c))^56)
#define PASSED_PAWN_VAL (50)
#define ISOLATED_PAWN_VAL (-20)
#define DOUBLED_PAWNS (-20)
#define CONNECTED_PAWNS (20)
#define KNIGHT_MOBILITY 4
#define QUEEN_MOBILITY 1
#define ROOK_MOBILITY 2
#define BISHOP_MOBILITY 5
#define KING_PAWNS 25



#define FILE_A 0x0101010101010101ULL
#define FILE_B (FILE_A << 1)
#define FILE_C (FILE_A << 2)
#define FILE_D (FILE_A << 3)
#define FILE_E (FILE_A << 4)
#define FILE_F (FILE_A << 5)
#define FILE_G (FILE_A << 6)
#define FILE_H (FILE_A << 7)

#define ROW_A 0x00000000000000FFULL
#define ROW_B (ROW_A << 1)
#define ROW_C (ROW_A << 2)
#define ROW_D (ROW_A << 3)
#define ROW_E (ROW_A << 4)
#define ROW_F (ROW_A << 5)
#define ROW_G (ROW_A << 6)
#define ROW_H (ROW_A << 7)



extern int pieces_values[6];
extern int capture_values[6];
extern short *pst_mg[6];
extern short *pst_eg[6];

int eval_position();
int passed_pawn_check(int r, int c , color col);
int passed_isolated_pawns(color col);
int eval_pawns();
int doubled_pawns(color col);
int mobility();
int bishop_eval();
int rook_eval();
int friendly_pawn_on_file(int file, color col);
int center_bonus();
int king_shield();
int pawn_shield(color col);
int pawn_shield_black();
int pawn_shield_white();



#endif