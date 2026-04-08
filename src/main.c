#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "types.h"
#include "board.h"
#include "move_gen.h"
#include "make_move.h"
#include "game_eval.h"
#include "search.h"
#include "game_state.h"
#include <string.h>
#include "zobrist.h"

#include <locale.h>
#ifdef _WIN32
    #include <windows.h>
    #define SET_UTF8 system("chcp 65001")
#else
    #define SET_UTF8()
#endif

#define MAX_MOVES 800
#define DEPTH 8

void commit_print(move play)
{
	commit_move(play);
	current_TT_age++;
	print_board();
	printf("\n\n");	
	printf("%d\n\n", eval_position());
}


int check_endgame(int count)
 {
	if (mate(game.armys[game.side_to_move]))
		{
			printf("there has been a mate");
			return 1;
		}
		if (stalemate( game.armys[game.side_to_move]))
		{
			printf("there has been a stalemate");
			return 1;
		}
		if(!enough_material_white() && !enough_material_black())
		{
			printf("there is not enough material. a tie");
			return 1;
		}
		if (count >= MAX_MOVES)
		{
			printf("game long enough. time to go home");
			return 1;
		}
	return 0;

 }

void engine_vs_engine()
{
	int count=0;
	move curr_move;
	while(1)
	{
		if(check_endgame(count))
			break;
		count++;
		curr_move=find_best_move(game.side_to_move,8);
		commit_print(curr_move);
		
	}
}


void player_vs_engine(int starter)
{
	move curr_move;
	int count=0;
	print_board();
	printf("\n\n");	
	if (starter)
	{
		curr_move=find_best_move(game.side_to_move,DEPTH);
		commit_print(curr_move);
	}
	while(1)
	{
		curr_move.play=ILLEGAL_MOVE;
		while(curr_move.play==ILLEGAL_MOVE)
		{
			int fromR, toR, fromC, toC;
			scanf("%d%d%d%d",&fromR, &fromC, &toR, &toC);
			printf("\n");	

			curr_move=create_move_from_input(fromR, fromC, toR, toC,game.side_to_move);
			commit_print(curr_move);
		}
		if (check_endgame(count))
			break;
		count++;
		curr_move=find_best_move(game.side_to_move,8);
		commit_print(curr_move);
		if(check_endgame(count))
			break;
	}
}


void player_vs_player()
{
	move curr_move;
	int count=0;
	print_board();
	printf("\n\n");	
	while(1)
	{
		curr_move.play=ILLEGAL_MOVE;
		while(curr_move.play==ILLEGAL_MOVE)
		{
			int fromR, toR, fromC, toC;
			scanf("%d%d%d%d",&fromR, &fromC, &toR, &toC);
			printf("\n");	
			curr_move=create_move_from_input( fromR, fromC, toR, toC,game.side_to_move);
			commit_print(curr_move);
		}
		check_endgame(count);
		count+=1;
	}
}


int check_allocation()
{
	for (int i=0; i<game.count_soldiers[0]; i++)
		if (!game.armys[0][i]) return 0;
	for (int i=0; i<game.count_soldiers[1]; i++)
		if (!game.armys[1][i]) return 0;
	return 1;
}

void clean_game()
{
	for (int i=0; i<game.count_soldiers[0]; i++) 
	{
		free(game.armys[0][i]);
	}
	for (int i=0; i<game.count_soldiers[1]; i++) 
	{
		free(game.armys[1][i]);
	}
	free(game.TT);
}


int main(int argc, char* argv[])
{
	SET_UTF8;
	game.TT = calloc(TABLE_SIZE, sizeof(TTBucket));
	if (game.TT==NULL)
	{
		printf("couldnt allocate the Transportation tabel");
		return 0;
	}
	initialize_hashing_values();
	
	char* fen;
	if (argc>1)
		fen=argv[1];
	else
		fen=STARTING_POSITION;
	//arrays of flags- in 0 for castling game.queensiode, ind 1 for castling game.kingside, ind2 for active in curent turn
	int white[]={0,0,0};
	int black[]={0,0,0};
	fen_initialization(fen,white,black);
	if (!check_allocation())
	{
		printf("couldnt allocatre the memory for the game armys");
		return 0;
	}
	
	int gameType;
	printf("choose game type. 0 to watch a game of engine vs engine, 1 to play against the engine, 2 to play player vs player\n");
	scanf("%d", &gameType);

	if(gameType==0)
		engine_vs_engine();
	
	if (gameType==1)
	{
		int starter;
		printf("when about to promote press after the indexes the promotion to- 0 for knight, 1 for rook, 2 for game.queen and 3 for bishop \n");
		printf("if you want to start enter 0, else 1\n");
		scanf("%d", &starter);
		player_vs_engine(starter);
	}
	if (gameType==2)
	{
		printf("when about to promote press after the indexes the promotion to- 0 for knight, 1 for rook, 2 for game.queen and 3 for bishop \n");
		player_vs_player();
	}
	
	clean_game();
	
	

	return 0;
}
