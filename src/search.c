#include "search.h"
#include "types.h"
#include <stdlib.h>
#include "move_gen.h"
#include "board.h"
#include "game_eval.h"
#include "make_move.h"
#include "game_state.h"
#include <stdio.h>
#define QUIESCE_DEPTH 3



uint8_t current_TT_age = 0;


void move_to_entry(TTEntry *TT,int ind, move* play, int flag, int depth, int score, uint64_t key)
{
    TT[ind].key=key;
    TT[ind].depth=depth;
    TT[ind].score=score;
    TT[ind].flag=flag;
    TT[ind].fromR=play->fromR;
    TT[ind].fromC=play->fromC;
    TT[ind].toR=play->toR;
    TT[ind].toC=play->toC;
    TT[ind].promotionTo=play->promotionTO;
}

move recover_move(TTEntry en)
{
    move curr;
    curr.fromR=en.fromR;
    curr.fromC=en.fromC;
    curr.toR=en.toR;
    curr.toC=en.toC;
    curr.promotionTO=en.promotionTo;
    curr.preCastlingBlack=game.black_castling;
    curr.preCastlingWhite=game.white_castling;
    curr.prev_hash_val=game.position_hash;
    curr.prev_el[0]=game.el[0];
    curr.prev_el[1]=game.el[1];
    curr.moved=game.board[curr.fromR][curr.fromC].piece;
    if ( game.board[curr.fromR][curr.fromC].piece->piece==PAWN && curr.toR==game.el[0] && curr.toC==game.el[1])
    {
        curr.play=EL;
        if (game.board[curr.fromR][curr.fromC].piece->color==BLACK)
            curr.eaten=game.board[curr.toR+1][curr.toC].piece;
        else
            curr.eaten=game.board[curr.toR-1][curr.toC].piece;
            
    }
    else if(game.board[curr.fromR][curr.fromC].piece->piece==PAWN)
    {
        if (curr.toR==7 || curr.toR==0)
        {
            if (game.board[curr.toR][curr.toC].piece)
            {
                curr.eaten=game.board[curr.toR][curr.toC].piece;
                curr.play=PROMOTION_CAPTURE;
            }
            else
            {
                curr.eaten=NULL;
                curr.play=PROMOTION;
            }
        }
    }
    else if (game.board[curr.fromR][curr.fromC].piece->piece==KING && ABS(curr.toC-curr.fromC)>1)
    {
        curr.eaten=NULL;
        if (curr.toC==2)
            curr.play=CASTLE_QUEEN;
        else
            curr.play=CASTLE_KING;
    }
    else if (game.board[curr.fromR][curr.fromC].piece->piece==PAWN && ABS(curr.toR-curr.fromR)==2)
    {
        curr.eaten=NULL;
        curr.play=DOUBLE_PAWN;
    }
    else if (game.board[curr.toR][curr.toC].piece)
    {
        curr.eaten=game.board[curr.toR][curr.toC].piece;
        curr.play=CAPTURE;
    }
    else
    {
        curr.eaten=NULL;
        curr.play=QUIET;
    }
    return curr;
}



move final_play;
int d;



int bucket_limits[9] = {-700, -350, -200, -120, 60, 120, 200, 350, 700};



int fast_eval(move m)
{
    int score = 0;

    if (m.play == CAPTURE || m.play == PROMOTION_CAPTURE||m.play==EL)
    {
        int victim_val = capture_values[m.eaten->piece];
        int attacker_val = capture_values[game.board[m.fromR][m.fromC].piece->piece];
        score=10000+(victim_val*10)-(attacker_val/10);
        return score;
    
    
    }
    if (m.play == PROMOTION || m.play == PROMOTION_CAPTURE)
        score += 9000;

    if (m.play == CASTLE_KING||m.play==CASTLE_QUEEN)
        score += 1000;

    return score;
}

int same_move(move a, move b)
{
    return (a.fromR == b.fromR &&
            a.fromC == b.fromC &&
            a.toR   == b.toR &&
            a.toC   == b.toC &&
            a.play  == b.play &&
            a.promotionTO == b.promotionTO);
}



int compare_moves(const void *a, const void *b)
{
    const move *move_a=(const move*)a;
    const move *move_b=(const move*)b;
    return move_b->score - move_a->score;
}

void ordering(move opt[], int size)
{
    qsort(opt,size, sizeof(move), compare_moves);
}


/*
used to make sure the last move doesnt leave any piece hanging without it being worth it

*/
int Quiesce( int alpha, int beta, int depth, color col) {
    int static_eval = eval_position();
    if (depth==0)
        return static_eval;
    if (col == WHITE) 
    {
        if (static_eval >= beta) 
            return static_eval;
        if (static_eval > alpha) 
            alpha = static_eval;
    } 
    else 
    {
        if (static_eval <= alpha) 
            return static_eval;
        if (static_eval < beta) 
            beta = static_eval;
    }
    int best_value = static_eval;
  
    move pseudo[330];
    move eat[330];
    int ind_pseudo=0, ind_eat=0;
    ind_pseudo=generate_pseudo_possible_moves(game.armys[col], game.count_soldiers[col], pseudo, 0);
    for (int k=0; k<ind_pseudo; k++)
    {
        if(pseudo[k].play==CAPTURE || pseudo[k].play==PROMOTION_CAPTURE || pseudo[k].play==PROMOTION || pseudo[k].play==EL)
        {
            eat[ind_eat]=pseudo[k];
            ind_eat+=1;
        }
    }
    for (int i = 0; i < ind_eat; i++)
    {
        eat[i].score = fast_eval(eat[i]);
    }
    
    ordering(eat, ind_eat);

    for( int i=0; i<ind_eat; i++ ) 
     {
        commit_move(eat[i]);
        if (check(col)) 
        {
            reverse_move(eat[i]);
            continue;
        }
        int score = Quiesce(alpha, beta , depth-1, !col);
        reverse_move(eat[i]);
        if (col == WHITE) 
        {
            if (score > best_value) 
                best_value = score;
            if (score >= beta) 
                return best_value;
            if (score > alpha) 
                alpha = score;
        } 
        else 
        {
            if (score < best_value) 
                best_value = score;
            if (score <= alpha) 
                return best_value;
            if (score < beta) 
                beta = score;
        }
    }
    return best_value;
}







/*
used alpha beta pruning the eliminate unneeded searches in branches that won't be chosen.
ordering to start the search from as good moves as possible efficiently to prune as many branches as possible.
Transposition table to avoid repeating calculation. used two level hashing to decrease collisions.
*/
int search_best_score(int depth, color col, int alpha, int beta)
{
    if (depth<=0)
    {
        return Quiesce(alpha,beta,QUIESCE_DEPTH,col);
    }
    size_t ind=game.position_hash&(TABLE_SIZE-1);

   
 
    if (game.TT[ind].slot[0].key == game.position_hash && game.TT[ind].slot[0].depth >= depth) {
        // 1. Exact Score: Return immediately
        if (game.TT[ind].slot[0].flag == 0) {
            if (depth == d) 
                final_play = recover_move(game.TT[ind].slot[0]); // Update global move!      
            return game.TT[ind].slot[0].score;
        }
        // 2. Lower Bound: If the score is >= beta, we can cause a cutoff immediately
        else if (game.TT[ind].slot[0].flag == 1 && game.TT[ind].slot[0].score >= beta) {
            return game.TT[ind].slot[0].score; 
        }
        // 3. Upper Bound: If the score is <= alpha, we can cause a cutoff immediately
        else if (game.TT[ind].slot[0].flag == 2 && game.TT[ind].slot[0].score <= alpha) {
            return game.TT[ind].slot[0].score;
        }
    }
    if (game.TT[ind].slot[1].key == game.position_hash && game.TT[ind].slot[1].depth >= depth) {
        // 1. Exact Score: Return immediately
        if (game.TT[ind].slot[1].flag == 0) {
            return game.TT[ind].slot[1].score;
        }
        // 2. Lower Bound: If the score is >= beta, we can cause a cutoff immediately
        else if (game.TT[ind].slot[1].flag == 1 && game.TT[ind].slot[1].score >= beta) {
            return game.TT[ind].slot[1].score; 
        }
        // 3. Upper Bound: If the score is <= alpha, we can cause a cutoff immediately
        else if (game.TT[ind].slot[1].flag == 2 && game.TT[ind].slot[1].score <= alpha) {
            return game.TT[ind].slot[1].score;
        }
    }
   
    int original_alpha=alpha;
    int original_beta=beta;
    int best_val;
    
    int ind_pseudo=0, ind_llegal=0;
    move pseudo[330];
    int curr_val;
    move best_move;
    ind_pseudo=generate_pseudo_possible_moves(game.armys[col], game.count_soldiers[col],pseudo ,ind_pseudo);
    for (int i = 0; i < ind_pseudo; i++)
    {
        pseudo[i].score = fast_eval(pseudo[i]);
    }
    
    ordering(pseudo, ind_pseudo);

    if (col)
        best_val=99999;
    else
        best_val=-99999;
   
    best_move=pseudo[0];
    int found_TT_move = 0;
    if (game.TT[ind].slot[0].key == game.position_hash) 
    { 
        for (int i = 0; i < ind_pseudo; i++) 
        { 
            if (pseudo[i].fromR == game.TT[ind].slot[0].fromR && 
                pseudo[i].fromC == game.TT[ind].slot[0].fromC && 
                pseudo[i].toR == game.TT[ind].slot[0].toR && 
                pseudo[i].toC == game.TT[ind].slot[0].toC &&
                pseudo[i].promotionTO == game.TT[ind].slot[0].promotionTo
            ) 
            { 
                move temp = pseudo[0]; 
                pseudo[0] = pseudo[i]; 
                pseudo[i] = temp; 
                found_TT_move = 1;
                break; 
            } 
        }
    } 
    if (!found_TT_move && game.TT[ind].slot[1].key == game.position_hash) 
    { 
        for (int i = 0; i < ind_pseudo; i++) 
        { 
            if (pseudo[i].fromR == game.TT[ind].slot[1].fromR && 
                pseudo[i].fromC == game.TT[ind].slot[1].fromC && 
                pseudo[i].toR == game.TT[ind].slot[1].toR && 
                pseudo[i].toC == game.TT[ind].slot[1].toC &&
                pseudo[i].promotionTO == game.TT[ind].slot[1].promotionTo
            ) 
            { 
                move temp = pseudo[0]; 
                pseudo[0] = pseudo[i]; 
                pseudo[i] = temp; 
                break; 
            } 
        } 
    }


    

    for(int i=0; i<ind_pseudo; i++)
    {
        commit_move(pseudo[i]);

        if(check(col))
        {
            reverse_move(pseudo[i]);
            continue;
        }
        ind_llegal+=1;
        curr_val=search_best_score(depth-1, !col,alpha, beta);
        reverse_move(pseudo[i]);
        
        if (col==BLACK)
        {
            if (curr_val<best_val)
            {
                best_val=curr_val;
                best_move=pseudo[i];
            }
            beta=MIN(beta,best_val);
                
        }
        else
        {
            if (curr_val>best_val)
            {
                best_move=pseudo[i];
                best_val=curr_val;
            }
            alpha=MAX(alpha, best_val);
        }
        if (alpha>=beta)
        {
           break;
        }
    }
    if (ind_llegal==0)
    {
        if (check(col))
            return (2*col-1)*(30000+depth);
        return 0;
    }
    int flag;
    if (best_val <= original_alpha) flag = 2; // Upper Bound
    else if (best_val >= original_beta) flag = 1; // Lower Bound
    else flag = 0; // Exact Score
    int target_slot=-1;
    if (game.TT[ind].slot[0].key == game.position_hash) target_slot = 0;
    else if (game.TT[ind].slot[1].key == game.position_hash) target_slot = 1;

    if (target_slot != -1) {
    // If the position is already in the game.TT, only overwrite if new search is deeper
    if (depth >= game.TT[ind].slot[target_slot].depth) {
        move_to_entry(game.TT[ind].slot, target_slot, &best_move, flag, depth, best_val, game.position_hash);
        game.TT[ind].slot[target_slot].age = current_TT_age;
    }
}
    
    else{ 
    if (game.TT[ind].slot[0].key == 0 || 
    game.TT[ind].slot[0].age != current_TT_age || 
    depth >= game.TT[ind].slot[0].depth) 
    {
        move_to_entry(game.TT[ind].slot, 0, &best_move, flag, depth, best_val, game.position_hash);
        game.TT[ind].slot[0].age = current_TT_age;
    }
// Check if it's already in Slot 1, or just overwrite Slot 1 (Always Replace)
    else 
    {
        move_to_entry(game.TT[ind].slot, 1, &best_move, flag, depth, best_val, game.position_hash);
        game.TT[ind].slot[1].age = current_TT_age;
    }
}
    
    

    if (depth==d && original_alpha == -99999)
        final_play=best_move;
    return best_val;

}





move find_best_move(color side, int maxDepth) 
{
    d = 0;
    for (int depth = 1; depth <= maxDepth; depth++) {
        d = depth;               // global search depth limit
        search_best_score(depth, side, -99999, 99999); // starting values for alpha and beta.
    }
    current_TT_age+=1;
    return final_play;
}

