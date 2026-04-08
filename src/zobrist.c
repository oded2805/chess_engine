#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include "board.h"
#include <math.h>






uint64_t pieces_hash_values[2][6][64];
uint64_t side_to_move_hash_values[2];
uint64_t castling_hash_values[16];
uint64_t el_hash_values[8];

uint64_t g_seed = 123456789ULL; 

uint64_t xorshift64() {
    g_seed ^= g_seed << 13;
    g_seed ^= g_seed >> 7;
    g_seed ^= g_seed << 17;
    return g_seed;
}


void initialize_hashing_values()
{
    for (int i=0; i<2; i++)
    {
        side_to_move_hash_values[i]=xorshift64();
        for (int j=0; j<6; j++)
        {
            for (int k=0; k<64; k++)
                pieces_hash_values[i][j][k]=xorshift64();
        }
    }
    for (int i=0; i<16; i++)
        castling_hash_values[i]=xorshift64();
    for (int i=0; i<8; i++)
        el_hash_values[i]=xorshift64();
    
}


uint64_t hash_value_start()
{
    uint64_t hash_score=0;
    for (int i=0; i<8; i++)
    {
        for (int j=0; j<8; j++)
        {
            if (game.board[i][j].piece)
                hash_score^=pieces_hash_values[game.board[i][j].piece->color][game.board[i][j].piece->piece][(8*i)+j];
        }
    }
    hash_score^=side_to_move_hash_values[game.side_to_move];
    hash_score^=castling_hash_values[game.black_castling | (game.white_castling << 2)];
    if (game.el[0]!=-3)
        hash_score^=el_hash_values[game.el[1]];

    return hash_score;
}