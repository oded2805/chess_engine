#ifndef MOVE_GEN_H
#define MOVE_GEN_H
#include "types.h"
#include "board.h"
int knight_moves( gamePiece* piece, move legal_moves[], int ind);
int bishop_moves( gamePiece* piece, move legal_moves[], int ind);
int rook_moves(gamePiece* piece, move legal_moves[], int ind);
int white_pawn_moves(gamePiece* piece, move legal_moves[], int ind);
int black_pawn_moves( gamePiece* piece, move legal_moves[], int ind);
int king_moves( gamePiece* piece, move legal_moves[], int ind);
int queen_moves(gamePiece* piece, move legal_moves[], int ind);
int generate_pseudo_possible_moves( gamePiece* army[], int sizeArmy, move legal_moves[], int ind);
int generate_legal_moves(move pseudo_moves[], move legal_moves[], int size);
int add_move(int r, int c, move legal_moves[], int ind, gamePiece* piece, int promotion);
int sign(int x);

#endif