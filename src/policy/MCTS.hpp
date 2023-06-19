#pragma once
#include "../state/state.hpp"
#include <iostream>



/**
 * @brief Policy class for random policy, 
 * your policy class should have get_move method
 */
class MCTS{
public:
  static Move get_move(State *state);
  static Move monte_carlo_tree_search(State* root, int);
  static State* get_best_leaf(State* root);
  static double rollout(State* leaf, int);
  static void backpropagate(State* root, State* leaf, double);
  static void delete_tree(State* root);
};