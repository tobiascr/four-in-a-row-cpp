#include <stdlib.h>
#include "test_game_state.h"

int test_negamax(TestGameState& game_state, int last_move, int depth, int alpha, int beta)
{
    int move;
    int value;

    if (game_state.four_in_a_row(last_move))
        return -1 - depth;

    if (game_state.board_full())
        return 0;

    if (depth == 0)
        return 0;

    int moves[7] = {3, 2, 4, 1, 5, 0, 6};

    for (int i=0; i<=6; i++)
    {
        move = moves[i];
        if (game_state.column_not_full(move))
        {
            game_state.make_move(move);
            value = -test_negamax(game_state, move, depth - 1, -beta, -alpha);
            game_state.undo_move(move);
            if (value >= beta)
                return beta; // Fail hard beta-cutoff.
            if (value > alpha)
                alpha = value;
        }
    }
    return alpha;
}

int test_random_engine_move(TestGameState& game_state, int depth)
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
            new_value = -test_negamax(game_state, move, depth, -beta, -alpha);
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

int test_engine_move_easy(TestGameState& game_state)
{
    return test_random_engine_move(game_state, 1);
}

int test_engine_move_medium(TestGameState& game_state)
{
    return test_random_engine_move(game_state, 4);
}

int test_engine_move_hard(TestGameState& game_state)
{
    // Find the number of columns that are not full.
    int columns = 0;
    for (int i=0; i<=6; i++)
    {
        if (game_state.column_not_full(i))
            columns++;
    }

    if (columns < 5)
        return test_random_engine_move(game_state, 30);
    if (columns == 5)
        return test_random_engine_move(game_state, 18);
    return test_random_engine_move(game_state, 10);
}
