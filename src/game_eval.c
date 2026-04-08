#include "board.h"
#include "game_eval.h"
#include <stdint.h>
#include "move_gen.h"
#include "types.h"





int capture_values[]={PAWN_VALUE,ROOK_VALUE,KNIGHT_VALUE,BISHOP_VALUE, QUEEN_VALUE, KING_VALUE};


short pawn_pst_mg[]={ 
     0,   0,   0,   0,   0,   0,  0,   0,
     98, 134,  61,  95,  68, 126, 34, -11,
     -6,   7,  26,  31,  65,  56, 25, -20,
    -14,  13,   6,  21,  23,  12, 17, -23,
    -27,  -2,  -5,  12,  17,   6, 10, -25,
    -26,  -4,  -4, -10,   3,   3, 33, -12,
    -35,  -1, -20, -23, -15,  24, 38, -22,
      0,   0,   0,   0,   0,   0,  0,   0       
    };

short knight_pst_mg[]={
    -167, -89, -34, -49,  61, -97, -15, -107,
     -73, -41,  72,  36,  23,  62,   7,  -17,
     -47,  60,  37,  65,  84, 129,  73,   44,
      -9,  17,  19,  53,  37,  69,  18,   22,
     -13,   4,  16,  13,  28,  19,  21,   -8,
     -23,  -9,  12,  10,  19,  17,  25,  -16,
     -29, -53, -12,  -3,  -1,  18, -14,  -19,
    -105, -21, -58, -33, -17, -28, -19,  -23
    };

short bishop_pst_mg[]={
    -29,   4, -82, -37, -25, -42,   7,  -8,
    -26,  16, -18, -13,  30,  59,  18, -47,
    -16,  37,  43,  40,  35,  50,  37,  -2,
     -4,   5,  19,  50,  37,  37,   7,  -2,
     -6,  13,  13,  26,  34,  12,  10,   4,
      0,  15,  15,  15,  14,  27,  18,  10,
      4,  15,  16,   0,   7,  21,  33,   1,
    -33,  -3, -14, -21, -13, -12, -39, -21
};

short rook_pst_mg[]={
    32,  42,  32,  51, 63,  9,  31,  43,
     27,  32,  58,  62, 80, 67,  26,  44,
     -5,  19,  26,  36, 17, 45,  61,  16,
    -24, -11,   7,  26, 24, 35,  -8, -20,
    -36, -26, -12,  -1,  9, -7,   6, -23,
    -45, -25, -16, -17,  3,  0,  -5, -33,
    -44, -16, -20,  -9, -1, 11,  -6, -71,
    -19, -13,   1,  17, 16,  7, -37, -26
};

short queen_pst_mg[]={
    -28,   0,  29,  12,  59,  44,  43,  45,
    -24, -39,  -5,   1, -16,  57,  28,  54,
    -13, -17,   7,   8,  29,  56,  47,  57,
    -27, -27, -16, -16,  -1,  17,  -2,   1,
     -9, -26,  -9, -10,  -2,  -4,   3,  -3,
    -14,   2, -11,  -2,  -5,   2,  14,   5,
    -35,  -8,  11,   2,   8,  15,  -3,   1,
     -1, -18,  -9,  10, -15, -25, -31, -50
};

short king_pst_mg[]={
    -65,  23,  16, -15, -56, -34,   2,  13,
     29,  -1, -20,  -7,  -8,  -4, -38, -29,
     -9,  24,   2, -16, -20,   6,  22, -22,
    -17, -20, -12, -27, -30, -25, -14, -36,
    -49,  -1, -27, -39, -46, -44, -33, -51,
    -14, -14, -22, -46, -44, -30, -15, -27,
      1,   7,  -8, -64, -43, -16,   9,   8,
    -15,  36,  12, -54,   8, -28,  24,  14
};



short pawn_pst_eg[]={
     0,   0,   0,   0,   0,   0,   0,   0,
    178, 173, 158, 134, 147, 132, 165, 187,
     94, 100,  85,  67,  56,  53,  82,  84,
     32,  24,  13,   5,  -2,   4,  17,  17,
     13,   9,  -3,  -7,  -7,  -8,   3,  -1,
      4,   7,  -6,   1,   0,  -5,  -1,  -8,
     13,   8,   8,  10,  13,   0,   2,  -7,
      0,   0,   0,   0,   0,   0,   0,   0
    };

short knight_pst_eg[]={
    -58, -38, -13, -28, -31, -27, -63, -99,
    -25,  -8, -25,  -2,  -9, -25, -24, -52,
    -24, -20,  10,   9,  -1,  -9, -19, -41,
    -17,   3,  22,  22,  22,  11,   8, -18,
    -18,  -6,  16,  25,  16,  17,   4, -18,
    -23,  -3,  -1,  15,  10,  -3, -20, -22,
    -42, -20, -10,  -5,  -2, -20, -23, -44,
    -29, -51, -23, -15, -22, -18, -50, -64
};

short bishop_pst_eg[]={
     -14, -21, -11,  -8, -7,  -9, -17, -24,
     -8,  -4,   7, -12, -3, -13,  -4, -14,
      2,  -8,   0,  -1, -2,   6,   0,   4,
     -3,   9,  12,   9, 14,  10,   3,   2,
     -6,   3,  13,  19,  7,  10,  -3,  -9,
    -12,  -3,   8,  10, 13,   3,  -7, -15,
    -14, -18,  -7,  -1,  4,  -9, -15, -27,
    -23,  -9, -23,  -5, -9, -16,  -5, -17
};

short rook_pst_eg[]={
    13, 10, 18, 15, 12,  12,   8,   5,
    11, 13, 13, 11, -3,   3,   8,   3,
     7,  7,  7,  5,  4,  -3,  -5,  -3,
     4,  3, 13,  1,  2,   1,  -1,   2,
     3,  5,  8,  4, -5,  -6,  -8, -11,
    -4,  0, -5, -1, -7, -12,  -8, -16,
    -6, -6,  0,  2, -9,  -9, -11,  -3,
    -9,  2,  3, -1, -5, -13,   4, -20
};

short queen_pst_eg[]={
    -9,  22,  22,  27,  27,  19,  10,  20,
    -17,  20,  32,  41,  58,  25,  30,   0,
    -20,   6,   9,  49,  47,  35,  19,   9,
      3,  22,  24,  45,  57,  40,  57,  36,
    -18,  28,  19,  47,  31,  34,  39,  23,
    -16, -27,  15,   6,   9,  17,  10,   5,
    -22, -23, -30, -16, -16, -23, -36, -32,
    -33, -28, -22, -43,  -5, -32, -20, -41
};

short king_pst_eg[]={
    -74, -35, -18, -18, -11,  15,   4, -17,
    -12,  17,  14,  17,  17,  38,  23,  11,
     10,  17,  23,  15,  20,  45,  44,  13,
     -8,  22,  24,  27,  26,  33,  26,   3,
    -18,  -4,  21,  24,  27,  23,   9, -11,
    -19,  -3,  11,  21,  23,  16,   7,  -9,
    -27, -11,   4,  13,  14,   4,  -5, -17,
    -53, -34, -21, -11, -28, -14, -24, -43
};

short *pst_mg[6] = { pawn_pst_mg, rook_pst_mg, knight_pst_mg, bishop_pst_mg,  queen_pst_mg, king_pst_mg };
short *pst_eg[6] = { pawn_pst_eg, rook_pst_eg, knight_pst_eg, bishop_pst_eg, queen_pst_eg, king_pst_eg };

int pieces_values[]={PAWN_VALUE, ROOK_VALUE, KNIGHT_VALUE, BISHOP_VALUE, QUEEN_VALUE, KING_VALUE};
int passed_bonus_by_rank[][8] = {{0, 5, 10, 20, 35, 60, 100, 0},{0, 100, 60, 35, 20, 10, 5, 0}};

int play_bonus[]={0,100,0,20,50,50,1000,1500,-1,10000,0};






int center_bonus()
{
    int score=0;
    if (game.board[3][3].state==FULL && game.board[3][3].piece)
    {
        score+=(1-2*game.board[3][3].piece->color)*50;
    }
     if (game.board[3][4].state==FULL && game.board[3][4].piece)
    {
        score+=(1-2*game.board[3][4].piece->color)*50;
    }
     if (game.board[4][3].state==FULL && game.board[4][3].piece)
    {
        score+=(1-2*game.board[4][3].piece->color)*50;
    }
     if (game.board[4][4].state==FULL && game.board[4][4].piece)
    {
        score+=(1-2*game.board[4][4].piece->color)*50;
    }
    return score;
}


int friendly_pawn_on_file(int file, color col)
{
    uint64 fileMask =FILE_A<<file;
    if (fileMask & game.pawns[col])
        return 1;
    return 0;
}


int passed_isolated_pawns(color col)
{
    uint64_t bb = game.pawns[col];

    int val_passed=0, count_isolated=0;
    while (bb)
    {
        int sq = __builtin_ctzll(bb);
        bb &= bb - 1; 
        int row=sq/8;
        if(!(game.passed_mask[col][sq] & game.pawns[!col]))
            val_passed+=passed_bonus_by_rank[col][row];
          if(!(game.isolated_mask[col][sq] & game.pawns[col]))
            count_isolated++;
    }
    return val_passed + count_isolated*ISOLATED_PAWN_VAL;
}




int doubled_pawns(color col)
{
    int count=0;
    for (int i=0; i<8; i++)
    {
        uint64 fileMask = FILE_A<<i;
        uint64 pawns_on_file = game.pawns[col]&fileMask;
        int n = __builtin_popcountll(pawns_on_file);
        if (n>=2)
            count++;      
    }

    return count*DOUBLED_PAWNS;
}

int eval_pawns()
{
    int val_black=0, val_white=0;
    val_black+=doubled_pawns(BLACK);
    val_black+=passed_isolated_pawns(BLACK);
    val_white+=doubled_pawns(WHITE);
    val_white+=passed_isolated_pawns(WHITE);
    return val_white-val_black;
}

int mobility()
{
    move bank[300];
    int black_val=0, white_val=0;
    for (int i=0; i<game.count_soldiers[0]; i++)
    {
        if (!game.armys[0][i]->alive)
            continue;
        switch (game.armys[0][i]->piece)
        {
            case ROOK:
            {
                white_val+=(ROOK_MOBILITY)*rook_moves( game.armys[0][i], bank, 0);
                break;
            }
            case QUEEN:
            {
                white_val+=(QUEEN_MOBILITY)*queen_moves( game.armys[0][i], bank, 0);
                break;
            }
            case BISHOP:
            {
                white_val+=(BISHOP_MOBILITY)*bishop_moves(game.armys[0][i], bank, 0);
                break;
            }
            case KNIGHT:
            {
                white_val+=(KNIGHT_MOBILITY)*knight_moves(game.armys[0][i], bank, 0);
                break;
            }
            default:
                break;
        }
    }
    for (int i=0; i<game.count_soldiers[1]; i++)
    {
        if (!game.armys[1][i]->alive)
            continue;
        switch (game.armys[1][i]->piece)
        {
            case ROOK:
            {
                black_val+=(ROOK_MOBILITY)*rook_moves( game.armys[1][i], bank, 0);
                break;
            }
            case QUEEN:
            {
                black_val+=(QUEEN_MOBILITY)*queen_moves( game.armys[1][i], bank, 0);
                break;
            }
            case BISHOP:
            {
                black_val+=(BISHOP_MOBILITY)*bishop_moves( game.armys[1][i], bank, 0);
                break;
            }
            case KNIGHT:
            {
                black_val+=(KNIGHT_MOBILITY)*knight_moves( game.armys[1][i], bank, 0);
                break;
            }
            default:
                break;
        }
    }
    return white_val-black_val;
}



int bishop_eval()
{
    int val_white=0, val_black=0;
    if (game.bishops[0][0] && game.bishops[0][0]->alive && game.bishops[0][1] && game.bishops[0][1]->alive)
        val_white+=50;
    if (game.bishops[1][0] && game.bishops[1][0]->alive && game.bishops[1][1] && game.bishops[1][1]->alive)
        val_black+=50;
    return val_white-val_black;
}


int rook_eval()
{
    int val_white=0, val_black=0;
    if(game.rooks[0][0] && game.rooks[0][0]->alive)
    {
        if (game.rooks[0][0]->ind[0]==6)
            val_white+=10;
        if (!friendly_pawn_on_file(game.rooks[0][0]->ind[1], WHITE))
            val_white+=10;
        if ((!friendly_pawn_on_file(game.rooks[0][0]->ind[1], BLACK)))
            val_white+=10; 
    }
    if(game.rooks[0][1] && game.rooks[0][1]->alive)
    {
        if (game.rooks[0][1]->ind[0]==6)
            val_white+=10;
        if (!friendly_pawn_on_file(game.rooks[0][1]->ind[1], WHITE))
            val_white+=10;
        if ((!friendly_pawn_on_file(game.rooks[0][1]->ind[1], BLACK)))
            val_white+=10; 
    }
    if(game.rooks[1][0] && game.rooks[1][0]->alive)
    {
        if (game.rooks[1][0]->ind[0]==1)
            val_black+=10;
        if (!friendly_pawn_on_file(game.rooks[1][0]->ind[1], WHITE))
            val_black+=10;
        if ((!friendly_pawn_on_file(game.rooks[1][0]->ind[1], BLACK)))
            val_black+=10; 
    }
    if(game.rooks[1][1] && game.rooks[1][1]->alive)
    {
        if (game.rooks[1][1]->ind[0]==1)
            val_black+=10;
        if (!friendly_pawn_on_file(game.rooks[1][1]->ind[1], WHITE))
            val_black+=10;
        if ((!friendly_pawn_on_file(game.rooks[1][1]->ind[1], BLACK)))
            val_black+=10; 
    }
    if(game.rooks[0][0] && game.rooks[0][1] && game.rooks[0][0]->alive && game.rooks[0][1]->alive)
    {
         if (game.rooks[0][0]->ind[0]==game.rooks[0][1]->ind[0])
        {
            val_white+=150;
            for (int i=MIN(game.rooks[0][0]->ind[1],game.rooks[0][1]->ind[1])+1; i<=MAX(game.rooks[0][1]->ind[0],game.rooks[0][1]->ind[0])-1; i++)
            {
                if(game.board[game.rooks[0][0]->ind[0]][i].state==FULL)
                {
                    val_white-=150;
                    break;
                }
            }

             if (game.rooks[0][0]->ind[1]==game.rooks[0][1]->ind[1])
            {
                val_white+=150;
                for (int i=MIN(game.rooks[0][0]->ind[0],game.rooks[0][1]->ind[0])+1; i<=MAX(game.rooks[0][1]->ind[0],game.rooks[0][1]->ind[0])-1; i++)
                {
                    if(game.board[i][game.rooks[0][0]->ind[1]].state==FULL)
                    {
                        val_white-=150;
                        break;
                    }
                }
            }
        }
    }
    if(game.rooks[1][0] && game.rooks[1][1] && game.rooks[1][0]->alive && game.rooks[1][1]->alive)
    {
         if (game.rooks[1][0]->ind[0]==game.rooks[1][1]->ind[0])
        {
            val_black+=150;
            for (int i=MIN(game.rooks[1][0]->ind[1],game.rooks[1][1]->ind[1])+1; i<=MAX(game.rooks[1][0]->ind[0],game.rooks[1][1]->ind[0])-1; i++)
            {
                if(game.board[game.rooks[1][0]->ind[0]][i].state==FULL)
                {
                    val_black-=150;
                    break;
                }
            }
             if (game.rooks[1][0]->ind[1]==game.rooks[1][1]->ind[1])
            {
                val_black+=150;
                for (int i=MIN(game.rooks[1][0]->ind[0],game.rooks[1][1]->ind[0])+1; i<=MAX(game.rooks[1][0]->ind[0],game.rooks[1][1]->ind[0])-1; i++)
                {
                    if(game.board[i][game.rooks[1][0]->ind[1]].state==FULL)
                    {
                        val_black-=150;
                        break;
                    }
                }
            }
        }
    }


    return val_white-val_black;
}


int pawn_shield_white()
{   color col=WHITE;
    int count = 0;
    if (game.kings[col]->ind[0]<7 && (game.pawns[col]& (1ULL<<(pst_index_black(game.kings[col]->ind[0]+1, game.kings[col]->ind[1])))))
        count++;

    if (game.kings[col]->ind[0]>0 && (game.pawns[col]& (1ULL<<(pst_index_black(game.kings[col]->ind[0]-1, game.kings[col]->ind[1])))))
        count++;

    if (game.kings[col]->ind[1]<7 && (game.pawns[col]& (1ULL<<(pst_index_black(game.kings[col]->ind[0], game.kings[col]->ind[1]+1)))))
        count++;

    if (game.kings[col]->ind[0]>0 && (game.pawns[col]& (1ULL<<(pst_index_black(game.kings[col]->ind[0]+1, game.kings[col]->ind[1]-1)))))
        count++;

    if (game.kings[col]->ind[0]<7 && game.kings[col]->ind[1]<7 && (game.pawns[col]& (1ULL<<(pst_index_black(game.kings[col]->ind[0]+1, game.kings[col]->ind[1]+1)))))
        count++;

    

    return count*KING_PAWNS;
}



int pawn_shield_black()
{   color col=BLACK;
    int count = 0;
    if (game.kings[col]->ind[0]<7 && (game.pawns[col]& (1ULL<<(pst_index_black(game.kings[col]->ind[0]+1, game.kings[col]->ind[1])))))
        count++;

    if (game.kings[col]->ind[0]>0 && (game.pawns[col]& (1ULL<<(pst_index_black(game.kings[col]->ind[0]-1, game.kings[col]->ind[1])))))
        count++;

        
    if (game.kings[col]->ind[1]<7 && (game.pawns[col]& (1ULL<<(pst_index_black(game.kings[col]->ind[0], game.kings[col]->ind[1]+1)))))
        count++;

    if (game.kings[col]->ind[0]>0 && (game.pawns[col]& (1ULL<<(pst_index_black(game.kings[col]->ind[0]+1, game.kings[col]->ind[1]-1)))))
        count++;

    if (game.kings[col]->ind[0]>0 && game.kings[col]->ind[0]>0  &&  (game.pawns[col]& (1ULL<<(pst_index_black(game.kings[col]->ind[0]-1, game.kings[col]->ind[1]-1)))))
        count++;

    return count*KING_PAWNS;
}


int king_shield()
{
    return  pawn_shield_white()-pawn_shield_black();
}


int eval_position()
{
    int mg_score = game.pst_white_mg - game.pst_black_mg;
    int eg_score = game.pst_white_eg - game.pst_black_eg;
    
    int phase_score;
    
    if (game.start_sum_pieces == 0) {
        phase_score = eg_score;
    } 
    else 
    {
        int mg_weight = game.sum_pieces; 
        
        int eg_weight = game.start_sum_pieces - game.sum_pieces; 
        
        phase_score = (mg_score * mg_weight + eg_score * eg_weight) / game.start_sum_pieces;
    }
    return phase_score  + eval_pawns() + bishop_eval() + rook_eval() + center_bonus() + king_shield();
}

