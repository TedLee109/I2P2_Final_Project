#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <random>
#include <time.h>

#include "../state/state.hpp"
#include "./MCTS.hpp"


using namespace std;

#define simulate_time 300000


Move MCTS::get_move(State* state) {
    return monte_carlo_tree_search(state, state->player);
}

Move MCTS::monte_carlo_tree_search(State* root, int player) {
    double result;
    int k = 0;
    State* leaf;
    if(!root->legal_actions.size())
        root->get_legal_actions();
    for(auto m : root->legal_actions){
        auto ns = root->next_state(m);
        root->children.push_back(ns);
        ns->parent = root;
        ns->m = m;
    }
    while(k < simulate_time) {
        leaf = get_best_leaf(root);
        result = rollout(leaf, player);
        backpropagate(root, leaf, result);
        k++;
    }
    double max = -1.0;
    Move next_move = root->children[0]->m;
    for(auto s : root->children) {
        if(s->ni != 0) {
            if(s->w/(double)s->ni >= max) {
                next_move = s->m;
                max = s->w/(double)s->ni;
            }
                
        }
    }
    delete_tree(root);
    return next_move;
}

State* MCTS::get_best_leaf(State* root) {
    State* curnode = root, *next_node;
    while(curnode->children.size() && curnode->game_state != WIN && curnode->game_state != DRAW) {
        double best_score = -1.0;
        for(auto c : curnode->children) {
            if(c->ni == 0) {
                next_node = c;
                break;
            }
            // wi = the player of the curnode win how many times ?
            double wi = (curnode->player==root->player? c->w : c->ni - c->w);
            double UCB_score = wi/(double)c->ni + 1.414*(sqrt(log(curnode->ni)/(double)c->ni));
            if(UCB_score >= best_score) {
                next_node = c;
                best_score = UCB_score;
            }
        }
        curnode = next_node;
    }
    if(curnode->game_state == WIN || curnode->game_state == DRAW) return curnode;
    //expand if the node has been visited 
    srand(time(NULL));
    if(curnode->ni != 0 && !curnode->children.size()) {
        if(!curnode->legal_actions.size()) 
            curnode->get_legal_actions();
        for(auto m : curnode->legal_actions) {
            auto ns = curnode->next_state(m);
            curnode->children.push_back(ns);
            ns->parent = curnode;
        }
        //return the unvissited node;
        return curnode->children[rand()%curnode->children.size()];
    }
    return curnode;
}

double MCTS::rollout(State* leaf, int player) {
    State* curstate = leaf;
    srand(time(NULL));
    while(curstate->game_state != WIN && curstate->game_state != DRAW) {
        if(!curstate->legal_actions.size())
            curstate->get_legal_actions();
        auto ns = curstate->next_state(curstate->legal_actions[rand()%curstate->legal_actions.size()]);
        curstate = ns;
    }
    if(curstate->game_state == DRAW) return 0.5;
    return (curstate->player==player?1.0:0.0);
}

void MCTS::backpropagate(State* root, State* leaf, double result) {
    State* curnode = leaf;
    while(curnode != root) {
        curnode->ni++;
        curnode->w += result;
        curnode = curnode->parent;
    }
    curnode->ni++;
    curnode->w += result;
}

void MCTS::delete_tree(State* root) {
    if(root->children.size() != 0) {
        for(auto c : root->children) {
            delete_tree(c);
        }
    }
    delete root;
}