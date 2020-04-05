#include <stdlib.h>
#include "test_game_state.h"

int test_negamax(TestGameState& game_state, int last_move, int depth)
{
    int new_value;
    int best_value;

    if (game_state.four_in_a_row(last_move))
        return -1 - depth;

    if (game_state.board_full())
        return 0;

    if (depth == 0)
        return 0;

    best_value = -1000;
    for (int move=0; move<=6; move++)
        if (game_state.column_not_full(move))
        {
            game_state.make_move(move);
            new_value = -test_negamax(game_state, move, depth - 1);
            if (new_value > best_value)
                best_value = new_value;
            game_state.undo_move(move);
        }
    return best_value;
}

int test_random_engine_move(TestGameState& game_state, int depth)
{
    int new_value;
    int best_value;
    int best_move;
    int move;
    int random_number = std::rand();

    best_value = -1000;
    for (int n=0; n<=6; n++)
    {
        move = (n + random_number) % 7;
        if (game_state.column_not_full(move))
        {
            game_state.make_move(move);
            new_value = -test_negamax(game_state, move, depth);
            if (new_value > best_value)
            {
                best_value = new_value;
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
    return test_random_engine_move(game_state, 7);
}
