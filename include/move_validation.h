#ifndef MOVE_VALIDATION_H
#define MOVE_VALIDATION_H
#include "types.h"

int valid_rook_move(gamePiece* piece, int r, int c);
int valid_bishop_move( gamePiece* piece, int r, int c);
int valid_knight_move(gamePiece* piece, int r, int c);
int valid_queen_move(gamePiece* piece, int r, int c);
int valid_pawn_white_move(gamePiece* piece, int r, int c);
int valid_king_move(gamePiece* piece, int r, int c);
int valid_move(gamePiece* piece, int r, int c);
int valid_pawn_black_move(gamePiece* piece, int r, int c);





#endif