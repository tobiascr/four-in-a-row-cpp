#include <stdlib.h>
#include "game_state.h"

int negamax(GameState& game_state, int last_move, int depth)
{
    int new_value;
    int best_value;

    if (game_state.three_in_a_row(last_move))
        return -1 - depth;

    if (game_state.board_full())
        return 0;

    if (depth == 0)
        return 0;

    best_value = -1000;
    for (int move=0; move<=8; move++)
        if (game_state.position_empty(move))
        {
            game_state.make_move(move);
            new_value = -negamax(game_state, move, depth - 1);
            if (new_value > best_value)
                best_value = new_value;
            game_state.undo_move(move);
        }
    return best_value;
}

int random_engine_move(GameState& game_state, int depth)
{
    int new_value;
    int best_value;
    int best_move;
    int move;
    int random_number = std::rand();
    int test;

    best_value = -1000;
    for (int n=0; n<=8; n++)
    {
        move = (n + random_number) % 9;
        if (game_state.position_empty(move))
        {
            game_state.make_move(move);
            new_value = -negamax(game_state, move, depth);
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

int random_first_move()
{
        if ((std::rand() % 12) < 3)
            return 4;
        else
            return std::rand() % 9;
}

int engine_move_easy(GameState& game_state)
{
    if (game_state.get_number_of_empty_positions() == 9)
        return random_first_move();
    return random_engine_move(game_state, 0);
}

int engine_move_medium(GameState& game_state)
{
    if (game_state.get_number_of_empty_positions() == 9)
        return random_first_move();
    return random_engine_move(game_state, 1);}

int engine_move_perfect(GameState& game_state)
{
    if (game_state.get_number_of_empty_positions() == 9)
        return random_first_move();
    return random_engine_move(game_state, 10);
}
