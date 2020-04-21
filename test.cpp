#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>
#include <chrono>
#include "game_state.h"
#include "engine_API.h"
#include "test_engine_API.h"
#include "transposition_table.h"

void load_position(Engine::GameState& game_state, std::string move_string)
/* Load a position to the given game_state object. A position is described
   as a string of moves ('0', '1', ..., '6'). For example move_string = "334".
   It gives the position that arises from first making move 3, then move 3 and then move 4. */
{
    using namespace Engine;
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

void load_position(Engine::EngineAPI& engine, std::string move_string)
/* Load a position to the given EngineAPI object. A position is described
   as a string of moves ('0', '1', ..., '6'). For example move_string = "334".
   It gives the position that arises from first making move 3, then move 3 and then move 4. */
{
    using namespace Engine;
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

void print_board(Engine::GameState& game_state)
{
    using namespace Engine;
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

void print_board(Engine::EngineAPI& engine)
{
    using namespace Engine;
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
    using namespace Engine;
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

    std::string b;
    b = game_state.get_key();
    std::cout << b;
}

void test_engine_API()
{
    using namespace Engine;
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

void test_transposition_table()
{
    using namespace Engine;
    TranspositionTable tt;
    tt.set_beta_cutuff_move("123", 1);
    std::cout << tt.beta_cutoff_move_available("123") << std::endl;
    std::cout << tt.beta_cutoff_move_available("1234") << std::endl;
    std::cout << tt.get_beta_cutoff_move("123") << std::endl;
    tt.set_lower_bound("111222", 20);
    std::cout << tt.lower_bound_available("111222") << std::endl;
    std::cout << tt.lower_bound_available("1234") << std::endl;
    std::cout << tt.get_lower_bound("111222") << std::endl;

    GameState game_state;
    load_position(game_state, "01231234233");
    std::string key;
    key = game_state.get_key();
    tt.set_lower_bound(key, 1000);
    std::cout << tt.lower_bound_available(key) << std::endl;
    std::cout << tt.get_lower_bound(key) << std::endl;
    tt.reset();
    std::cout << tt.lower_bound_available("123") << std::endl;
    std::cout << tt.lower_bound_available("111222") << std::endl;
    std::cout << tt.lower_bound_available(key) << std::endl;
}

void engine_vs_engine(Engine::EngineAPI& engine1, TestEngineAPI& engine2, int number_of_games,
                bool display_move_times)
// Let engine1 and engine2 play against each other.
{
    using namespace Engine;
    int engine1_wins = 0;
    int engine2_wins = 0;
    int draws = 0;
    bool engine1_begin = true;
    int result;

    bool engine1_to_play = engine1_begin;
    int move;
    std::chrono::steady_clock::time_point t0;
    std::chrono::steady_clock::time_point t1;
    std::chrono::steady_clock::duration move_time;
    std::chrono::steady_clock::duration game_time_engine1 =
                 std::chrono::steady_clock::duration::zero();
    std::chrono::steady_clock::duration game_time_engine2 =
                 std::chrono::steady_clock::duration::zero();
    std::chrono::steady_clock::duration total_time_engine1 =
                 std::chrono::steady_clock::duration::zero();
    std::chrono::steady_clock::duration total_time_engine2 =
                 std::chrono::steady_clock::duration::zero();

    for (int n=1; n<=number_of_games; n++)
    {
        engine1.new_game();
        engine2.new_game();
        engine1_to_play = engine1_begin;
        game_time_engine1 = std::chrono::steady_clock::duration::zero();
        game_time_engine2 = std::chrono::steady_clock::duration::zero();

        while (true)
        {
            if (engine1_to_play)
            {
                t0 = std::chrono::steady_clock::now();
                move = engine1.engine_move();
                t1 = std::chrono::steady_clock::now();
                move_time = t1 - t0;
                if (display_move_times)
                {
                    std::cout << "Engine1 made a move. It took "
                    << std::chrono::duration_cast<std::chrono::microseconds>(move_time).count()
                    << " microseconds" << std::endl;
                }
                game_time_engine1 += move_time;
                engine1.make_move(move);
                engine2.make_move(move);
                if (engine1.four_in_a_row(move))
                {
                    std::cout << "Engine1 win. Engine1 time: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(game_time_engine1).count()
                    << " ms, Engine2 time: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(game_time_engine2).count()
                    << " ms" << std::endl;
                    engine1_wins++;
                    total_time_engine1 += game_time_engine1;
                    total_time_engine2 += game_time_engine2;
                    break;
                }
                if (engine1.board_full())
                {
                    std::cout << "Draw. Engine1 time: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(game_time_engine1).count()
                    << " ms, Engine2 time: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(game_time_engine2).count()
                    << " ms" << std::endl;
                    draws++;
                    total_time_engine1 += game_time_engine1;
                    total_time_engine2 += game_time_engine2;
                    break;
                }
            }
            else
            {
                t0 = std::chrono::steady_clock::now();
                move = engine2.engine_move();
                t1 = std::chrono::steady_clock::now();
                move_time = t1 - t0;
                if (display_move_times)
                {
                    std::cout << "Engine2 made a move. It took "
                    << std::chrono::duration_cast<std::chrono::microseconds>(move_time).count()
                    << " microseconds" << std::endl;
                }
                game_time_engine2 += move_time;
                engine1.make_move(move);
                engine2.make_move(move);
                if (engine1.four_in_a_row(move))
                {
                    std::cout << "Engine2 win. Engine1 time: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(game_time_engine1).count()
                    << " ms, Engine2 time: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(game_time_engine2).count()
                    << " ms" << std::endl;
                    engine2_wins++;
                    total_time_engine1 += game_time_engine1;
                    total_time_engine2 += game_time_engine2;
                    break;
                }
                if (engine1.board_full())
                {
                    std::cout << "Draw. Engine1 time: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(game_time_engine1).count()
                    << " ms, Engine2 time: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(game_time_engine2).count()
                    << " ms" << std::endl;
                    draws++;
                    total_time_engine1 += game_time_engine1;
                    total_time_engine2 += game_time_engine2;
                    break;
                }
            }
            engine1_to_play = not engine1_to_play;
        }

        engine1_begin = not engine1_begin;
    }

    std::cout << "Number of games: " << number_of_games << std::endl;
    std::cout << "Engine 1 wins: " << engine1_wins << std::endl;
    std::cout << "Engine 2 wins: " << engine2_wins << std::endl;
    std::cout << "Draws: " << draws << std::endl;
    std::cout << "Total Engine1 time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(total_time_engine1).count()
              << " ms" << std::endl;
    std::cout << "Total Engine2 time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(total_time_engine2).count()
              << " ms" << std::endl;
}

int main()
{
    std::srand(time(NULL)); // Initialize the random number generator.

    //test_game_state();
    //test_engine_API();
    //test_transposition_table();

    Engine::EngineAPI engine1;
    engine1.set_difficulty_level(3);
    TestEngineAPI engine2;
    engine2.set_difficulty_level(3);


    engine_vs_engine(engine1, engine2, 10, false);

    return 0;
}
