#ifndef ENGINE_API_H
#define ENGINE_API_H

#include <array>
#include <vector>
#include <random>
#include <string>
#include "game_state.h"
#include "opening_book.h"
#include "transposition_table.h"

namespace Engine
{

class EngineAPI
{
public:
    EngineAPI();

    EngineAPI(unsigned int seed);
    // This constructor take a random number generator seed as an argument.

    void set_difficulty_level(int difficulty_level);
    // difficulty_level intended for game play are 1, 2 or 3.
    // Some other levels can be made as well. See the code.

    void new_game();

    void clear_transposition_table();
    /* Normally the transposition table does not need to be cleared. But for some testing it
    can be useful.*/

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

    int position_value_full_depth(const bool use_opening_book=true);
    /* Compute a value of the current position at full depth. Return a positive
    integer for a winning game_state for the player in turn, 0 for a draw or unknown
    outcome and a negative integer for a loss. A win at move 42 give the value 1,
    a win at move 41 give a the value 2 etc, and vice versa for losses.
    This function can only used for positions that has no four in a rows.*/

private:
    Engine::GameState game_state;
    Engine::OpeningBook opening_book;
    Engine::TranspositionTable transposition_table;
    int difficulty_level_;
    std::mt19937 random_generator;
    uint64_t transposition_table_2[100000] = {0};

    int position_heuristic(int move) const;

    int position_heuristic_2(int move) const;

    int random_game_heuristic();

    std::array<int,7> move_order();

    std::array<int,7> move_order(uint64_t moves_bitboard);

    std::array<int,7> move_order_random_games();

    std::array<int,7> move_order(int first_move);

    int negamax(const int depth, int alpha, int beta);

    std::array<int,2> root_negamax(const int depth,
                  std::array<int,7> move_order, int alpha, int beta);

    int iterative_deepening_full_depth_value();

    int iterative_deepening_full_depth_move(std::array<int,7> move_order_);

    int iterative_deepening_full_depth_move_likely_win(std::array<int,7> move_order_);

    int engine_move(const int depth);

    int random_move();

    int engine_move_easy();

    int engine_move_medium();

    int engine_move_hard();
};
}

#endif
