#include "move_validation.h"
#include "types.h"
#include "game_state.h"
#include "board.h"



int valid_rook_move( gamePiece* piece, int r, int c)
{
	if (r!=piece->ind[0] && c!=piece->ind[1])
		return 0;
	
	if (r==piece->ind[0])
	{
		for (int i = MIN(c,piece->ind[1])+1; i<MAX(c,piece->ind[1]); i++)
		{
			if (game.board[r][i].state!=EMPTY)
				return 0;
		}
	}
	else
	{
		for (int i = MIN(r,piece->ind[0])+1; i<MAX(r,piece->ind[0]); i++)
		{
			if (game.board[i][c].state!=EMPTY)
				return 0;
		}
	}
	return 1;
}


int valid_bishop_move( gamePiece* piece, int r, int c)
{
	if (ABS(r-piece->ind[0])!=ABS(c-piece->ind[1]))
		return 0;
	
	for (int i = 1; i< ABS(r-piece->ind[0]); i++)
	{
			if (game.board[piece->ind[0]+(GREATER(r,piece->ind[0])*i)][piece->ind[1]+(GREATER(c,piece->ind[1])*i)].state!=EMPTY)
				return 0;
	}
	return 1;
}




int valid_knight_move(gamePiece* piece, int r, int c)
{	//use MIN and MAX to simplify expression
	if (((r==piece->ind[0]+2) && (c==piece->ind[1]+1)) ||
	((r==piece->ind[0]+2) && (c==piece->ind[1]-1))||
	((r==piece->ind[0]-2) && (c==piece->ind[1]+1))||
	((r==piece->ind[0]-2) && (c==piece->ind[1]-1))||
	((r==piece->ind[0]+1) && (c==piece->ind[1]-2))||
	((r==piece->ind[0]+1) && (c==piece->ind[1]+2))||
	((r==piece->ind[0]-1) && (c==piece->ind[1]-2))||
	((r==piece->ind[0]-1) && (c==piece->ind[1]+2)))
		return 1;
	return 0;
}



int valid_queen_move( gamePiece* piece, int r, int c)
{
	if (valid_rook_move(piece,r,c)||valid_bishop_move(piece,r,c))
		return 1;
	return 0;
}



int valid_pawn_white_move( gamePiece* piece, int r, int c)
{
	if ( piece->ind[0]==1 && piece->ind[1]==c && r==3 && game.board[r][c].state==EMPTY && game.board[r-1][c].state==EMPTY)
		return 1;
	else if(piece->ind[0]==r-1 && piece->ind[1]==c && game.board[r][c].state==EMPTY)
		return 1;
	else if(piece->ind[0]==r-1 && piece->ind[1]==c-1 && game.board[r][c].state==FULL && game.board[r][c].piece->color!=piece->color)
		return 1;
	else if(piece->ind[0]==r-1 && piece->ind[1]==c+1 && game.board[r][c].state==FULL && game.board[r][c].piece->color!=piece->color)
		return 1;
	else if (piece->ind[0]==r-1 && piece->ind[1]==c+1 && r==game.el[0]&& c==game.el[1])
		return 1;
	else if(piece->ind[0]==r-1 && piece->ind[1]==c-1 && r==game.el[0]&& c==game.el[1])
		return 1;
	return 0;
}

int valid_king_move( gamePiece* piece, int r, int c)
{
	if (MAX(ABS(r-piece->ind[0]),ABS(c-piece->ind[1]))>1)
		return 0;
	if (check_threat(r,c,piece->color^1))
		return 0;
	
	return 1;
}

int valid_move(gamePiece* piece, int r, int c)
{
	if (MIN(r,c)<0 || MAX(r,c)>7 || (game.board[r][c].state==FULL && piece->color==game.board[r][c].piece->color )|| (piece->ind[0]==r && piece->ind[1]==c))
		return 0;
	
	if (game.board[r][c].state == FULL && game.board[r][c].piece->piece == KING)
    	return 0;
	switch (piece->piece)
	{
		case KNIGHT:
		{
			return valid_knight_move(piece,r,c);
		}
		case QUEEN:
		{
			return valid_queen_move(piece,r,c);
		}
		case BISHOP:
		{
			return valid_bishop_move(piece,r,c);
		}
		case ROOK:
		{
			return valid_rook_move(piece,r,c);
		}
		case KING:
		{
			return valid_king_move(piece,r,c);
		}
		case PAWN:
		{
			if (piece->color==BLACK)
				return valid_pawn_black_move(piece,r,c);
			return valid_pawn_white_move(piece,r,c);
		}
	}
	return 0;
}



int valid_pawn_black_move( gamePiece* piece, int r, int c)
{
	if ( piece->ind[0]==6 && piece->ind[1]==c && r==4  && game.board[r][c].state==EMPTY && game.board[r+1][c].state==EMPTY)
		return 1;
	else if(piece->ind[0]==r+1 && piece->ind[1]==c && game.board[r][c].state==EMPTY)
		return 1;
	else if(piece->ind[0]==r+1 && piece->ind[1]==c-1 && game.board[r][c].state==FULL && game.board[r][c].piece->color!=piece->color)
		return 1;
	else if(piece->ind[0]==r+1 && piece->ind[1]==c+1 && game.board[r][c].state==FULL && game.board[r][c].piece->color!=piece->color)
		return 1;
	else if (piece->ind[0]==r+1 && piece->ind[1]==c+1 && r==game.el[0]&& c==game.el[1])
		return 1;
	else if(piece->ind[0]==r+1 && piece->ind[1]==c-1 && r==game.el[0]&& c==game.el[1])
		return 1;
	return 0;
}