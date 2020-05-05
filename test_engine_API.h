#ifndef TEST_ENGINE_API_H
#define TEST_ENGINE_API_H

#include <array>
#include <random>
#include <algorithm>
#include "test_game_state.h"

namespace TestEngine
{

class EngineAPI
{
public:
    EngineAPI();

    void set_difficulty_level(int difficulty_level);
    // difficulty_level can be 1, 2 or 3.

    void new_game();

    bool legal_move(int column);

    int engine_move();
    // Return an integer från 0 to 6 that represents a move computed by the engine.

    void make_move(int position);

    bool board_full();

    char get_value(int column, int row);
    /* Return '1', '2' or '0' depending on the state of the corresponding position.
       '1' represents the player making the first move, '2' the player making
       second move and '0' that the position is empty.*/

    bool four_in_a_row();

private:
    TestEngine::GameState game_state;
    int difficulty_level_;

    std::mt19937 random_generator;

    int heuristic_value(int move) const;

    std::array<int,7> move_order();
    /* Return a suggested move order for a root negamax search based on a heuristic evaluation
       of the current game state. There is some randomness included in the move ordering
       for moves that are estimated to be equally strong.*/

    bool can_win_this_move();

    int negamax(const int depth, int alpha, int beta);
    /* Compute a value of game_state. Return a positive integer for a winning game_state for
       the player in turn, 0 for a draw or unknown outcome and a negative integer for a loss.
       A win at move 42 give the score 1, a win at move 41 give a the score 2 etc,
       and vice versa for losses.
       Depth is counted as the move number at which the search is stopped. For example,
       depth=42 give a maximum depth search. This function can only be used on if the game state
       have no four in a row.
    */

    int random_engine_move(const int depth);
    /* Return an integer from 0 to 6 that represents a best move made by the engine
       at the given depth level. If there are several equally good moves, one of them
       is chosen randomly. Depth is counted as the move number at which the search is stopped.
       For example, depth=42 give a maximum depth search.
    */

    int engine_move_easy();

    int engine_move_medium();

    int engine_move_hard();
};
}

#endif
