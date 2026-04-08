#include "game_state.h"
#include "types.h"
#include "board.h"
#include <stdlib.h>
#include "move_gen.h"
#include "move_validation.h"


int enough_material_white()
{
	int alive[6]={0};
	for (int i=0; i<game.count_soldiers[0]; i++)
	{
		if(!game.armys[0][i]->alive)
			continue;
		alive[game.armys[0][i]->piece]+=1;
	}
	if (alive[QUEEN]||alive[ROOK]||alive[PAWN]||(alive[KNIGHT] && alive [BISHOP])|| alive[BISHOP]==2 || alive[KNIGHT]==2)
		return 1;
	return 0;
}

int enough_material_black()
{
	
	int alive[6]={0};
	for (int i=0; i<game.count_soldiers[1]; i++)
	{
		if(!game.armys[1][i]->alive)
			continue;
		alive[game.armys[1][i]->piece]+=1;
	}
	if (alive[QUEEN]||alive[ROOK]||alive[PAWN]||(alive[KNIGHT] && alive [BISHOP])|| alive[BISHOP]==2 || alive[KNIGHT]==2)
		return 1;
	return 0;
}



int check(color c)
{
	if (c==WHITE)
		return check_threat(game.white_king->ind[0],game.white_king->ind[1],BLACK);
	return check_threat(game.black_king->ind[0],game.black_king->ind[1],WHITE);
}


int check_everywhere( gamePiece* army[])
{
	move pseudo_moves[330];
	int ind_pseudo=0;
	move legal_moves[330];
	int ind_legal=0;
	int sizeArmy= army[0]->color==BLACK ? game.count_soldiers[1] : game.count_soldiers[0];
	ind_pseudo=generate_pseudo_possible_moves(army, sizeArmy, pseudo_moves, ind_pseudo);
	ind_legal = generate_legal_moves(pseudo_moves, legal_moves, ind_pseudo);
	if (!ind_legal)
	{
		return 1;
	}
	return 0;
}

int mate(gamePiece* army[])
{
	if (army[0]->color==BLACK)
	{
		if (!check( BLACK))
			return 0;
	}
	if (army[0]->color==WHITE)
	{
		if (!check( WHITE))
			return 0;
	}
	return check_everywhere( army);
}


int stalemate( gamePiece* army[])
{
	if (army[0]->color==BLACK)
		return check_everywhere(army)&& !check_threat( game.black_king->ind[0], game.black_king->ind[1], WHITE );
	return check_everywhere(army)&& !check_threat( game.white_king->ind[0], game.white_king->ind[1], BLACK);
}



int check_threat( int r, int c, color attacking)
{
	
	for (int i=r+1; i< 8; i++)
	{
		if (game.board[i][c].state==FULL && game.board[i][c].piece->color==attacking && 
			( game.board[i][c].piece->piece==QUEEN || game.board[i][c].piece->piece==ROOK))
			return 1;
		else if (game.board[i][c].state==FULL)
			break;
	}
	for (int i=r-1; i>=0; i--)
	{
		if (game.board[i][c].state==FULL && game.board[i][c].piece->color==attacking && 
			( game.board[i][c].piece->piece==QUEEN || game.board[i][c].piece->piece==ROOK))
			return 1;
		else if (game.board[i][c].state==FULL)
			break;
	}
	for (int i=c+1; i< 8; i++)
	{
		if (game.board[r][i].state==FULL && game.board[r][i].piece->color==attacking && 
			( game.board[r][i].piece->piece==QUEEN ||  game.board[r][i].piece->piece==ROOK))
			return 1;
		else if (game.board[r][i].state==FULL)
			break;
	}
	for (int i=c-1; i>=0; i--)
	{
		if (game.board[r][i].state==FULL && game.board[r][i].piece->color==attacking && 
			( game.board[r][i].piece->piece==QUEEN ||  game.board[r][i].piece->piece==ROOK))
			return 1;
		else if (game.board[r][i].state==FULL)
			break;
	}
	for (int i=1; i<8 && MAX(r+i, c+i)<8 ;i++)
	{
		if (game.board[r+i][c+i].state==FULL && game.board[r+i][c+i].piece->color==attacking && 
			( game.board[r+i][c+i].piece->piece==QUEEN ||  game.board[r+i][c+i].piece->piece==BISHOP))
			return 1;
		else if (game.board[r+i][c+i].state==FULL)
			break;
	}
	for (int i=1; i<8 && r+i<8 && c-i>=0;i++)
	{
		if (game.board[r+i][c-i].state==FULL && game.board[r+i][c-i].piece->color==attacking && 
			( game.board[r+i][c-i].piece->piece==QUEEN ||  game.board[r+i][c-i].piece->piece==BISHOP))
			return 1;
		else if (game.board[r+i][c-i].state==FULL)
			break;
	}
	for (int i=1; i<8 && MIN(r-i,c-i)>=0 ;i++)
	{
		if (game.board[r-i][c-i].state==FULL && game.board[r-i][c-i].piece->color==attacking && 
			( game.board[r-i][c-i].piece->piece==QUEEN ||  game.board[r-i][c-i].piece->piece==BISHOP))
			return 1;
		else if (game.board[r-i][c-i].state==FULL)
			break;
	}
	for (int i=1; i<8 &&r-i>=0 &&c+i<8;i++)
	{
		if (game.board[r-i][c+i].state==FULL && game.board[r-i][c+i].piece->color==attacking && 
			( game.board[r-i][c+i].piece->piece==QUEEN ||  game.board[r-i][c+i].piece->piece==BISHOP))
			return 1;
		else if (game.board[r-i][c+i].state==FULL)
			break;
	}
	if(attacking==WHITE)
	{
		if (IN_BOUNDS(r-1,c-1) && game.board[r-1][c-1].state==FULL && game.board[r-1][c-1].piece->piece==PAWN && game.board[r-1][c-1].piece->color==WHITE)
			return 1;
		if (IN_BOUNDS(r-1,c+1) && game.board[r-1][c+1].state==FULL && game.board[r-1][c+1].piece->piece==PAWN && game.board[r-1][c+1].piece->color==WHITE)
			return 1;
	}
	else		
	{
		if (IN_BOUNDS(r+1,c-1) && game.board[r+1][c-1].state==FULL && game.board[r+1][c-1].piece->piece==PAWN && game.board[r+1][c-1].piece->color==BLACK)
			return 1;
		if (IN_BOUNDS(r+1,c+1) && game.board[r+1][c+1].state==FULL && game.board[r+1][c+1].piece->piece==PAWN && game.board[r+1][c+1].piece->color==BLACK)
			return 1;
	}
	if(game.knights[attacking][0] && game.knights[attacking][0]->alive && (ABS(game.knights[attacking][0]->ind[0]-r)==2 && ABS(game.knights[attacking][0]->ind[1]-c)==1))
		return 1;
	if(game.knights[attacking][1] && game.knights[attacking][1]->alive && (ABS(game.knights[attacking][1]->ind[0]-r)==2 && ABS(game.knights[attacking][1]->ind[1]-c)==1))
		return 1;
	if(game.knights[attacking][0] && game.knights[attacking][0]->alive && (ABS(game.knights[attacking][0]->ind[0]-r)==1 && ABS(game.knights[attacking][0]->ind[1]-c)==2))
		return 1;
	if(game.knights[attacking][1] && game.knights[attacking][1]->alive && (ABS(game.knights[attacking][1]->ind[0]-r)==1 && ABS(game.knights[attacking][1]->ind[1]-c)==2))
		return 1;
	if (game.kings[attacking] &&
    game.kings[attacking]->alive &&
    MAX(ABS(game.kings[attacking]->ind[0] - r), ABS(game.kings[attacking]->ind[1] - c)) == 1)
    	return 1;
	
	return 0;
}
