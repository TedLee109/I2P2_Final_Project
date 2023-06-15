#include <cstdlib>
#include <algorithm>

#include "../state/state.hpp"
#include "./MinMax.hpp"

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
Move MinMax::get_move(State *state, int depth){
  if(!state->legal_actions.size())
    state->get_legal_actions();
  
  auto actions = state->legal_actions;
  Move next_move;
    if(state->player == 0) {
        int max = min_score, score;
        for(auto m : actions) {
            if((score = minmax(state->next_state(m), depth - 1, false)) > max) {
                next_move = m;
                max = score;
            }
        }
        
    } else {
        int min = max_score, score;
        for(auto m : actions) {
            if((score = minmax(state->next_state(m), depth - 1, true)) < min) {
                next_move = m;
                min = score;
            }
        }
    }
    
  return next_move;
}

int MinMax::minmax(State* root, int depth, bool maximizer) {
    if(depth == 0) {
        return root->evaluate();
    } 
    if(maximizer) {
        int value = min_score;
        if(!root->legal_actions.size())
            root->get_legal_actions();
        auto actions = root->legal_actions;
        for(auto m : actions) {
            auto ns = root->next_state(m);
            value = max(value, minmax(ns, depth - 1, !maximizer));
        }
        return value;
    }
    else {
        int value = max_score;
        if(!root->legal_actions.size())
            root->get_legal_actions();
        for(auto m : root->legal_actions){
            auto ns = root->next_state(m);
            value = min(value, minmax(ns, depth - 1, !maximizer));
        }
        return value;
    }
}