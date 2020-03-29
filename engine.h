#ifndef ENGINE_H
#define ENGINE_H

#include "game_state.h"

/* Positions are represented as lists of length 9 where each entry is " ", "X" or "O"
 (the letter O). The diagram below shows which square corresponds to
 which place in the list.

  --- --- ---
 | 0 | 1 | 2 |
  --- --- ---
 | 3 | 4 | 5 |
  --- --- ---
 | 6 | 7 | 8 |
  --- --- ---

The random number generator is used in this library. It needs to be initialized with srand.
*/

int negamax(GameState& game_state, int last_move, int depth);
/* Compute a value of game_state. Return a positive integer for a winning game_state for
   the player in turn, 0 for a draw and a negative integer for a loss. A win or loss in_row
   fewer moves gives a higher score in absolute value. last_move is the last position
   where a move has been made to.
*/

int random_engine_move(GameState& game_state, int depth);
/* Return an integer from 0 to 8 that represents a best move made by the engine
   at the given depth level. If there are several equally good moves, one of them
   is chosen randomly.
*/

int random_first_move();
//Return a random move where the central move is made more likely to occur.

int engine_move_easy(GameState& game_state);

int engine_move_medium(GameState& game_state);

int engine_move_perfect(GameState& game_state);

#endif
