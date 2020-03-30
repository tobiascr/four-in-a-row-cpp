#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>
#include "game_state.h"
#include "engine.h"
#include "engine_API.h"

void load_position(GameState& game_state, std::string move_string)
/* Load a position to the given game_state object. A position is described
   as a string of moves ('0', '1', ..., '6'). For example move_string = "334".
   It gives the position that arises from first making move 3, then move 3 and then move 4. */
{
    game_state.reset();
    int n;
    for (n=0; n<move_string.length(); n++)
    {
        if (move_string[n] == '0')
            game_state.make_move(0);
        if (move_string[n] == '1')
            game_state.make_move(1);
        if (move_string[n] == '2')
            game_state.make_move(2);
        if (move_string[n] == '3')
            game_state.make_move(3);
        if (move_string[n] == '4')
            game_state.make_move(4);
        if (move_string[n] == '5')
            game_state.make_move(5);
        if (move_string[n] == '6')
            game_state.make_move(6);
    }
}

void load_position(EngineAPI& engine, std::string move_string)
/* Load a position to the given EngineAPI object. A position is described
   as a string of moves ('0', '1', ..., '6'). For example move_string = "334".
   It gives the position that arises from first making move 3, then move 3 and then move 4. */
{
    engine.new_game();
    int n;
    for (n=0; n<move_string.length(); n++)
    {
        if (move_string[n] == '0')
            engine.make_move(0);
        if (move_string[n] == '1')
            engine.make_move(1);
        if (move_string[n] == '2')
            engine.make_move(2);
        if (move_string[n] == '3')
            engine.make_move(3);
        if (move_string[n] == '4')
            engine.make_move(4);
        if (move_string[n] == '5')
            engine.make_move(5);
        if (move_string[n] == '6')
            engine.make_move(6);
    }
}

void print_board(GameState& game_state)
{
    int row;
    int col;
    std::cout << std::endl;
    for (row=5; row>=0; row--)
    {
        for (col=0; col<=6; col++)
            std::cout << game_state.get_value(col, row);
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void print_board(EngineAPI& engine)
{
    int row;
    int col;
    std::cout << std::endl;
    for (row=5; row>=0; row--)
    {
        for (col=0; col<=6; col++)
            std::cout << engine.get_value(col, row);
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void test_game_state()
{
    GameState game_state;
    print_board(game_state);
    game_state.make_move(0);
    game_state.make_move(0);
    game_state.make_move(3);
    game_state.make_move(6);
    print_board(game_state);
    game_state.undo_move(6);
    print_board(game_state);

    load_position(game_state, "34440");
    print_board(game_state);

    load_position(game_state, "000000111111222222333333444444555555666666");
    print_board(game_state);
    std::cout << game_state.board_full() << " "
              << game_state.get_number_of_moves() << std::endl; 
    load_position(game_state, "011111122222233333344455555666");
    print_board(game_state);
    std::cout << game_state.column_not_full(0) << std::endl;
    std::cout << game_state.column_not_full(1) << std::endl;
    std::cout << game_state.column_not_full(2) << std::endl;
    std::cout << game_state.column_not_full(3) << std::endl;
    std::cout << game_state.column_not_full(4) << std::endl;
    std::cout << game_state.column_not_full(5) << std::endl;
    std::cout << game_state.column_not_full(6) << std::endl;

    load_position(game_state, "010203");
    print_board(game_state);
    std::cout << game_state.four_in_a_row(0) << std::endl;
    load_position(game_state, "0102030");
    print_board(game_state);
    std::cout << game_state.four_in_a_row(0) << std::endl;
    load_position(game_state, "01020364");
    print_board(game_state);
    std::cout << game_state.four_in_a_row(2) << std::endl;
    std::cout << game_state.four_in_a_row(3) << std::endl;
    std::cout << game_state.four_in_a_row(0) << std::endl;
    std::cout << game_state.four_in_a_row(6) << std::endl;

    load_position(game_state, "01231234233");
    print_board(game_state);
    std::cout << game_state.four_in_a_row(0) << std::endl;
    std::cout << game_state.four_in_a_row(1) << std::endl;
    std::cout << game_state.four_in_a_row(2) << std::endl;
    std::cout << game_state.four_in_a_row(3) << std::endl;
    std::cout << game_state.four_in_a_row(4) << std::endl;

    load_position(game_state, "65435430433");
    print_board(game_state);
    std::cout << game_state.four_in_a_row(0) << std::endl;
    std::cout << game_state.four_in_a_row(3) << std::endl;
    std::cout << game_state.four_in_a_row(4) << std::endl;
    std::cout << game_state.four_in_a_row(5) << std::endl;
    std::cout << game_state.four_in_a_row(6) << std::endl;
}

void test_engine()
{
    int move;
    int depth;
    GameState game_state;
    print_board(game_state);
    depth = 7;
    move = random_engine_move(game_state, depth);
    std::cout << move << std::endl;

    load_position(game_state, "3344");
    print_board(game_state);
    move = random_engine_move(game_state, depth);
    std::cout << move << std::endl;
    move = engine_move_easy(game_state);
    std::cout << move << std::endl;
    move = engine_move_medium(game_state);
    std::cout << move << std::endl;
    move = engine_move_hard(game_state);
    std::cout << move << std::endl;
}

void test_engine_API()
{
    EngineAPI engine;
    std::cout << "engine_API test" << std::endl;
    engine.set_difficulty_level(3);
    print_board(engine);
    load_position(engine, "334");
    print_board(engine);
    std::cout << "Four in a row: " << engine.four_in_a_row(4) << std::endl;
    std::cout << "Board full: " << engine.board_full() << std::endl;
    std::cout << "Legal move 4: " << engine.legal_move(4) << std::endl;
    load_position(engine, "000111222333444555666000111222333444555666");
    print_board(engine);
    std::cout << "Legal move 4: " << engine.legal_move(4) << std::endl;
    std::cout << "Board full: " << engine.board_full() << std::endl;
    load_position(engine, "01231234233");
    print_board(engine);
    std::cout << "Four in a row: " << engine.four_in_a_row(0) << std::endl;
    load_position(engine, "3344");
    print_board(engine);
    std::cout << "Engine move: " << engine.engine_move() << std::endl;
    load_position(engine, "334455");
    print_board(engine);
    std::cout << "Engine move: " << engine.engine_move() << std::endl;
}

int main()
{
    std::srand(time(NULL)); // Initialize the random number generator.

    //test_game_state();
    //test_engine();
    test_engine_API();



    return 0;
}
