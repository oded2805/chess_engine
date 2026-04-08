#ifndef ZOBRIST_H
#define ZOBRIST_H
#include <stdint.h>


extern uint64_t pieces_hash_values[2][6][64];
extern uint64_t side_to_move_hash_values[2];
extern uint64_t castling_hash_values[16];
extern uint64_t el_hash_values[8];

void initialize_hashing_values();
uint64_t hash_value_start();
uint64_t xorshift64();






#endif