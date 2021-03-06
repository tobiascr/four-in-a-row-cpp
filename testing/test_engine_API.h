#ifndef TEST_ENGINE_API_H
#define TEST_ENGINE_API_H

#include <array>
#include <vector>
#include <random>
#include <unordered_map>
#include <string>
#include "test_game_state.h"

namespace TestEngine
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
    TestEngine::GameState game_state;
    int difficulty_level_;
    const int max_ply_for_values_in_opening_book = 8;
    std::mt19937 random_generator;
    std::unordered_map<uint64_t, uint_fast16_t> transposition_table;
    std::unordered_map<uint64_t, std::string> opening_book_values;
    std::unordered_map<uint64_t, std::string> opening_book_moves;

    void load_opening_book(std::string file_name, bool values);

    bool can_find_best_moves_from_opening_book() const;

    std::vector<int> get_best_moves_from_opening_book();

    int position_heuristic(int move) const;

    int position_heuristic_2(int move) const;

    int left_right_balance() const;

    int adjacent_filled_position_count(int column) const;

    std::array<int,7> move_order();

    std::array<int,7> move_order_random();

    std::array<int,7> move_order(int first_move);

    std::array<int,7> move_order_open_four_in_a_row();

    int negamax(const int depth, int alpha, int beta, const bool use_opening_book);

    std::array<int,2> root_negamax(const int depth,
                  std::array<int,7> move_order, int alpha, int beta,
                  const bool use_opening_book);

    std::array<int,2> iterative_deepening(const int depth,
                  std::array<int,7> move_order_, int alpha, int beta,
                  const bool use_opening_book);

    int engine_move(const int depth, const bool use_opening_book);

    int random_move();

    int random_best_opening_move();

    int engine_move_easy();

    int engine_move_medium();

    int engine_move_hard();
};
}

#endif
