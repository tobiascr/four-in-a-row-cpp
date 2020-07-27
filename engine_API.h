#ifndef ENGINE_API_H
#define ENGINE_API_H

#include <array>
#include <random>
#include <unordered_map>
#include "game_state.h"

namespace Engine
{

class EngineAPI
{
public:
    EngineAPI();

    EngineAPI(unsigned int seed);
    // This constructor take a random number generator seed as an argument.

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

    int position_value_full_depth();
    /* Compute a value of the current position at full depth. Return a positive
       integer for a winning game_state for the player in turn, 0 for a draw or unknown
       outcome and a negative integer for a loss. A win at move 42 give the value 1,
       a win at move 41 give a the value 2 etc, and vice versa for losses.
       This function can only used for positions that have no four in a rows.
    */

private:
    Engine::GameState game_state;
    int difficulty_level_;
    const int max_number_of_moves_in_opening_book = 9;
    std::mt19937 random_generator;
    std::unordered_map<uint64_t, uint_fast16_t> transposition_table;
    std::unordered_map<uint64_t, short int> opening_book;

    void load_opening_book();

    int position_heuristic(int move) const;

    int position_heuristic_2(int move) const;

    int open_four_in_a_row_count(int player) const;

    int open_four_in_a_row_heuristic(int move);

    std::array<int,7> move_order();
    /* Return a move order for a root negamax search based on a heuristic evaluation
       of the current game state. There is some randomness included in the move ordering
       for moves that are estimated to be equally strong.*/

    std::array<int,7> move_order(int first_move);
    /* Return a move order with the given first move.*/

    std::array<int,7> move_order_open_four_in_a_row();

    short int negamax(const short int depth, short int alpha, short int beta);

    int root_negamax(const short int depth, std::array<int,7> move_order, short int alpha, short int beta);
    /* Return a move (0 to 6) computed with the negamax algorithm. Depth is counted as
    the move number at which the search is stopped.*/

    int engine_move(const short int depth);
    /* Return an integer from 0 to 6 that represents a best move made by the engine
       at the given depth level. Depth is counted as the move number at which the search is stopped.
       For example, depth=42 give a maximum depth search.
    */

    int engine_move_easy();

    int engine_move_medium();

    int engine_move_hard();
};
}

#endif
