#include "board.h"
#include "types.h"
#include "game_state.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include "zobrist.h"
#include "game_eval.h"


gamePosition game={0};

int pst_index(color col, int r, int c)
{
	if (!col)
		return pst_index_white(r,c);
	return pst_index_black(r,c);
}
void add_pst_points(color col, piecep p, int r, int c)
{
	if (col)
	{
		game.pst_black_mg += pst_mg[p][pst_index(col, r, c)] + pieces_values[p];
		game.pst_black_eg += pst_eg[p][pst_index(col, r,c)] + pieces_values[p];
		game.pst_black+=pieces_values[p];
	}
	else
	{
		game.pst_white_mg += pst_mg[p][pst_index(col, r, c)] + pieces_values[p];
		game.pst_white_eg += pst_eg[p][pst_index(col, r,c)] + pieces_values[p];
		game.pst_white+=pieces_values[p];
	}
}


void add_piece(color col, piecep p, int r, int c, int ind)
{
	game.sum_pieces+=pieces_values[p];
	gamePiece* curr=malloc(sizeof(gamePiece));
	curr->alive=1;
	curr->color=col;
	curr->ind[0]=r;
	curr->ind[1]=c;
	curr->piece=p;
	game.armys[col][ind]=curr;
	game.board[r][c].piece=curr;
	game.board[r][c].state=FULL;
	add_pst_points(col, p, r, c);	
}

//used to mask fules in fron of a piece
void init_FILE_FORWARD_masks() {
    for (int sq = 0; sq < 64; sq++) {
        int file = sq % 8;

        uint64_t file_mask = FILE_A<<file;

        game.FILE_FORWARD[0][sq] =
            file_mask & (~((1ULL << (sq + 1)) - 1));

        game.FILE_FORWARD[1][sq] =
            file_mask & ((1ULL << sq) - 1);
    }
}



//used to check if a pawn is a passed pawn
void init_passed_masks()
{
    for (int sq = 0; sq < 64; sq++)
    {
        int row = sq / 8;
        int col = sq % 8;

        game.passed_mask[WHITE][sq] = 0ULL;
        game.passed_mask[BLACK][sq] = 0ULL;

        for (int r = row + 1; r < 8; r++)
        {
            for (int c = col - 1; c <= col + 1; c++)
            {
                if (IN_BOUNDS(r, c))
                    game.passed_mask[WHITE][sq] |= (1ULL << pst_index_black(r, c));
            }
        }

        for (int r = row - 1; r >= 0; r--)
        {
            for (int c = col - 1; c <= col + 1; c++)
            {
                if (IN_BOUNDS(r, c))
                    game.passed_mask[BLACK][sq] |= (1ULL << pst_index_black(r, c));
            }
        }
    }
}


//used to check if a pawn is isolated pawn
void init_isolated_masks()
{
    for (int sq = 0; sq < 64; sq++)
    {
        int row = sq / 8;
        int col = sq % 8;

        game.isolated_mask[WHITE][sq] = 0ULL;
        game.isolated_mask[BLACK][sq] = 0ULL;

        for (int r = row-1; r < 8; r++)
        {
            for (int c = col - 1; c <= col + 1; c++)
            {
                if (IN_BOUNDS(r, c))
                    game.isolated_mask[WHITE][sq] |= (1ULL << pst_index_black(r, c));
            }
        }

        for (int r = row-1; r >= 0; r--)
        {
            for (int c = col - 1; c <= col + 1; c++)
            {
                if (IN_BOUNDS(r, c))
                    game.isolated_mask[BLACK][sq] |= (1ULL << pst_index_black(r, c));
            }
        }
    }
}



int* fen_initialization(char* fen,int playerW[], int playerB[])
{
	init_FILE_FORWARD_masks();
	init_passed_masks();
	game.pst_white=0;
	game.pst_black=0;
	game.pst_white_mg = 0;
	game.pst_white_eg = 0;
	game.pst_black_mg = 0;
	game.pst_black_eg = 0;
	game.sum_pieces = 0;
	game.el[0]=-3;
	game.el[1]=-3;
	game.position_hash = hash_value_start();
	game.count_soldiers[0]=0;
	game.count_soldiers[1]=0;

	int count, ind=0, w=0, b=0;
	for (int i=7; i>=0; i--)
	{
		count=0;
		while (count<8)
		{
			if (isdigit(fen[ind]))
			{
				for (int j=count; j<count+fen[ind]-'0';j++)
				{
					game.board[i][j].state=EMPTY;
					game.board[i][j].piece = NULL;
				}

				count+=(fen[ind]-'0');
			}				
			else
			{
				switch (fen[ind])
				{
					case 'r':
					{
						add_piece(BLACK, ROOK, i, count, b);
						b++;
						if (game.rooks[1][0])
							game.rooks[1][1]=game.armys[1][b-1];
						else
							game.rooks[1][0]=game.armys[1][b-1];
						break;
					}
					case 'R':
					{
						add_piece(WHITE, ROOK, i, count, w);
						w++;
						if (game.rooks[0][0])
							game.rooks[0][1]=game.armys[0][w-1];
						else
							game.rooks[0][0]=game.armys[0][w-1];
						break;
					}
					case 'b':
					{
						add_piece(BLACK, BISHOP, i, count, b);

						b++;
						if (game.bishops[1][0])
							game.bishops[1][1]=game.armys[1][b-1];
						else
							game.bishops[1][0]=game.armys[1][b-1];
						break;
					}
					case 'B':
					{
						add_piece(WHITE, BISHOP, i, count, w);

						w++;
						if (game.bishops[0][0])
							game.bishops[0][1]=game.armys[0][w-1];
						else
							game.bishops[0][0]=game.armys[0][w-1];
						break;
					}
					case 'n':
					{
						add_piece(BLACK, KNIGHT, i, count, b);
						b++;
						if (game.knights[1][0])
							game.knights[1][1]=game.armys[1][b-1];
						else
							game.knights[1][0]=game.armys[1][b-1];
						break;
					}
					case 'N':
					{
						add_piece(WHITE, KNIGHT, i, count, w);

						w++;
						if (game.knights[0][0])
							game.knights[0][1]=game.armys[0][w-1];
						else
							game.knights[0][0]=game.armys[0][w-1];
						break;
					}
					case 'k':
					{
					
						add_piece(BLACK, KING, i, count, b);
						b++;
						game.black_king=game.armys[1][b-1];
						game.kings[1]=game.armys[1][b-1];
						break;
					}
					case 'K':
					{
						add_piece(WHITE, KING, i, count, w);

						game.white_king=game.armys[0][w];
						game.kings[0]=game.armys[0][w];
						w++;
						break;
					}
					case 'q':
					{
						add_piece(BLACK, QUEEN, i, count, b);

						b++;
						game.queen[1]=game.armys[1][b-1];
						break;
					}
					case 'Q':
					{
						add_piece(WHITE, QUEEN, i, count, w);

						w++;
						game.queen[0]=game.armys[0][w-1];
						break;
					}
					case 'p':
					{
						add_piece(BLACK, PAWN, i, count, b);

						b++;
						game.pawns[1] |= (1ULL << ((i*8)+count));
						break;
					}
					case 'P':
					{
						add_piece(WHITE, PAWN, i, count, w);
						w++;
						game.pawns[0] |= (1ULL << ((i*8)+count));
						break;
					}
				}
				count++;
			}
			ind+=1;
		}
		ind+=1;
	}	
	if (fen[ind]=='w')
		playerW[2]=1;
	else
		playerB[2]=1;
	ind+=2;
	if (fen[ind]=='K')
	{
		playerW[1]=1;
		ind+=1;
		if (fen[ind]=='Q')
		{
			playerW[0]=1;
			ind+=1;
		}
	}
	if (fen[ind]=='k')
	{
		playerB[1]=1;
		ind+=1;
		if (fen[ind]=='q')
		{
			playerB[0]=1;
			ind+=1;
		}
	}
	if (fen[ind]!='\0' && fen[++ind]!='\0' && isalpha(fen[ind]))
	{
		int row=fen[ind+1]-'1';
		int col=fen[ind]-'a';
		game.el[0]=row;
		game.el[1]=col;
	}
	game.count_soldiers[0]=w;
	game.count_soldiers[1]=b;

	for (int i=0; i<8; i++)
	{
		for (int j=0; j<8; j++)
		{
			if (game.board[i][j].state==EMPTY)
				game.board[i][j].piece=NULL;
		}
	}
	game.start_sum_pieces=game.sum_pieces;
	game.black_castling=0;
	game.white_castling=0;
	game.side_to_move=(playerW[2]^1);
	if(playerW[0])
		game.white_castling+=1;
	if(playerW[1])
		game.white_castling+=2;
	if(playerB[0])
		game.black_castling+=1;
	if(playerB[1])
		game.black_castling+=2;
	return game.count_soldiers;
}

void print_board()
{
	for (int i=7; i>=0; i--)
	{
		printf("%d  ", i);
		for (int j=0; j<=7; j++)
		{
			if (game.board[i][j].state==EMPTY)
				printf(".  ");
			else{
				gamePiece p=*game.board[i][j].piece;
				switch (p.piece)
				{
					case PAWN:
					{
						if (p.color==WHITE)
							printf(PRINT_WHITE_PAWN);
						else
							printf(PRINT_BLACK_PAWN);
					break;
					}
					case KNIGHT:
					{
						if (p.color==WHITE)
							printf(PRINT_WHITE_KNIGHT);
						else
							printf(PRINT_BLACK_KNIGHT);
					break;
					}
					case BISHOP:
					{
						if (p.color==WHITE)
							printf(PRINT_WHITE_BISHOP);
						else
							printf(PRINT_BLACK_BISHOP);
					break;
					}
					case QUEEN:
					{
						if (p.color==WHITE)
							printf(PRINT_WHITE_QUEEN);
						else
							printf(PRINT_BLACK_QUEEN);
						break;
					}
					case ROOK:
					{
						if (p.color==WHITE)
							printf(PRINT_WHITE_ROOK);
						else
							printf(PRINT_BLACK_ROOK);
						break;
					}
					case KING:
					{
						if (p.color==WHITE)
							printf(PRINT_WHITE_KING);
						else
							printf(PRINT_BLACK_KING);
						break;
					}
				}
			}
		}
		printf("\n\n");
	}
	printf("c  ");
	for (int i=0; i<8; i++)
	{
		printf("%d  ", i);
	}
}