#include <stdlib.h>
#include <time.h>
#include "engine_API.h"

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
        return engine_move_easy(game_state);
    if (difficulty_level_ == 2)
        return engine_move_medium(game_state);
    if (difficulty_level_ == 3)
        return engine_move_hard(game_state);
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
