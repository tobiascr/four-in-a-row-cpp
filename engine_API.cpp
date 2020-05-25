#include <array>
#include <random>
#include <iostream>
#include <algorithm>
#include <utility>
#include "engine_API.h"
#include "game_state.h"

namespace Engine
{

EngineAPI::EngineAPI()
{
    // Initialize the random number generator.
    std::random_device rd;
    random_generator.seed(rd());
    difficulty_level_ = 2;
}

EngineAPI::EngineAPI(unsigned int seed)
{
    // Initialize the random number generator.
    random_generator.seed(seed);
    difficulty_level_ = 2;
}

void EngineAPI::set_difficulty_level(int difficulty_level)
{
    difficulty_level_ = difficulty_level;
}

void EngineAPI::new_game()
{
    game_state.reset();
    transposition_table.clear();
}

bool EngineAPI::legal_move(int column)
{
    return game_state.column_not_full(column);
}

int EngineAPI::engine_move()
{
    if (difficulty_level_ == 1)
        return engine_move_easy();
    if (difficulty_level_ == 2)
        return engine_move_medium();
    if (difficulty_level_ == 3)
        return engine_move_hard();
    return 0;
}

void EngineAPI::make_move(int position)
{
    game_state.make_move(position);
}

bool EngineAPI::board_full()
{
    return game_state.board_full();
}

char EngineAPI::get_value(int column, int row)
{
    return game_state.get_value(column, row);
}

bool EngineAPI::four_in_a_row()
{
    return game_state.four_in_a_row();
}

int EngineAPI::position_heuristic(int move) const
/*Give a heuristic evaluation in form of a number of how good it would be to make
 the given move to the current game state. The value is higher the better the move.
 Central positions are given higher values. If the move is not legal, the value is 0.
*/
{
    if (not game_state.column_not_full(move)) {return 0;}
    int row = game_state.get_number_of_disks_in_column(move);
    const int values[6][7] =
        {{0, 0, 0, 0, 0, 0, 0},
         {0, 0, 1, 1, 1, 0, 0},
         {0, 1, 1, 1, 1, 1, 0},
         {0, 1, 1, 1, 1, 1, 0},
         {0, 0, 1, 1, 1, 0, 0},
         {0, 0, 0, 0, 0, 0, 0}};
    return values[row][move];
}

int EngineAPI::position_heuristic_2(int move) const
/*Give a heuristic evaluation in form of a number of how good it would be to make
 the given move to the current game state. The value is higher the better the move.
 Central positions are given higher values. If the move is not legal, the value is 0.
*/
{
    if (not game_state.column_not_full(move)) {return 0;}
    int row = game_state.get_number_of_disks_in_column(move);
    const int values[6][7] =
        {{-1, 0, 0, 1, 0, 0, -1},
         {0,  1, 2, 3, 2, 1,  0},
         {0,  2, 3, 3, 3, 2,  0},
         {0,  2, 3, 3, 3, 2,  0},
         {0,  1, 2, 3, 2, 1,  0},
         {-1, 0, 0, 1, 0, 0, -1}};
    return values[row][move];
}

int EngineAPI::position_heuristic_3(int move) const
/*Give a heuristic evaluation in form of a number of how good it would be to make
 the given move to the current game state. The value is higher the better the move.
 Central positions are given higher values. If the move is not legal, the value is 0.
*/
{
    if (not game_state.column_not_full(move)) {return 0;}
    int row = game_state.get_number_of_disks_in_column(move);
    const int values[6][7] =
        {{3, 4,  5,  7,  5, 4, 3},
         {4, 6,  8, 10,  8, 6, 4},
         {5, 8, 11, 13, 11, 8, 5},
         {5, 8, 11, 13, 11, 8, 5},
         {4, 6,  8, 10,  8, 6, 4},
         {3, 4,  5,  7,  5, 4, 3}};
    return values[row][move];
}

int EngineAPI::open_four_in_a_row_count(int player) const
/* Return the number of unoccupied places on the board that are not in the bottom
   of the columns, that give player a four in a row. player is 0 for the player
   making the first move and 1 for the other player.*/
{
    int count = 0;
    for (int col=0; col<=6; col++)
    {
        for (int row=game_state.get_number_of_disks_in_column(col) + 1; row<=5; row++)
        {
            if (game_state.four_in_a_row(player, col, row))
            {
                count++;
            }
        }
    }
    return count;
}

int EngineAPI::open_four_in_a_row_heuristic(int move)
/* Give a heuristic evaluation in form of a number of how good it would be to make
 the given move to the current game state. The value is higher the better the move.
 The value is based on the number of open four in a rows.
*/
{
    int value;
    game_state.make_move(move);
    value = EngineAPI::open_four_in_a_row_count(0) - EngineAPI::open_four_in_a_row_count(1);
    game_state.undo_move(move);
    if (game_state.get_number_of_moves() % 2 == 0)
    {
        return value;
    }
    else
    {
        return -value;
    }
}

std::array<int,7> EngineAPI::move_order()
{
    std::array<int,7> moves = {3, 2, 4, 1, 5, 0, 6};
    int values[7];

    // Adding randomness to the move order.
    shuffle (moves.begin(), moves.end(), random_generator);

    for (int n=0; n<=6; n++)
    {
          values[n] = position_heuristic_2(n);
    }

    std::stable_sort(moves.begin(), moves.end(),
                     [&values](int i, int j){return values[i] > values[j];});
    return moves;
}

int EngineAPI::negamax(const int depth, int alpha, int beta)
{
    int move;
    int value;
    std::pair<uint64_t, uint64_t> key;

    if (game_state.can_win_this_move())
    {
        return 42 - game_state.get_number_of_moves();
    }

    if (game_state.get_number_of_moves() == depth - 1)
    {
        return 0;
    }

    const bool use_transposition_table = depth - game_state.get_number_of_moves() > 7;
    if (use_transposition_table)
    {
        key = game_state.get_key_2();
        if (transposition_table.count(key) == 1)
        {
            int lower_bound = transposition_table[key];
            if (lower_bound >= beta)
            {
                return beta;
            }
            else
            {
                if (lower_bound > alpha)
                {
                    alpha = lower_bound;
                }
            }
        }
    }

    // Move order.
    const int moves[7] = {3, 2, 4, 1, 5, 0, 6};

    for (int i=0; i<=6; i++)
    {
        move = moves[i];
        if (game_state.column_not_full(move))
        {
            game_state.make_move(move);
            value = -negamax(depth, -beta, -alpha);
            game_state.undo_move(move);
            if (value >= beta) // Fail hard beta-cutoff.
            {
                if (use_transposition_table)
                {
                    transposition_table[key] = value;
                }
                return beta;
            }
            if (value > alpha)
            {
                alpha = value;
                if (use_transposition_table)
                {
                    transposition_table[key] = value;
                }
            }
        }
    }

    return alpha;
}

int EngineAPI::root_negamax(const int depth, std::array<int,7> move_order, int alpha, int beta)
{
    int new_value;
    int move;
    int best_move;

    for (int n=0; n<=6; n++)
    {
        move = move_order[n];
        if (game_state.column_not_full(move))
        {
            game_state.make_move(move);
            new_value = -negamax(depth, -beta, -alpha);
            if (new_value > alpha)
            {
                alpha = new_value;
                best_move = move;
            }
            game_state.undo_move(move);
        }
    }
    return best_move;
}

int EngineAPI::random_engine_move(const int depth)
{
    int move;
    int result;
    int alpha = -1000;
    int beta = 1000;

    transposition_table.clear();

    std::array<int,7> moves = move_order();

    // Look for a move that makes a four in a row.
    for (int n=0; n<=6; n++)
    {
        move = moves[n];
        if (game_state.column_not_full(move))
        {
            game_state.make_move(move);
            result = game_state.four_in_a_row();
            game_state.undo_move(move);
            if (result) {return move;}
        }
    }

    // Look for blocking moves.
    for (int n=0; n<=6; n++)
    {
        move = moves[n];
        if (game_state.column_not_full(move))
        {
            if (game_state.is_blocking_move(move)) {return move;}
        }
    }

    return root_negamax(depth, moves, alpha, beta);
}

int EngineAPI::engine_move_easy()
{
    int moves = game_state.get_number_of_moves();
    int depth = moves + 2;
    if (depth > 42) {depth = 42;}
    return random_engine_move(depth);
}

int EngineAPI::engine_move_medium()
{
    int moves = game_state.get_number_of_moves();
    int depth = moves + 4;
    if (depth > 42) {depth = 42;}
    return random_engine_move(depth);
}

int EngineAPI::engine_move_hard()
{
    int number_of_moves = game_state.get_number_of_moves();
    int depth;

    // Some opening moves.
    if (number_of_moves < 2) {return 3;}

    if (number_of_moves > 14)   //14
    {
        return random_engine_move(42);
    }
    if (number_of_moves > 6)
    {
        depth = number_of_moves + 20; //18
        if (depth > 42) {depth = 42;}
        return random_engine_move(depth);
    }

    depth = number_of_moves + 14; //12
    if (depth > 42) {depth = 42;}
    return random_engine_move(depth);
}
}
