#include "board.h"
#include "move_gen.h"
#include "types.h"
#include "move_validation.h"
#include "make_move.h"
#include <stddef.h>
#include "game_state.h"
#include <stdio.h>
#include <stdlib.h>

int sign(int x)
{
	if(x>0)
		return 1;
	if (x<0)
		return -1;
	return 0;
}





int knight_moves(gamePiece* piece, move legal_moves[], int ind)
{
	int ops[8][2]={{1,2},{-1,2},{-1,-2},{1,-2},{2,1},{2,-1},{-2,1},{-2,-1}};
	for (int i=0; i<8; i++)
	{
		if (valid_move( piece, piece->ind[0]+ops[i][0],piece->ind[1]+ops[i][1]))
			ind=add_move(piece->ind[0]+ops[i][0],piece->ind[1]+ops[i][1], legal_moves, ind, piece, 0);
	}
	return ind;
}

int bishop_moves(gamePiece* piece, move legal_moves[], int ind)
{
	for(int i=1; i<8; i++)
	{
		if(valid_move(piece, piece->ind[0]+i, piece->ind[1]+i))
			ind=add_move( piece->ind[0]+i, piece->ind[1]+i, legal_moves, ind, piece, 0);
		else
			break;
	}
	for(int i=1; i<8; i++)
	{
		if(valid_move(piece, piece->ind[0]+i, piece->ind[1]-i))
			ind=add_move( piece->ind[0]+i, piece->ind[1]-i, legal_moves, ind, piece, 0);
		else
			break;
	}
	for(int i=1; i<8; i++)
	{
		if(valid_move(piece, piece->ind[0]-i, piece->ind[1]-i))
			ind=add_move( piece->ind[0]-i, piece->ind[1]-i, legal_moves, ind, piece, 0);
		else
			break;
	}
	for(int i=1; i<8; i++)
	{
		if(valid_move(piece, piece->ind[0]-i, piece->ind[1]+i))
			ind=add_move( piece->ind[0]-i, piece->ind[1]+i, legal_moves, ind, piece, 0);
		else
			break;
	}
	return ind;



}
int rook_moves( gamePiece* piece, move legal_moves[], int ind)
{
	for(int i=1; i<8; i++)
	{
		if(valid_move(piece, piece->ind[0]+i, piece->ind[1]))
			ind=add_move( piece->ind[0]+i, piece->ind[1], legal_moves, ind, piece, 0);
		else
			break;
	}
	for(int i=1; i<8; i++)
	{
		if(valid_move(piece, piece->ind[0], piece->ind[1]-i))
			ind=add_move( piece->ind[0], piece->ind[1]-i, legal_moves, ind, piece, 0);
		else
			break;
	}
	for(int i=1; i<8; i++)
	{
		if(valid_move(piece, piece->ind[0]-i, piece->ind[1]))
			ind=add_move( piece->ind[0]-i, piece->ind[1], legal_moves, ind, piece, 0);
		else
			break;
	}
	for(int i=1; i<8; i++)
	{
		if(valid_move(piece, piece->ind[0], piece->ind[1]+i))
			ind=add_move(piece->ind[0], piece->ind[1]+i, legal_moves, ind, piece, 0);
		else
			break;
	}
	return ind;

}

int white_pawn_moves( gamePiece* piece, move legal_moves[], int ind)
{
	if (valid_move(piece, piece->ind[0]+1,piece->ind[1]))
	{
		if (piece->ind[0]+1==7)
			ind=add_move(piece->ind[0]+1,piece->ind[1], legal_moves, ind, piece, 1);
		else
			ind=add_move(piece->ind[0]+1,piece->ind[1], legal_moves, ind, piece, 0);
	}
	if (valid_move( piece, piece->ind[0]+2,piece->ind[1]))
		ind=add_move(piece->ind[0]+2,piece->ind[1], legal_moves, ind, piece, 0);
	if (valid_move(piece, piece->ind[0]+1,piece->ind[1]+1))
	{
		if (piece->ind[0]+1==7)
			ind=add_move(piece->ind[0]+1,piece->ind[1]+1, legal_moves, ind, piece, 1);
		else
			ind=add_move(piece->ind[0]+1,piece->ind[1]+1, legal_moves, ind, piece, 0);
	}
	if (valid_move(piece, piece->ind[0]+1,piece->ind[1]-1))
	{
		if (piece->ind[0]+1==7)
			ind=add_move(piece->ind[0]+1,piece->ind[1]-1, legal_moves, ind, piece, 1);
		else
			ind=add_move(piece->ind[0]+1,piece->ind[1]-1, legal_moves, ind, piece, 0);
	}

	return ind;

}
int black_pawn_moves( gamePiece* piece, move legal_moves[], int ind)
{
	if (valid_move( piece, piece->ind[0]-1,piece->ind[1]))
	{
		if (piece->ind[0]-1==0)
			ind=add_move(piece->ind[0]-1,piece->ind[1], legal_moves, ind, piece, 1);
		else
			ind=add_move(piece->ind[0]-1,piece->ind[1], legal_moves, ind, piece, 0);
	}
	if (valid_move(piece, piece->ind[0]-2,piece->ind[1]))
		ind=add_move(piece->ind[0]-2,piece->ind[1], legal_moves, ind, piece, 0);
	if (valid_move(piece, piece->ind[0]-1,piece->ind[1]-1))
	{
		if (piece->ind[0]-1==0)
			ind=add_move(piece->ind[0]-1,piece->ind[1]-1, legal_moves, ind, piece, 1);
		else
			ind=add_move(piece->ind[0]-1,piece->ind[1]-1, legal_moves, ind, piece, 0);
	}
	if (valid_move(piece, piece->ind[0]-1,piece->ind[1]+1))
	{
		if (piece->ind[0]-1==0)
			ind=add_move(piece->ind[0]-1,piece->ind[1]+1, legal_moves, ind, piece, 1);
		else
			ind=add_move(piece->ind[0]-1,piece->ind[1]+1, legal_moves, ind, piece, 0);
	}
	
	return ind;
}

int king_moves(gamePiece* piece, move legal_moves[], int ind)
{
	int ops[8][2]={{0,1},{1,0},{-1,0},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
	for (int i=0; i<8; i++)
	{
		if (valid_move(piece, piece->ind[0]+ops[i][0],piece->ind[1]+ops[i][1]))
			ind=add_move(piece->ind[0]+ops[i][0],piece->ind[1]+ops[i][1], legal_moves, ind, piece, 0);
	}
	if (piece->color==WHITE && game.board[0][4].piece && game.board[0][4].piece->piece==KING)
	{
		if (game.white_castling==1 || game.white_castling==3)
		{
			if (game.board[0][1].state==EMPTY && game.board[0][2].state==EMPTY && game.board[0][3].state==EMPTY && !check_threat(0,1,BLACK) && !check_threat(0,2,BLACK) && 
			!check_threat(0,3,BLACK) && !check_threat(0,4,BLACK)&& game.board[0][0].piece && game.board[0][0].piece->piece==ROOK)
			{
				legal_moves[ind].eaten=NULL;
				legal_moves[ind].play=CASTLE_QUEEN;
				legal_moves[ind].fromR=0;
				legal_moves[ind].fromC=4;
				legal_moves[ind].toR=0;
				legal_moves[ind].toC=2;
				legal_moves[ind].promotionTO = 0;
				legal_moves[ind].preCastlingWhite=game.white_castling;
				legal_moves[ind].preCastlingBlack=game.black_castling;
				legal_moves[ind].prev_el[0]=game.el[0];
				legal_moves[ind].prev_el[1]=game.el[1];
				legal_moves[ind].prev_hash_val=game.position_hash;
				ind++;
			}
		}
		if (game.white_castling==2 || game.white_castling==3)
		{
			if (game.board[0][5].state==EMPTY && game.board[0][6].state==EMPTY && !check_threat(0,5,BLACK) && !check_threat(0,6,BLACK) && 
			!check_threat(0,4,BLACK)&& game.board[0][7].piece && game.board[0][7].piece->piece==ROOK)
			{
				legal_moves[ind].eaten=NULL;
				legal_moves[ind].play=CASTLE_KING;
				legal_moves[ind].fromR=0;
				legal_moves[ind].fromC=4;
				legal_moves[ind].toR=0;
				legal_moves[ind].toC=6;
				legal_moves[ind].preCastlingWhite=game.white_castling;
				legal_moves[ind].preCastlingBlack=game.black_castling;
				legal_moves[ind].promotionTO = 0;
				legal_moves[ind].prev_el[0]=game.el[0];
				legal_moves[ind].prev_el[1]=game.el[1];
				legal_moves[ind].prev_hash_val=game.position_hash;

				ind++;
			}
		}
	}
	if (piece->color==BLACK && game.board[7][4].piece && game.board[7][4].piece->piece==KING)
	{
		if (game.black_castling==1 || game.black_castling==3)
		{
			if (game.board[7][1].state==EMPTY && game.board[7][2].state==EMPTY && game.board[7][3].state==EMPTY && !check_threat(7,1,WHITE) && !check_threat(7,2,WHITE) && 
			!check_threat(7,3,WHITE) && !check_threat(7,4,WHITE)&& game.board[7][0].piece && game.board[7][0].piece->piece==ROOK)
			{
				legal_moves[ind].eaten=NULL;
				legal_moves[ind].play=CASTLE_QUEEN;
				legal_moves[ind].fromR=7;
				legal_moves[ind].fromC=4;
				legal_moves[ind].toR=7;
				legal_moves[ind].toC=2;
				legal_moves[ind].preCastlingWhite=game.white_castling;
				legal_moves[ind].preCastlingBlack=game.black_castling;
				legal_moves[ind].promotionTO = 0;
				legal_moves[ind].prev_el[0]=game.el[0];
				legal_moves[ind].prev_el[1]=game.el[1];
				legal_moves[ind].prev_hash_val=game.position_hash;

				ind++;
			}
		}
		if (game.black_castling==2 || game.black_castling==3)
		{
			if (game.board[7][5].state==EMPTY && game.board[7][6].state==EMPTY && !check_threat(7,5,WHITE) && !check_threat(7,6,WHITE) && 
			!check_threat(7,4,WHITE) && game.board[7][7].piece && game.board[7][7].piece->piece==ROOK)
			{
				legal_moves[ind].eaten=NULL;
				legal_moves[ind].play=CASTLE_KING;
				legal_moves[ind].fromR=7;
				legal_moves[ind].fromC=4;
				legal_moves[ind].toR=7;
				legal_moves[ind].toC=6;
				legal_moves[ind].preCastlingWhite=game.white_castling;
				legal_moves[ind].preCastlingBlack=game.black_castling;
				legal_moves[ind].promotionTO = 0;
				legal_moves[ind].prev_el[0]=game.el[0];
				legal_moves[ind].prev_el[1]=game.el[1];
				legal_moves[ind].prev_hash_val=game.position_hash;
				ind++;
			}
		}
	}
	return ind;
}



int queen_moves(gamePiece* piece, move legal_moves[], int ind)
{
	ind=rook_moves(piece,legal_moves,ind);
	ind=bishop_moves(piece,legal_moves,ind);
	return ind;
}

int generate_pseudo_possible_moves(gamePiece* army[], int sizeArmy, move legal_moves[], int ind)
{
	for (int i=0; i<sizeArmy; i++)
	{
		if (army[i]->alive)
		{
			switch (army[i]->piece)
			{
				case ROOK:
				{
					ind=rook_moves( army[i], legal_moves, ind);
					break;
				}
				case QUEEN:
				{
					ind=queen_moves(army[i], legal_moves, ind);
					break;
				}
				case KING:
				{
					ind=king_moves(army[i], legal_moves, ind);
					break;
				}
				case BISHOP:
				{
					ind=bishop_moves(army[i], legal_moves, ind);
					break;
				}
				case PAWN:
				{
					if (army[i]->color==WHITE)
						ind=white_pawn_moves(army[i], legal_moves, ind);
					else
						ind=black_pawn_moves(army[i], legal_moves, ind);
					break;
				}
				case KNIGHT:
				{
					ind=knight_moves(army[i], legal_moves, ind);
					break;
				}
			}
		}
	}
	return ind;
}


int generate_legal_moves(move pseudo_moves[], move legal_moves[], int size)
{
	
	
	
	int ind=0;
	for (int i=0; i<size; i++)
	{
		color col;
		col=game.board[pseudo_moves[i].fromR][pseudo_moves[i].fromC].piece->color;
		if (pseudo_moves[i].play==EL || game.board[pseudo_moves[i].fromR][pseudo_moves[i].fromC].piece->piece==KING||check_threat( game.kings[col]->ind[0], game.kings[col]->ind[1], !col))
		{
			commit_move(pseudo_moves[i]);
			if (!check_threat(game.kings[col]->ind[0],game.kings[col]->ind[1], !col))
			{
				legal_moves[ind]=pseudo_moves[i];
				ind++;
			}
			reverse_move(pseudo_moves[i]);
			continue;
		}
		int kr=game.kings[col]->ind[0];
		int kc=game.kings[col]->ind[1];
		int pr=pseudo_moves[i].fromR;
		int pc=pseudo_moves[i].fromC;
		int tr=pseudo_moves[i].toR;
		int tc=pseudo_moves[i].toC;


		int aligned=(kr==pr)||(kc==pc)||(ABS(kr-pr)==ABS(kc-pc));
		int dirR=sign(pr-kr);
		int dirC=sign(pc-kc);
		int dirRA=sign(tr-kr);
		int dirCA=sign(tc-kc);
		int alignedA=(kr==tr)||(kc==tc)||(ABS(kr-tr)==ABS(kc-tc));

		if (!aligned || ((dirRA==dirR && dirCA==dirC)&& alignedA))
		{
			legal_moves[ind]=pseudo_moves[i];
			ind++;
			continue;
		}
		int valid=1;
		int r=kr+dirR;
		int c=kc+dirC;
		while(r>=0 && r<8 && c<8 && c>=0)
		{
			if(game.board[r][c].state!=EMPTY)
				break;
			

			r+=dirR;
			c+=dirC;
		}
		if (r!=pr || c!=pc)
		{
			legal_moves[ind]=pseudo_moves[i];
			ind++;
			continue;

		}
		r+=dirR;
		c+=dirC;
		int diagonal=(ABS(kr-pr)==ABS(kc-pc));
		int straight=(kr==pr)||(kc==pc);
		while(r>=0 && r<8 && c<8 && c>=0)
		{
			if(game.board[r][c].state!=EMPTY && diagonal && game.board[r][c].piece->color!=col && (game.board[r][c].piece->piece==QUEEN || game.board[r][c].piece->piece==BISHOP))
			{
				valid=0;
				break;
			}
			else if (game.board[r][c].state!=EMPTY && straight && game.board[r][c].piece->color!=col && (game.board[r][c].piece->piece==QUEEN || game.board[r][c].piece->piece==ROOK))
			{
				valid=0;
				break;
			}
			else if (game.board[r][c].state!=EMPTY)
				break;
			

			r+=dirR;
			c+=dirC;
		}
		if (valid)
		{
			legal_moves[ind]=pseudo_moves[i];
			ind++;
		}
	}
	return ind;
}

int add_move( int r, int c, move legal_moves[], int ind, gamePiece* piece, int promotion)
{

	if (promotion)
	{
		
		for (int i=0; i<4; i++)
		{
			legal_moves[ind].fromR=piece->ind[0];
			legal_moves[ind].fromC=piece->ind[1];
			legal_moves[ind].toR=r;
			legal_moves[ind].toC=c;
			legal_moves[ind].preCastlingBlack=game.black_castling;
			legal_moves[ind].preCastlingWhite=game.white_castling;
			legal_moves[ind].prev_el[0]=game.el[0];
			legal_moves[ind].prev_el[1]=game.el[1];
			legal_moves[ind].prev_hash_val=game.position_hash;
			if (game.board[r][c].state==FULL)
			{
				legal_moves[ind].eaten=game.board[r][c].piece;
				legal_moves[ind].play=PROMOTION_CAPTURE;
			}
			else
			{
				legal_moves[ind].eaten=NULL;
				legal_moves[ind].play=PROMOTION;
			}
			legal_moves[ind].promotionTO=i;
			ind++;
		}
	}
	else
	{
		legal_moves[ind].prev_hash_val=game.position_hash;
		legal_moves[ind].fromR=piece->ind[0];
		legal_moves[ind].fromC=piece->ind[1];
		legal_moves[ind].toR=r;
		legal_moves[ind].toC=c;
		legal_moves[ind].preCastlingBlack=game.black_castling;
		legal_moves[ind].preCastlingWhite=game.white_castling;
		legal_moves[ind].prev_el[0]=game.el[0];
		legal_moves[ind].prev_el[1]=game.el[1];
		legal_moves[ind].promotionTO = 0;
		if (piece->piece==PAWN && r==game.el[0] && c == game.el[1])
		{
    		legal_moves[ind].play = EL;
			if(piece->color==BLACK)
				legal_moves[ind].eaten=game.board[r+1][c].piece;
			else
				legal_moves[ind].eaten=game.board[r-1][c].piece;
		}
		else if (game.board[r][c].state==FULL)
		{
			legal_moves[ind].eaten=game.board[r][c].piece;
			legal_moves[ind].play=CAPTURE;
		}
		else 
		{
			legal_moves[ind].eaten=NULL;
			legal_moves[ind].play=QUIET;
		}
		if (piece->piece==PAWN && !(r==game.el[0] && c == game.el[1]))
		{
			if (piece->color==WHITE)
			{
				if (piece->ind[0]==1 && r==3)
					legal_moves[ind].play=DOUBLE_PAWN;
			}
			else if (piece->color==BLACK)
			{
				if (piece->ind[0]==6 && r==4)
					legal_moves[ind].play=DOUBLE_PAWN;
			}
		}
		ind++;
	}
	return ind;
}