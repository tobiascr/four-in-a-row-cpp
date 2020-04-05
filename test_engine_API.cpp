#include <stdlib.h>
#include <time.h>
#include "test_engine_API.h"

TestEngineAPI::TestEngineAPI()
{
    std::srand(time(NULL)); // Initialize the random number generator.
    difficulty_level_ = 2;
}

void TestEngineAPI::set_difficulty_level(int difficulty_level)
{
    difficulty_level_ = difficulty_level;
}

void TestEngineAPI::new_game()
{
    game_state.reset();
}

bool TestEngineAPI::legal_move(int column)
{
    return game_state.column_not_full(column);
}

int TestEngineAPI::engine_move()
{
    if (difficulty_level_ == 1)
        return test_engine_move_easy(game_state);
    if (difficulty_level_ == 2)
        return test_engine_move_medium(game_state);
    if (difficulty_level_ == 3)
        return test_engine_move_hard(game_state);
    return 0;
}

void TestEngineAPI::make_move(int position)
{
    game_state.make_move(position);
}

bool TestEngineAPI::board_full()
{
    return game_state.board_full();
}

char TestEngineAPI::get_value(int column, int row)
{
    return game_state.get_value(column, row);
}

bool TestEngineAPI::four_in_a_row(int column)
{
    return game_state.four_in_a_row(column);
}
