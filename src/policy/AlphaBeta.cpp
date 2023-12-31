#include <cstdlib>
#include <algorithm>

#include "../state/state.hpp"
#include "./AlphaBeta.hpp"

//it's not a stable algorithm

using namespace std;

const int max_score = 10000;
const int min_score = -10000;

/**
 * @brief Randomly get a legal action
 * 
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move 
 */
Move AlphaBeta::get_move(State *state, int depth){
  if(!state->legal_actions.size())
    state->get_legal_actions();
  
  auto actions = state->legal_actions;
  Move next_move = actions[0]; // default, in case of every actions will cause us to loose
    if(state->player == 0) {
        int max = min_score, score;
        for(auto m : actions) {
            if((score = alphabeta(state->next_state(m), depth - 1, false, min_score, max_score)) > max) {
                //We could never let opponent win. The win codition is for opponent
                if(state->next_state(m)->game_state != WIN) {
                    next_move = m;
                    max = score;
                }
                
            }
        }
        
    } else {
        int min = max_score, score;
        for(auto m : actions) {
            if((score = alphabeta(state->next_state(m), depth - 1, true, min_score, max_score)) < min) {
                //We could never let opponent win. The win codition is for opponent
                if(state->next_state(m)->game_state != WIN){
                    next_move = m; 
                    min = score;
                } 
            }
        }
    }
    
  return next_move;
}

int AlphaBeta::alphabeta(State* root, int depth, bool maximizer, int alpha, int beta) {
    if(depth == 0) {
        return root->evaluate();
    } 
    if(maximizer) {
        if(!root->legal_actions.size())
            root->get_legal_actions();
        auto actions = root->legal_actions;
        for(auto m : actions) {
            auto ns = root->next_state(m);
            alpha = max(alpha, alphabeta(ns, depth - 1, !maximizer, alpha, beta));
            if(alpha >= beta) break;
        }
        return alpha;
    }
    else {
        if(!root->legal_actions.size())
            root->get_legal_actions();
        for(auto m : root->legal_actions){
            auto ns = root->next_state(m);
            beta = min(beta, alphabeta(ns, depth - 1, !maximizer, alpha, beta));
            if(alpha >= beta) break;
        }
        return beta;
    }
}