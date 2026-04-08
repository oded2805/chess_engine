#include "types.h"
#include "make_move.h"
#include <stddef.h>
#include "board.h"
#include <stdio.h>
#include <stdio.h>
#include "move_validation.h"
#include "game_state.h"
#include <stdlib.h>
#include "zobrist.h"
#include "game_eval.h"

void commit_move(move play)
{
		if (play.play==ILLEGAL_MOVE)
	{
		printf("illeagle move agame.TTempted\n\n");
		return  ;
	}
	if (play.play==MATE)
	{
		printf("there has been a mate\n\n");
		return ;
	}
	if (play.play==STALEMATE)
	{
		printf("there has been a stalemate\n\n");
		return ;
	}



	if (play.eaten && play.eaten->piece == KING)
	{
    printf("BUG: agame.TTempted king capture\n");
    exit(1);
	}
	if (game.el[0] != -3)
		game.position_hash ^= el_hash_values[game.el[1]];
	
	game.position_hash ^= castling_hash_values[game.black_castling | (game.white_castling << 2)];
	game.position_hash ^= side_to_move_hash_values[game.side_to_move];


	piecep p = game.board[play.fromR][play.fromC].piece->piece;

	if (game.board[play.fromR][play.fromC].piece->color==WHITE)
	{
		game.pst_white_mg -= pst_mg[p][pst_index_white(play.fromR, play.fromC)] + pieces_values[p];
		game.pst_white_eg -= pst_eg[p][pst_index_white(play.fromR, play.fromC)] + pieces_values[p];
		
		game.pst_white_mg += pst_mg[p][pst_index_white(play.toR, play.toC)] + pieces_values[p];
		game.pst_white_eg += pst_eg[p][pst_index_white(play.toR, play.toC)] + pieces_values[p];
		
		if(play.eaten)
		{
			game.pst_black_mg -= pst_mg[play.eaten->piece][pst_index_black(play.eaten->ind[0], play.eaten->ind[1])] + pieces_values[play.eaten->piece];
			game.pst_black_eg -= pst_eg[play.eaten->piece][pst_index_black(play.eaten->ind[0], play.eaten->ind[1])] + pieces_values[play.eaten->piece];
		}
	}
	else 
	{
		game.pst_black_mg -= pst_mg[p][pst_index_black(play.fromR, play.fromC)] + pieces_values[p];
		game.pst_black_eg -= pst_eg[p][pst_index_black(play.fromR, play.fromC)] + pieces_values[p];
		
		game.pst_black_mg += pst_mg[p][pst_index_black(play.toR, play.toC)] + pieces_values[p];
		game.pst_black_eg += pst_eg[p][pst_index_black(play.toR, play.toC)] + pieces_values[p];
		
		if (play.eaten)
		{
			game.pst_white_mg -= pst_mg[play.eaten->piece][pst_index_white(play.eaten->ind[0], play.eaten->ind[1])] + pieces_values[play.eaten->piece];
			game.pst_white_eg -= pst_eg[play.eaten->piece][pst_index_white(play.eaten->ind[0], play.eaten->ind[1])] + pieces_values[play.eaten->piece];
		}
	}


	game.position_hash^=pieces_hash_values[game.board[play.fromR][play.fromC].piece->color][game.board[play.fromR][play.fromC].piece->piece][(8*play.fromR)+play.fromC];
	game.position_hash^=pieces_hash_values[game.board[play.fromR][play.fromC].piece->color][game.board[play.fromR][play.fromC].piece->piece][(8*play.toR)+play.toC];
	switch(play.play)
	{
		case CAPTURE:
		{
			game.position_hash^=pieces_hash_values[play.eaten->color][play.eaten->piece][(8*play.eaten->ind[0])+play.eaten->ind[1]];
			break;
		}
		case CASTLE_KING:
		{
			if (play.toR==0)
			{
				game.position_hash^=pieces_hash_values[game.board[play.fromR][play.fromC].piece->color][ROOK][7];
				game.position_hash^=pieces_hash_values[game.board[play.fromR][play.fromC].piece->color][ROOK][5];
			}
			else
			{
				game.position_hash^=pieces_hash_values[game.board[play.fromR][play.fromC].piece->color][ROOK][63];
				game.position_hash^=pieces_hash_values[game.board[play.fromR][play.fromC].piece->color][ROOK][61];
			}
			break;
		}
		case CASTLE_QUEEN:
		{
			if (play.toR==0)
			{
				game.position_hash^=pieces_hash_values[game.board[play.fromR][play.fromC].piece->color][ROOK][0];
				game.position_hash^=pieces_hash_values[game.board[play.fromR][play.fromC].piece->color][ROOK][3];
			}
			else
			{
				game.position_hash^=pieces_hash_values[game.board[play.fromR][play.fromC].piece->color][ROOK][56];
				game.position_hash^=pieces_hash_values[game.board[play.fromR][play.fromC].piece->color][ROOK][59];
			}
			break;
		}
		case EL:
		{
			game.position_hash^=pieces_hash_values[play.eaten->color][play.eaten->piece][(8*play.eaten->ind[0])+play.eaten->ind[1]];

			break;
		}
		case PROMOTION:
		{
			switch (play.promotionTO)
			{
				case 0:
				{
					game.position_hash^=pieces_hash_values[game.board[play.fromR][play.fromC].piece->color][KNIGHT][(8*play.toR)+play.toC];
					break;
				}
				case 1:
				{
					game.position_hash^=pieces_hash_values[game.board[play.fromR][play.fromC].piece->color][ROOK][(8*play.toR)+play.toC];
					break;
				}
				case 2:
				{
					game.position_hash^=pieces_hash_values[game.board[play.fromR][play.fromC].piece->color][QUEEN][(8*play.toR)+play.toC];
					break;
				}
				case 3:
				{
					game.position_hash^=pieces_hash_values[game.board[play.fromR][play.fromC].piece->color][BISHOP][(8*play.toR)+play.toC];
					break;
				}
			}
			game.position_hash^=pieces_hash_values[game.board[play.fromR][play.fromC].piece->color][game.board[play.fromR][play.fromC].piece->piece][(8*play.toR)+play.toC];
			break;
		}
		case PROMOTION_CAPTURE:
		{
			switch (play.promotionTO)
			{
				case 0:
				{
					game.position_hash^=pieces_hash_values[game.board[play.fromR][play.fromC].piece->color][KNIGHT][(8*play.toR)+play.toC];
					break;
				}
				case 1:
				{
					game.position_hash^=pieces_hash_values[game.board[play.fromR][play.fromC].piece->color][ROOK][(8*play.toR)+play.toC];
					break;
				}
				case 2:
				{
					game.position_hash^=pieces_hash_values[game.board[play.fromR][play.fromC].piece->color][QUEEN][(8*play.toR)+play.toC];
					break;
				}
				case 3:
				{
					game.position_hash^=pieces_hash_values[game.board[play.fromR][play.fromC].piece->color][BISHOP][(8*play.toR)+play.toC];
					break;
				}
			}
			game.position_hash^=pieces_hash_values[game.board[play.fromR][play.fromC].piece->color][game.board[play.fromR][play.fromC].piece->piece][(8*play.toR)+play.toC];
			game.position_hash^=pieces_hash_values[play.eaten->color][play.eaten->piece][(8*play.eaten->ind[0])+play.eaten->ind[1]];
			break;
		}
		default:
			break;
	}









	if (game.board[play.fromR][play.fromC].piece)
		game.pawns[game.board[play.fromR][play.fromC].piece->color]&=~(1ULL<<((play.fromR*8)+play.fromC));
	
	if (game.board[play.fromR][play.fromC].piece->piece==PAWN && play.play!=PROMOTION_CAPTURE && play.play !=PROMOTION)
	{
			game.pawns[game.board[play.fromR][play.fromC].piece->color]&=~(1ULL<<((play.fromR*8)+play.fromC));
			game.pawns[game.board[play.fromR][play.fromC].piece->color]|=(1ULL<<((play.toR*8)+play.toC));

			if (play.toR==game.el[0] && play.toC==game.el[1])
				game.pawns[!game.board[play.fromR][play.fromC].piece->color]&=~(1ULL<<(((play.toR+1-(2*(!game.board[play.fromR][play.fromC].piece->color)))*8 )+play.toC));
	}
	if(play.play==PROMOTION || play.play==PROMOTION_CAPTURE)
		game.pawns[game.board[play.fromR][play.fromC].piece->color]&=~(1ULL<<((play.fromR*8)+play.fromC));
		

	if (play.play==CASTLE_KING)
	{
		game.el[0]=-3;
		game.el[1]=-3;
		if (play.fromR==0)
		{
			game.board[0][6].piece=game.board[0][4].piece;
			game.board[0][6].piece->ind[1]=6;
			game.board[0][6].piece->ind[0]=0;
			game.board[0][5].piece=game.board[0][7].piece;
			game.board[0][5].piece->ind[1]=5;
			game.board[0][5].piece->ind[0]=0;

			game.board[0][4].state=EMPTY;
			game.board[0][5].state=FULL;
			game.board[0][6].state=FULL;
			game.board[0][7].state=EMPTY;
			game.board[0][7].piece=NULL;
			game.board[0][4].piece=NULL;
			game.white_castling=0;
		}
		else{
			game.board[7][6].piece=game.board[7][4].piece;
			game.board[7][6].piece->ind[1]=6;
			game.board[7][6].piece->ind[0]=7;
			game.board[7][5].piece=game.board[7][7].piece;
			game.board[7][5].piece->ind[1]=5;
			game.board[7][5].piece->ind[0]=7;
			game.board[7][4].piece=NULL;
			game.board[7][7].piece=NULL;
			game.board[7][4].state=EMPTY;
			game.board[7][5].state=FULL;
			game.board[7][6].state=FULL;
			game.board[7][7].state=EMPTY;
			game.black_castling=0;
		}
	}
	else if (play.play==CASTLE_QUEEN)
	{
		game.el[0]=-3;
		game.el[1]=-3;
		if (play.fromR==0)
		{
			game.board[0][2].piece=game.board[0][4].piece;
			game.board[0][2].piece->ind[1]=2;
			game.board[0][2].piece->ind[0]=0;

			game.board[0][3].piece=game.board[0][0].piece;
			game.board[0][3].piece->ind[1]=3;
			game.board[0][3].piece->ind[0]=0;

			game.board[0][4].piece=NULL;
			game.board[0][0].piece=NULL;
			game.board[0][4].state=EMPTY;
			game.board[0][2].state=FULL;
			game.board[0][3].state=FULL;
			game.board[0][0].state=EMPTY;
			game.white_castling=0;
		}
		else{
			game.board[7][2].piece=game.board[7][4].piece;
			game.board[7][2].piece->ind[1]=2;
			game.board[7][2].piece->ind[0]=7;

			game.board[7][3].piece=game.board[7][0].piece;
			game.board[7][3].piece->ind[1]=3;
			game.board[7][3].piece->ind[0]=7;

			game.board[7][0].piece=NULL;
			game.board[7][4].piece=NULL;
			game.board[7][4].state=EMPTY;
			game.board[7][2].state=FULL;
			game.board[7][3].state=FULL;
			game.board[7][0].state=EMPTY;
			game.black_castling=0;
		}
	}
	// check how to tub bunch of lines, or do it by hand ):
	else{
		if (game.board[play.fromR][play.fromC].piece->piece==KING)
		{
			if (game.board[play.fromR][play.fromC].piece && game.board[play.fromR][play.fromC].piece->color==WHITE)
				game.white_castling=0;
			else
				game.black_castling=0;
		}

		if (game.board[play.fromR][play.fromC].piece->piece==ROOK)
		{
			if (game.board[play.fromR][play.fromC].piece->color==WHITE && play.fromC==0 && (game.white_castling==1||game.white_castling==3))
				game.white_castling-=1;
			else if (game.board[play.fromR][play.fromC].piece->color==WHITE && play.fromC==7 && (game.white_castling==2||game.white_castling==3))
				game.white_castling-=2;
			else if (game.board[play.fromR][play.fromC].piece->color==BLACK && play.fromC==0 && (game.black_castling==1||game.black_castling==3))
				game.black_castling-=1;
			else if (game.board[play.fromR][play.fromC].piece->color==BLACK && play.fromC==7 && (game.black_castling==2||game.black_castling==3))
				game.black_castling-=2;
		}
		if (play.eaten)
		{
			game.sum_pieces-=pieces_values[play.eaten->piece];
			play.eaten->alive=0;
			game.board[play.toR][play.toC].state=EMPTY;
			if (play.eaten->piece==ROOK)
			{
				if (play.eaten->ind[1]==0 && play.eaten->ind[0]==0 && play.eaten->color==WHITE && (game.white_castling==1 || game.white_castling==3))
					game.white_castling-=1;
				else if (play.eaten->ind[1]==0 && play.eaten->ind[0]==7 && play.eaten->color==BLACK && (game.black_castling==1 || game.black_castling==3))
					game.black_castling-=1;
				else if (play.eaten->ind[1]==7 && play.eaten->ind[0]==0 && play.eaten->color==WHITE && (game.white_castling==2 || game.white_castling==3))
					game.white_castling-=2;
				else if (play.eaten->ind[1]==7 && play.eaten->ind[0]==7 && play.eaten->color==BLACK && (game.black_castling==2 || game.black_castling==3))
					game.black_castling-=2;
			}
		}
		
		game.board[play.toR][play.toC].piece=game.board[play.fromR][play.fromC].piece;
		game.board[play.fromR][play.fromC].state=EMPTY;
		game.board[play.fromR][play.fromC].piece=NULL;
		game.board[play.toR][play.toC].state=FULL;
		game.board[play.toR][play.toC].piece->ind[0]=play.toR;
		game.board[play.toR][play.toC].piece->ind[1]=play.toC;
		if (play.play==PROMOTION || play.play==PROMOTION_CAPTURE)
		{
			switch (play.promotionTO)
			{
				case 0:
				{
					game.board[play.toR][play.toC].piece->piece=KNIGHT;
					break;
				}
				case 1:
				{
					game.board[play.toR][play.toC].piece->piece=ROOK;
					break;
				}
				case 2:
				{
					game.board[play.toR][play.toC].piece->piece=QUEEN;
					break;
				}
				case 3:
				{
					game.board[play.toR][play.toC].piece->piece=BISHOP;
					break;
				}
			}
		}
		if (play.play==DOUBLE_PAWN)
		{
			if (play.toR==3)
			{
				game.el[0]=2;
				game.el[1]=play.toC;
			}
			if (play.toR==4)
			{
				game.el[0]=5;
				game.el[1]=play.toC;
			}
		}
		else
		{
			game.el[0]=-3;
			game.el[1]=-3;
		}
		if (play.play==EL)
		{
			if (play.toR==5)
			{
				game.board[play.toR-1][play.toC].state=EMPTY;
				game.board[play.toR-1][play.toC].piece=NULL;

			}
			if (play.toR==2)
			{
				game.board[play.toR+1][play.toC].state=EMPTY;
				game.board[play.toR+1][play.toC].piece=NULL;
			}
		}
	}
	
	
	
	if (play.play==PROMOTION || play.play==PROMOTION_CAPTURE)
	{
		piecep new_piece = game.board[play.toR][play.toC].piece->piece;
		
		if (game.board[play.toR][play.toC].piece->color == WHITE)
		{
			game.pst_white_mg -= pst_mg[PAWN][pst_index_white(play.toR,play.toC)] + pieces_values[PAWN];
			game.pst_white_eg -= pst_eg[PAWN][pst_index_white(play.toR,play.toC)] + pieces_values[PAWN];
			
			game.pst_white_mg += pst_mg[new_piece][pst_index_white(play.toR,play.toC)] + pieces_values[new_piece];
			game.pst_white_eg += pst_eg[new_piece][pst_index_white(play.toR,play.toC)] + pieces_values[new_piece];
		}
		else 
		{
			game.pst_black_mg -= pst_mg[PAWN][pst_index_black(play.toR,play.toC)] + pieces_values[PAWN];
			game.pst_black_eg -= pst_eg[PAWN][pst_index_black(play.toR,play.toC)] + pieces_values[PAWN];
			
			game.pst_black_mg += pst_mg[new_piece][pst_index_black(play.toR,play.toC)] + pieces_values[new_piece];
			game.pst_black_eg += pst_eg[new_piece][pst_index_black(play.toR,play.toC)] + pieces_values[new_piece];
		}
	}

	if(play.play== CASTLE_KING)
	{
		if (play.fromR==0) // White
		{
			game.pst_white_mg -= pst_mg[ROOK][pst_index_white(0, 7)];
			game.pst_white_eg -= pst_eg[ROOK][pst_index_white(0, 7)];
			game.pst_white_mg += pst_mg[ROOK][pst_index_white(0, 5)];
			game.pst_white_eg += pst_eg[ROOK][pst_index_white(0, 5)];
		}
		else // Black
		{
			game.pst_black_mg -= pst_mg[ROOK][pst_index_black(7, 7)];
			game.pst_black_eg -= pst_eg[ROOK][pst_index_black(7, 7)];
			game.pst_black_mg += pst_mg[ROOK][pst_index_black(7, 5)];
			game.pst_black_eg += pst_eg[ROOK][pst_index_black(7, 5)];
		}
	}
	if(play.play== CASTLE_QUEEN)
	{
		if (play.fromR==0) // White
		{
			game.pst_white_mg -= pst_mg[ROOK][pst_index_white(0, 0)];
			game.pst_white_eg -= pst_eg[ROOK][pst_index_white(0, 0)];
			game.pst_white_mg += pst_mg[ROOK][pst_index_white(0, 3)];
			game.pst_white_eg += pst_eg[ROOK][pst_index_white(0, 3)];
		}
		else // Black
		{
			game.pst_black_mg -= pst_mg[ROOK][pst_index_black(7, 0)];
			game.pst_black_eg -= pst_eg[ROOK][pst_index_black(7, 0)];
			game.pst_black_mg += pst_mg[ROOK][pst_index_black(7, 3)];
			game.pst_black_eg += pst_eg[ROOK][pst_index_black(7, 3)];
		}
	}
	if (game.el[0] != -3)
		game.position_hash ^= el_hash_values[game.el[1]];
	
	game.position_hash ^= castling_hash_values[game.black_castling | (game.white_castling << 2)];
	
	game.side_to_move ^= 1;
	game.position_hash ^= side_to_move_hash_values[game.side_to_move];


}


void reverse_move(move play)
{	
	if (play.eaten)
		game.sum_pieces+=pieces_values[play.eaten->piece];
		
	// 1. Revert Rook PST if the move was a Castle
	if (play.play == CASTLE_KING) 
	{
		if (play.fromR == 0) // White
		{
			game.pst_white_mg -= pst_mg[ROOK][pst_index_white(0, 5)]; 
			game.pst_white_eg -= pst_eg[ROOK][pst_index_white(0, 5)]; 
			game.pst_white_mg += pst_mg[ROOK][pst_index_white(0, 7)]; 
			game.pst_white_eg += pst_eg[ROOK][pst_index_white(0, 7)]; 
		} 
		else // Black
		{
			game.pst_black_mg -= pst_mg[ROOK][pst_index_black(7, 5)];
			game.pst_black_eg -= pst_eg[ROOK][pst_index_black(7, 5)];
			game.pst_black_mg += pst_mg[ROOK][pst_index_black(7, 7)];
			game.pst_black_eg += pst_eg[ROOK][pst_index_black(7, 7)];
		}
	} 
	else if (play.play == CASTLE_QUEEN) 
	{
		if (play.fromR == 0) // White
		{
			game.pst_white_mg -= pst_mg[ROOK][pst_index_white(0, 3)];
			game.pst_white_eg -= pst_eg[ROOK][pst_index_white(0, 3)];
			game.pst_white_mg += pst_mg[ROOK][pst_index_white(0, 0)];
			game.pst_white_eg += pst_eg[ROOK][pst_index_white(0, 0)];
		} 
		else // Black
		{
			game.pst_black_mg -= pst_mg[ROOK][pst_index_black(7, 3)];
			game.pst_black_eg -= pst_eg[ROOK][pst_index_black(7, 3)];
			game.pst_black_mg += pst_mg[ROOK][pst_index_black(7, 0)];
			game.pst_black_eg += pst_eg[ROOK][pst_index_black(7, 0)];
		}
	}

	// 2. Identify leaving/returning piece
	piecep leaving_piece = game.board[play.toR][play.toC].piece->piece;
	piecep returning_piece = leaving_piece;

	if (play.play == PROMOTION || play.play == PROMOTION_CAPTURE) {
		returning_piece = PAWN;
	}

	// 3. Undo piece movement
	if (game.board[play.toR][play.toC].piece->color == WHITE)
	{
		// Add returning piece to origin
		game.pst_white_mg += pst_mg[returning_piece][pst_index_white(play.fromR,play.fromC)] + pieces_values[returning_piece];
		game.pst_white_eg += pst_eg[returning_piece][pst_index_white(play.fromR,play.fromC)] + pieces_values[returning_piece];
		
		// Subtract leaving piece from destination
		game.pst_white_mg -= pst_mg[leaving_piece][pst_index_white(play.toR,play.toC)] + pieces_values[leaving_piece];
		game.pst_white_eg -= pst_eg[leaving_piece][pst_index_white(play.toR,play.toC)] + pieces_values[leaving_piece];

		if (play.eaten)
		{
			game.pst_black_mg += pst_mg[play.eaten->piece][pst_index_black(play.eaten->ind[0],play.eaten->ind[1])] + pieces_values[play.eaten->piece];
			game.pst_black_eg += pst_eg[play.eaten->piece][pst_index_black(play.eaten->ind[0],play.eaten->ind[1])] + pieces_values[play.eaten->piece];
		}
	}
	else // BLACK
	{
		// Add returning piece to origin
		game.pst_black_mg += pst_mg[returning_piece][pst_index_black(play.fromR,play.fromC)] + pieces_values[returning_piece];
		game.pst_black_eg += pst_eg[returning_piece][pst_index_black(play.fromR,play.fromC)] + pieces_values[returning_piece];
		
		// Subtract leaving piece from destination
		game.pst_black_mg -= pst_mg[leaving_piece][pst_index_black(play.toR,play.toC)] + pieces_values[leaving_piece];
		game.pst_black_eg -= pst_eg[leaving_piece][pst_index_black(play.toR,play.toC)] + pieces_values[leaving_piece];

		if (play.eaten)
		{
			game.pst_white_mg += pst_mg[play.eaten->piece][pst_index_white(play.eaten->ind[0],play.eaten->ind[1])] + pieces_values[play.eaten->piece];
			game.pst_white_eg += pst_eg[play.eaten->piece][pst_index_white(play.eaten->ind[0],play.eaten->ind[1])] + pieces_values[play.eaten->piece];
		}
	}

	


	game.side_to_move^=1;
	game.position_hash=play.prev_hash_val;
	game.black_castling=play.preCastlingBlack;
	game.white_castling=play.preCastlingWhite;
	game.el[0]=play.prev_el[0];
	game.el[1]=play.prev_el[1];
	if (game.board[play.toR][play.toC].piece)
		game.pawns[game.board[play.toR][play.toC].piece->color]&=~(1ULL<<((8*play.toR)+play.toC));
	

	if (play.play==CASTLE_KING)
	{
		if (play.fromR==0)
		{
			game.board[0][4].piece=game.board[0][6].piece;
			game.board[0][4].piece->ind[1]=4;
			game.board[0][4].piece->ind[0]=0;
			game.board[0][7].piece=game.board[0][5].piece;
			game.board[0][7].piece->ind[1]=7;
			game.board[0][7].piece->ind[0]=0;
			game.board[0][5].piece=NULL;
			game.board[0][6].piece=NULL;
			game.board[0][4].state=FULL;
			game.board[0][5].state=EMPTY;
			game.board[0][6].state=EMPTY;
			game.board[0][7].state=FULL;
		}
		else{
			game.board[7][4].piece=game.board[7][6].piece;
			game.board[7][4].piece->ind[1]=4;
			game.board[7][4].piece->ind[0]=7;
			game.board[7][7].piece=game.board[7][5].piece;
			game.board[7][7].piece->ind[0]=7;
			game.board[7][7].piece->ind[1]=7;
			game.board[7][5].piece=NULL;
			game.board[7][6].piece=NULL;
			game.board[7][4].state=FULL;
			game.board[7][5].state=EMPTY;
			game.board[7][6].state=EMPTY;
			game.board[7][7].state=FULL;
		}
	}
	else if (play.play==CASTLE_QUEEN)
	{
		if (play.fromR==0)
		{
			game.board[0][4].piece=game.board[0][2].piece;
			game.board[0][4].piece->ind[1]=4;
			game.board[0][4].piece->ind[0]=0;
			game.board[0][0].piece=game.board[0][3].piece;
			game.board[0][0].piece->ind[1]=0;
			game.board[0][0].piece->ind[0]=0;
			game.board[0][1].piece=NULL;
			game.board[0][2].piece=NULL;
			game.board[0][3].piece=NULL;
			game.board[0][0].state=FULL;
			game.board[0][1].state=EMPTY;
			game.board[0][2].state=EMPTY;
			game.board[0][3].state=EMPTY;
			game.board[0][4].state=FULL;
		}
		else{
			game.board[7][4].piece=game.board[7][2].piece;
			game.board[7][4].piece->ind[1]=4;
			game.board[7][4].piece->ind[0]=7;
			game.board[7][0].piece=game.board[7][3].piece;
			game.board[7][0].piece->ind[1]=0;
			game.board[7][0].piece->ind[0]=7;

			game.board[7][1].piece=NULL;
			game.board[7][2].piece=NULL;
			game.board[7][3].piece=NULL;
			game.board[7][0].state=FULL;
			game.board[7][1].state=EMPTY;
			game.board[7][2].state=EMPTY;
			game.board[7][3].state=EMPTY;
			game.board[7][4].state=FULL;
		}
	}



	else
	{
		game.board[play.fromR][play.fromC].piece=game.board[play.toR][play.toC].piece;
		game.board[play.fromR][play.fromC].piece->ind[0]=play.fromR;
		game.board[play.fromR][play.fromC].piece->ind[1]=play.fromC;



		if ((play.play==CAPTURE || play.play==PROMOTION_CAPTURE) && play.eaten)
		{
			play.eaten->alive=1;
			if (play.play!=EL)
			{
				game.board[play.toR][play.toC].piece=play.eaten;
				game.board[play.toR][play.toC].state=FULL;
			}
			if (play.eaten->piece==PAWN && play.play!=EL)
			{
				game.pawns[play.eaten->color]|=(1ULL<<((8*play.toR)+play.toC));
			}
		}
		else if (play.play==EL)
		{
			play.eaten->alive = 1;
			if (play.toR == 5)
			{
				game.board[play.toR-1][play.toC].piece = play.eaten;
				game.board[play.toR-1][play.toC].state = FULL;
				game.pawns[play.eaten->color]|=(1ULL<<(((play.toR-1)*8)+play.toC));
			}
			else
			{
				game.board[play.toR+1][play.toC].piece = play.eaten;
				game.board[play.toR+1][play.toC].state = FULL;
				game.pawns[play.eaten->color]|=(1ULL<<(((play.toR+1)*8)+play.toC));
			}
			game.board[play.toR][play.toC].state=EMPTY;
			game.board[play.toR][play.toC].piece=NULL;
		}
		else
		{
			game.board[play.toR][play.toC].state=EMPTY;
			game.board[play.toR][play.toC].piece=NULL;
		}
		game.board[play.fromR][play.fromC].state=FULL;
		if (play.play==PROMOTION||play.play==PROMOTION_CAPTURE)
			game.board[play.fromR][play.fromC].piece->piece=PAWN;
			
		if (game.board[play.fromR][play.fromC].piece->piece==PAWN)
			game.pawns[game.board[play.fromR][play.fromC].piece->color]|=(1ULL<<((8*play.fromR)+play.fromC));
	}
	

}


move create_move_from_input(int fromR, int fromC, int toR, int toC, color side)
{
	move play={0};
	if (MIN(MIN(toR,toC),MIN(fromR,fromC))<0 || MAX(MAX(toR,toC),MAX(fromR,fromC))>7)
	{
		play.play=ILLEGAL_MOVE;
		return play;	
	}

	else if (game.board[fromR][fromC].piece==NULL || game.board[fromR][fromC].piece->color!=side )
	{
		play.play=ILLEGAL_MOVE;
		return play;
	}
	if (side==WHITE)
	{
		if (game.board[fromR][fromC].piece && game.board[fromR][fromC].piece->piece==KING)
		{
			if (toC==2)
			{
				if (toR==fromR &&(game.white_castling==1 || game.white_castling==3) && game.board[0][1].state==EMPTY && game.board[0][2].state==EMPTY && game.board[0][3].state==EMPTY && !check_threat(0,1,BLACK) && !check_threat(0,2,BLACK) && 
				!check_threat(0,3,BLACK) && !check_threat(0,4,BLACK))
				{
					play.play=CASTLE_QUEEN;
					play.fromR=0;
					play.fromC=4;
					play.toR=0;
					play.toC=2;
					play.preCastlingWhite=game.white_castling;
					play.preCastlingBlack=game.black_castling;
					play.prev_el[0]=game.el[0];
					play.prev_el[1]=game.el[1];
				}
				else
				{
					play.play=ILLEGAL_MOVE;
					return play;
				}
				return play;
			}
			if (toC==6)
			{
				if (toR==fromR && (game.white_castling==2 || game.white_castling==3)&& game.board[0][5].state==EMPTY && game.board[0][6].state==EMPTY && !check_threat(0,5,BLACK) && !check_threat(0,6,BLACK) && 
					!check_threat(0,4,BLACK))
				{
						play.play=CASTLE_KING;
						play.fromR=0;
						play.fromC=4;
						play.toR=0;
						play.toC=6;
						play.preCastlingWhite=game.white_castling;
						play.preCastlingBlack=game.black_castling;
						play.prev_el[0]=game.el[0];
						play.prev_el[1]=game.el[1];
				}
				else
				{
					play.play=ILLEGAL_MOVE;
					return play;
				}
				return play;
			}
		}
	}
	else if (side==BLACK)
	{
		if (game.board[fromR][fromC].piece && game.board[fromR][fromC].piece->piece==KING)
		{
			if (toC==2 )
			{
				if (toR==fromR &&(game.black_castling==1 || game.black_castling==3) && game.board[7][1].state==EMPTY && game.board[7][2].state==EMPTY && game.board[7][3].state==EMPTY && !check_threat(7,1,WHITE) && !check_threat(7,2,WHITE) && 
				!check_threat(7,3,WHITE) && !check_threat(7,4,WHITE))
				{
					play.play=CASTLE_QUEEN;
					play.fromR=7;
					play.fromC=4;
					play.toR=7;
					play.toC=2;
					play.preCastlingWhite=game.white_castling;
					play.preCastlingBlack=game.black_castling;
					play.prev_el[0]=game.el[0];
					play.prev_el[1]=game.el[1];
				}
				else
				{
					play.play=ILLEGAL_MOVE;
					return play;
				}
				return play;
			}
			if (toC==6)
			{
				if (toR==fromR && (game.black_castling==2 || game.black_castling==3)&& game.board[7][5].state==EMPTY && game.board[7][6].state==EMPTY && !check_threat(7,5,WHITE) && !check_threat(7,6,WHITE) && 
					!check_threat(7,4,WHITE))
				{
						play.play=CASTLE_KING;
						play.fromR=7;
						play.fromC=4;
						play.toR=7;
						play.toC=6;
						play.preCastlingWhite=game.white_castling;
						play.preCastlingBlack=game.black_castling;
						play.prev_el[0]=game.el[0];
						play.prev_el[1]=game.el[1];
				}
				else
				{
					play.play=ILLEGAL_MOVE;
					return play;
				}
				return play;
			}
		}
	}

	if (!(game.board[fromR][fromC].piece->piece == KING &&
      fromR == toR &&
      ABS(toC - fromC) == 2))
	{
		if (!valid_move( game.board[fromR][fromC].piece, toR, toC))
		{
			play.play = ILLEGAL_MOVE;
			return play;
		}
	}

	if (game.board[fromR][fromC].piece->piece==PAWN)
	{
		if (toR==game.el[0] && toC==game.el[1] )
		{
			play.play=EL;
			if (side==BLACK)
				play.eaten=game.board[toR+1][toC].piece;
			else
				play.eaten=game.board[toR-1][toC].piece;
		}
		else if ((toR-2)==fromR || (toR+2)==fromR)
		{
			play.play=DOUBLE_PAWN;
			play.eaten=NULL;
		}

		else if (side==BLACK && toR==0)
		{
			int promotion;
			printf("what do you want to promote to?\n");
			scanf("%d", &promotion);
			printf("\n");

			if (toC!=fromC)
			{
				play.play=PROMOTION_CAPTURE;
				play.eaten=game.board[toR][toC].piece;
				play.promotionTO=promotion;
			}
			else
			{
				play.play=PROMOTION;
				play.eaten=NULL;
				play.promotionTO=promotion;
			}
			if (promotion<0 || promotion>3)
				play.play=ILLEGAL_MOVE;
		}
		else if (side==WHITE && toR==7)
		{
			int promotion;
			printf("what do you want to promote to?\n");
			scanf("%d\n", &promotion);
			if (toC!=fromC)
			{
				play.play=PROMOTION_CAPTURE;
				play.eaten=game.board[toR][toC].piece;
				play.promotionTO=promotion;
			}
			else
			{
				play.play=PROMOTION;
				play.eaten=NULL;
				play.promotionTO=promotion;
			}
			if (promotion<0 || promotion>3)
				play.play=ILLEGAL_MOVE;
		}
		else if (game.board[toR][toC].piece!=NULL)
		{
			play.eaten=game.board[toR][toC].piece;
			play.play=CAPTURE;
		}
		else
		{
			play.eaten=NULL;
			play.play=QUIET;
		}
	}
	else if (game.board[toR][toC].piece!=NULL)
	{
		play.eaten=game.board[toR][toC].piece;
		play.play=CAPTURE;
	}
	else
	{
		play.eaten=NULL;
		play.play=QUIET;
	}
	play.fromR=fromR;
	play.toC=toC;
	play.fromC=fromC;
	play.toR=toR;
	play.preCastlingBlack=game.black_castling;
	play.preCastlingWhite=game.white_castling;
	play.prev_el[0]=game.el[0];
	play.prev_el[1]=game.el[1];
	return play;
}


