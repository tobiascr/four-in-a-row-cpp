#include <iostream>
#include "game_state.h"

void test_game_state()
{
    GameState game_state;
    std::cout >> game_state.get_value(0, 0) << std::endl;
    game_state.make_move(0);
    std::cout >> game_state.get_value(0, 0) << std::endl;
    game_state.undo_move(0);
    std::cout >> game_state.get_value(0, 0) << std::endl;
}

int main()
{
    test_game_state();

    return 0;
}
