#include <stdlib.h>
#include <time.h>
#include "test_engine_API.h"
#include "test_game_state.h"

namespace TestEngine
{

EngineAPI::EngineAPI()
{
    std::srand(time(NULL)); // Initialize the random number generator.
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

int EngineAPI::negamax(int last_move, int depth, int alpha, int beta)
{
    int first_move = 3;
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
    int moves[7] = {3, 2, 4, 1, 5, 0, 6};

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
    int random_number = std::rand();
    int alpha = -10000;
    int beta = 10000;
    int moves[7] = {3, 2, 4, 1, 5, 0, 6};

    // Adding some randomness to the move order.
    if (random_number % 2 == 0)
    {
        moves[1] = 4;
        moves[2] = 2;
    }

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
    // Find the number of columns that are not full.
    int columns = 0;
    for (int i=0; i<=6; i++)
    {
        if (game_state.column_not_full(i))
            columns++;
    }

    if (columns < 4)
        return random_engine_move(30);
    if (columns == 4)
        return random_engine_move(16);
    if (columns == 5)
        return random_engine_move(12);
    return random_engine_move(8);
}
}
