#pragma once
#include "../state/state.hpp"
#include <iostream>



/**
 * @brief Policy class for random policy, 
 * your policy class should have get_move method
 */
class MinMax{
public:
  static Move get_move(State *state, int depth);
  static int minmax(State* root, int depth, bool maximizer);
};