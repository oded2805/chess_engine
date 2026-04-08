#ifndef TYPES_H
#define TYPES_H
#include <stdint.h>
#define PRINT_WHITE_KING "\u2654  "
#define PRINT_WHITE_QUEEN "\u2655  "
#define PRINT_WHITE_ROOK "\u2656  "
#define PRINT_WHITE_BISHOP "\u2657  "
#define PRINT_WHITE_KNIGHT "\u2658  "
#define PRINT_WHITE_PAWN "\u2659  "
#define PRINT_BLACK_KING "\u265A  "
#define PRINT_BLACK_QUEEN "\u265B  "
#define PRINT_BLACK_ROOK "\u265C  "
#define PRINT_BLACK_BISHOP "\u265D  "
#define PRINT_BLACK_KNIGHT "\u265E  "
#define PRINT_BLACK_PAWN "\u265F  "
#define STARTING_POSITION "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -"
#define MIN(a,b) (((a)<=(b)) ? (a) : (b))
#define MAX(a,b) (((a)<=(b)) ? (b) : (a))
#define ABS(a) (((a)>=0) ? (a):-(a))
#define GREATER(a,b) (((a)>=(b)) ? 1: -1)
#define COLOR(c) ((c)==(BLACK)) ? black : white
#define TABLE_SIZE 33554432




typedef struct cube cube;
typedef struct gamePiece gamePiece;
typedef struct move move;
typedef struct gamePosition gamePosition;
typedef struct move_option move_option;
typedef struct TTEntry TTEntry;
typedef struct TTBucket TTBucket;
typedef uint64_t uint64;






enum moveType{
	QUIET,
	CAPTURE,
	DOUBLE_PAWN,
	EL,
	CASTLE_KING,
	CASTLE_QUEEN,
	PROMOTION,
	PROMOTION_CAPTURE,
	ILLEGAL_MOVE,
	MATE,
	STALEMATE
};


enum piecep{
	PAWN,
	ROOK,
	KNIGHT,
	BISHOP,
	QUEEN,
	KING
};

enum color{
	WHITE,
	BLACK
};

enum state{
	EMPTY,
	FULL
};


typedef enum moveType moveType;
typedef enum state state;
typedef enum piecep piecep;
typedef enum color color;

struct gamePiece{
	piecep piece;
	int8_t ind[2];
	color color;
	unsigned int alive :1;
};

struct cube{
	gamePiece* piece;
	state state;
};
//add moved 
struct move{
	moveType play;
	unsigned int fromR       :3;
	unsigned int fromC       :3;
	unsigned int toR         :3;
	unsigned int toC         :3;
	unsigned int promotionTO :2;
	unsigned int preCastlingWhite :2;
	unsigned int preCastlingBlack :2;
	unsigned int prev_game_phase :1;
	uint64_t prev_hash_val;
	gamePiece* eaten;
	gamePiece* moved;
	int prev_el[2];
	int score;
};




struct move_option{
	move op;
	int val;
};


struct TTEntry{
    uint64_t key;
    int depth;
    int score;
    unsigned int fromR :3;
	unsigned int fromC :3;
	unsigned int toR :3;
	unsigned int toC :3;
	unsigned int promotionTo :2;
    unsigned int flag :2;
	int8_t age;
};



struct TTBucket{
	TTEntry slot[2];
};



struct gamePosition{
	unsigned int side_to_move :1;
	unsigned int white_castling :2;
	unsigned int black_castling :2;
	int pst_white_mg;
	int pst_white_eg;
	int pst_black_mg;
	int pst_black_eg;
	double sum_pieces;
	uint64_t position_hash;
	double pst_white;
	double pst_black;
	int el[2];
	double start_sum_pieces;
 	gamePiece* white_king;
 	gamePiece* black_king;
 	int count_soldiers[2];
 	gamePiece* armys[2][16];
	cube board[8][8];
	uint64_t pawns[2];
	gamePiece* bishops[2][2];
	gamePiece* rooks[2][2];
	gamePiece* knights[2][2];
	gamePiece* queen[2];
	gamePiece* kings[2];
	TTBucket *TT;
	uint64 FILE_FORWARD[2][64];
	uint64 passed_mask[2][64];
	uint64 isolated_mask[2][64];
};


#endif