#ifndef ENGINE_H
#define ENGINE_H

#include "game_state.h"

/* The random number generator is used in this library. It needs to be initialized with srand.
*/

int negamax(GameState& game_state, int last_move, int depth, int alpha, int beta);
/* Compute a value of game_state. Return a positive integer for a winning game_state for
   the player in turn, 0 for a draw and a negative integer for a loss. A win or loss in_row
   fewer moves gives a higher score in absolute value. last_move is the last position
   where a move has been made to.
*/

int random_engine_move(GameState& game_state, int depth);
/* Return an integer from 0 to 6 that represents a best move made by the engine
   at the given depth level. If there are several equally good moves, one of them
   is chosen randomly.
*/

int engine_move_easy(GameState& game_state);

int engine_move_medium(GameState& game_state);

int engine_move_hard(GameState& game_state);

#endif
