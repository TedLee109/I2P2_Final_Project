#pragma once
#include "../state/state.hpp"
#include <iostream>



/**
 * @brief Policy class for random policy, 
 * your policy class should have get_move method
 */
class StrongAlphaBeta{
public:
  static Move get_move(State *state, int depth);
  static int alphabeta(State* root, int depth, bool maximizer, int, int);
};