#include <array>
#include <random>
#include <iostream>
#include <algorithm>
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

void EngineAPI::set_difficulty_level(int difficulty_level)
{
    difficulty_level_ = difficulty_level;
}

void EngineAPI::new_game()
{
    game_state.reset();
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

bool EngineAPI::four_in_a_row(int column)
{
    return game_state.four_in_a_row(column);
}

int EngineAPI::heuristic_value(int move)
{
    /*Give a heuristic evaluation in form of a number of how good it would be to make
    the given move to the current game state. The value is higher the better the move.
    Central positions are given higher values. If the move is not legal, the value is 0.
    */
    if (not game_state.column_not_full(move)) {return 0;}
    int row = game_state.get_number_of_disks_in_column(move);
    int values[6][7] =
        {{0, 0, 0, 0, 0, 0, 0},
         {0, 0, 1, 1, 1, 0, 0},
         {0, 1, 1, 1, 1, 1, 0},
         {0, 1, 1, 1, 1, 1, 0},
         {0, 0, 1, 1, 1, 0, 0},
         {0, 0, 0, 0, 0, 0, 0}};
    return values[row][move];
}

std::array<int,7> EngineAPI::move_order()
{
    std::array<int,7> moves = {3, 2, 4, 1, 5, 0, 6};

    // Adding randomness to the move order.
    shuffle (moves.begin(), moves.end(), random_generator);

    std::array<int,7> sorted_moves;

    int i=0;
    for (int n=0; n<=6; n++)
    {
        if (heuristic_value(moves[n]) == 1)
        {
            sorted_moves[i] = moves[n];
            i++;
        }
    }
    for (int n=0; n<=6; n++)
    {
        if (heuristic_value(moves[n]) == 0)
        {
            sorted_moves[i] = moves[n];
            i++;
        }
    }

    return sorted_moves;
}

int EngineAPI::negamax(int last_move, int depth, int alpha, int beta)
{
    int move;
    int value;

    if (game_state.four_in_a_row(last_move))
    {
        return -43 + game_state.get_number_of_moves();
    }

    if (game_state.board_full())
    {
        return 0;
    }

    if (depth == 0)
    {
        return 0;
    }

    // Move order.
    const int moves[7] = {3, 2, 4, 1, 5, 0, 6};

    for (int i=0; i<=6; i++)
    {
        move = moves[i];
        if (game_state.column_not_full(move))
        {
            game_state.make_move(move);
            value = -negamax(move, depth - 1, -beta, -alpha);
            game_state.undo_move(move);
            if (value >= beta) // Fail hard beta-cutoff.
            {
                return beta;
            }
            if (value > alpha)
            {
                alpha = value;
            }
        }
    }
    return alpha;
}

int EngineAPI::random_engine_move(int depth)
{
    int new_value;
    int best_move;
    int move;
    int alpha = -10000;
    int beta = 10000;

    std::array<int,7> moves = move_order();

    for (int n=0; n<=6; n++)
    {
        move = moves[n];
        if (game_state.column_not_full(move))
        {
            game_state.make_move(move);
            new_value = -negamax(move, depth, -beta, -alpha);
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

int EngineAPI::engine_move_easy()
{
    return random_engine_move(1);
}

int EngineAPI::engine_move_medium()
{
    return random_engine_move(4);
}

int EngineAPI::engine_move_hard()
{
    // Some opening moves.
    if (game_state.get_number_of_moves() <= 2) {return 3;}

    // Find the number of columns that are not full.
    int columns_not_full = 0;
    for (int i=0; i<=6; i++)
    {
        if (game_state.column_not_full(i))
            columns_not_full++;
    }

    if (columns_not_full < 4) {return random_engine_move(20);}
    if (columns_not_full == 4) {return random_engine_move(18);}
    if (columns_not_full == 5) {return random_engine_move(14);}
    if (columns_not_full == 6) {return random_engine_move(11);}
    if (game_state.get_number_of_moves() <= 8)
    {
        return random_engine_move(7);
    }
    else
    {
        return random_engine_move(9);
    }
}
}
