#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>
#include <chrono>
#include "../game_state.h"
#include "../engine_API.h"
#include "test_engine_API.h"

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

void print_bitboard(uint64_t bitboard)
{
    uint64_t one = 1;
    for (int row=6; row>=0; row--)
    {
        for (int col=0; col<9; col++)
        {
            int bit = row + col * 7;
            std::cout << ((bitboard & (one << bit)) != 0);
        }
        if (row > 0)
        {
            std::cout << std::endl;
        }
    }
    std::cout << ((bitboard & (one << 63)) != 0);
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
    std::cout << game_state.four_in_a_row() << std::endl;
    load_position(game_state, "0102030");
    print_board(game_state);
    std::cout << game_state.four_in_a_row() << std::endl;
    load_position(game_state, "01020364");
    print_board(game_state);
    std::cout << game_state.four_in_a_row() << std::endl;
    std::cout << game_state.four_in_a_row() << std::endl;
    std::cout << game_state.four_in_a_row() << std::endl;
    std::cout << game_state.four_in_a_row() << std::endl;

    load_position(game_state, "01231234233");
    print_board(game_state);
    std::cout << game_state.four_in_a_row() << std::endl;
    std::cout << game_state.four_in_a_row() << std::endl;
    std::cout << game_state.four_in_a_row() << std::endl;
    std::cout << game_state.four_in_a_row() << std::endl;
    std::cout << game_state.four_in_a_row() << std::endl;

    load_position(game_state, "65435430433");
    print_board(game_state);
    std::cout << game_state.four_in_a_row() << std::endl;
    std::cout << game_state.four_in_a_row() << std::endl;
    std::cout << game_state.four_in_a_row() << std::endl;
    std::cout << game_state.four_in_a_row() << std::endl;
    std::cout << game_state.four_in_a_row() << std::endl;
    std::cout << std::endl;
    std::cout << "Test of get_number_of_disks_in_column:" << std::endl;
    std::cout << game_state.get_number_of_disks_in_column(0) << std::endl;
    std::cout << game_state.get_number_of_disks_in_column(1) << std::endl;
    std::cout << game_state.get_number_of_disks_in_column(2) << std::endl;
    std::cout << game_state.get_number_of_disks_in_column(3) << std::endl;
    std::cout << game_state.get_number_of_disks_in_column(4) << std::endl;
    std::cout << game_state.get_number_of_disks_in_column(5) << std::endl;
    std::cout << game_state.get_number_of_disks_in_column(6) << std::endl;

    std::cout << "Test is_blocking_move:" << std::endl;
    load_position(game_state, "30303");
    print_board(game_state);
    for(int i=0; i<=6; i++)
    {
        std::cout << game_state.is_blocking_move(i) << std::endl;
    }
    load_position(game_state, "030405");
    print_board(game_state);
    for(int i=0; i<=6; i++)
    {
        std::cout << game_state.is_blocking_move(i) << std::endl;
    }

    std::cout << "Test can_win_this_move:" << std::endl;
    load_position(game_state, "332243");
    print_board(game_state);
    std::cout << game_state.can_win_this_move() << std::endl;
    print_board(game_state);

    std::cout << "Test four_in_a_row(int player, int column, int row)" << std::endl;
    load_position(game_state, "33224331");
    print_board(game_state);
    std::cout << game_state.four_in_a_row(0, 5, 0) << std::endl;
    std::cout << game_state.four_in_a_row(1, 4, 3) << std::endl;

    std::cout << "Test opponent_four_in_a_row_above(int column)" << std::endl;
    load_position(game_state, "33224331");
    print_board(game_state);
    std::cout << "Column 3: " << game_state.opponent_four_in_a_row_above(3) << std::endl;
    load_position(game_state, "332243313");
    print_board(game_state);
    std::cout << "Column 3: " << game_state.opponent_four_in_a_row_above(3) << std::endl;
    load_position(game_state, "3322433133");
    print_board(game_state);
    std::cout << "Column 3: " << game_state.opponent_four_in_a_row_above(3) << std::endl;
    load_position(game_state, "334455");
    print_board(game_state);
    std::cout << "Column 6: " << game_state.opponent_four_in_a_row_above(6) << std::endl;
    load_position(game_state, "123423463");
    print_board(game_state);
    std::cout << "Column 2: " << game_state.opponent_four_in_a_row_above(2) << std::endl;
    std::cout << "Column 3: " << game_state.opponent_four_in_a_row_above(3) << std::endl;
    std::cout << "Column 4: " << game_state.opponent_four_in_a_row_above(4) << std::endl;

    load_position(game_state, "334");
    print_board(game_state);
    int blocking_move;
    int number_of_blocking_moves = 0;
    for (int move=0; move<=6; move++)
    {
        if (game_state.column_not_full(move))
        {
            if(game_state.is_blocking_move(move))
            {
                blocking_move = move;
                number_of_blocking_moves++;
                std::cout << move << std::endl;
            }
        }
    }
    std::cout << number_of_blocking_moves << std::endl;

    load_position(game_state, "33435");
    print_board(game_state);
    number_of_blocking_moves = 0;
    for (int move=0; move<=6; move++)
    {
        if (game_state.column_not_full(move))
        {
            if(game_state.is_blocking_move(move))
            {
                blocking_move = move;
                number_of_blocking_moves++;
                std::cout << move << std::endl;
            }
        }
    }
    std::cout << number_of_blocking_moves << std::endl;

    std::cout << std::endl;
    std::cout << "Test print_bitboard" << std::endl;
    std::cout << std::endl;
    print_bitboard(0b0111111011111101111110111111011111101111110111111);
    std::cout << std::endl;
    print_bitboard(0b1011111101111110111111011111101111110111111011111101111110111111);
    std::cout << std::endl;
    print_bitboard(0b1011111101111110111111011111101111110111111000111101111110101011);

    std::cout << std::endl;
    std::cout << "Test get_opponent_winning_positions_bitboard()" << std::endl;

    load_position(game_state, "33435");
    std::cout << (game_state.get_number_of_moves() % 2) + 1 << " to play:"<< std::endl;
    print_board(game_state);
    print_bitboard(game_state.get_opponent_winning_positions_bitboard());
    std::array<bool,7> non_losing_moves = game_state.get_non_losing_moves();
    std::cout << "Non losing moves: ";
    for(int i=0; i<=6; i++)
    {
        std::cout << non_losing_moves[i];
    }
    std::cout << std::endl;
    std::cout << std::endl;

    load_position(game_state, "01010");
    std::cout << (game_state.get_number_of_moves() % 2) + 1 << " to play:"<< std::endl;
    print_board(game_state);
    print_bitboard(game_state.get_opponent_winning_positions_bitboard());
    non_losing_moves = game_state.get_non_losing_moves();
    std::cout << "Non losing moves: ";
    for(int i=0; i<=6; i++)
    {
        std::cout << non_losing_moves[i];
    }
    std::cout << std::endl;
    std::cout << std::endl;

    load_position(game_state, "23340445505");
    std::cout << (game_state.get_number_of_moves() % 2) + 1 << " to play:"<< std::endl;
    print_board(game_state);
    print_bitboard(game_state.get_opponent_winning_positions_bitboard());
    non_losing_moves = game_state.get_non_losing_moves();
    std::cout << "Non losing moves: ";
    for(int i=0; i<=6; i++)
    {
        std::cout << non_losing_moves[i];
    }
    std::cout << std::endl;
    std::cout << std::endl;

    load_position(game_state, "233404455050");
    std::cout << (game_state.get_number_of_moves() % 2) + 1 << " to play:"<< std::endl;
    print_board(game_state);
    print_bitboard(game_state.get_opponent_winning_positions_bitboard());
    non_losing_moves = game_state.get_non_losing_moves();
    std::cout << "Non losing moves: ";
    for(int i=0; i<=6; i++)
    {
        std::cout << non_losing_moves[i];
    }
    std::cout << std::endl;
    std::cout << std::endl;

    load_position(game_state, "0000101122");
    std::cout << (game_state.get_number_of_moves() % 2) + 1 << " to play:"<< std::endl;
    print_board(game_state);
    print_bitboard(game_state.get_opponent_winning_positions_bitboard());
    non_losing_moves = game_state.get_non_losing_moves();
    std::cout << "Non losing moves: ";
    for(int i=0; i<=6; i++)
    {
        std::cout << non_losing_moves[i];
    }
    std::cout << std::endl;
    std::cout << std::endl;

    load_position(game_state, "0011333");
    std::cout << (game_state.get_number_of_moves() % 2) + 1 << " to play:"<< std::endl;
    print_board(game_state);
    print_bitboard(game_state.get_opponent_winning_positions_bitboard());
    non_losing_moves = game_state.get_non_losing_moves();
    std::cout << "Non losing moves: ";
    for(int i=0; i<=6; i++)
    {
        std::cout << non_losing_moves[i];
    }
    std::cout << std::endl;
    std::cout << std::endl;

    load_position(game_state, "2244550");
    std::cout << (game_state.get_number_of_moves() % 2) + 1 << " to play:"<< std::endl;
    print_board(game_state);
    print_bitboard(game_state.get_opponent_winning_positions_bitboard());
    non_losing_moves = game_state.get_non_losing_moves();
    std::cout << "Non losing moves: ";
    for(int i=0; i<=6; i++)
    {
        std::cout << non_losing_moves[i];
    }
    std::cout << std::endl;
    std::cout << std::endl;
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
    std::cout << "Four in a row: " << engine.four_in_a_row() << std::endl;
    std::cout << "Board full: " << engine.board_full() << std::endl;
    std::cout << "Legal move 4: " << engine.legal_move(4) << std::endl;
    load_position(engine, "000111222333444555666000111222333444555666");
    print_board(engine);
    std::cout << "Legal move 4: " << engine.legal_move(4) << std::endl;
    std::cout << "Board full: " << engine.board_full() << std::endl;
    load_position(engine, "01231234233");
    print_board(engine);
    std::cout << "Four in a row: " << engine.four_in_a_row() << std::endl;
    load_position(engine, "3344");
    print_board(engine);
    std::cout << "Engine move: " << engine.engine_move() << std::endl;
    load_position(engine, "334455");
    print_board(engine);
    std::cout << "Engine move: " << engine.engine_move() << std::endl;
//    std::cout << engine.heuristic_value(0) << std::endl;
//    std::cout << engine.heuristic_value(1) << std::endl;
//    std::cout << engine.heuristic_value(2) << std::endl;
//    std::cout << engine.heuristic_value(3) << std::endl;
//    std::cout << engine.heuristic_value(4) << std::endl;
//    std::cout << engine.heuristic_value(5) << std::endl;
//    std::cout << engine.heuristic_value(6) << std::endl;
//    load_position(engine, "000000111113334456666");
//    print_board(engine);
//    std::cout << engine.heuristic_value(0) << std::endl;
//    std::cout << engine.heuristic_value(1) << std::endl;
//    std::cout << engine.heuristic_value(2) << std::endl;
//    std::cout << engine.heuristic_value(3) << std::endl;
//    std::cout << engine.heuristic_value(4) << std::endl;
//    std::cout << engine.heuristic_value(5) << std::endl;
//    std::cout << engine.heuristic_value(6) << std::endl;

//    load_position(engine, "33224331");
//    print_board(engine);
//    std::cout << engine.open_four_in_a_row_heuristic(0) << std::endl;
//    std::cout << engine.open_four_in_a_row_heuristic(1) << std::endl;
//    load_position(engine, "332243310");
//    print_board(engine);
//    std::cout << engine.open_four_in_a_row_heuristic(0) << std::endl;
//    std::cout << engine.open_four_in_a_row_heuristic(4) << std::endl;

    load_position(engine, "33224331");
    print_board(engine);
    std::cout << "Depth 42: " << engine.position_value_full_depth() << std::endl;

    load_position(engine, "044502445224");
    print_board(engine);
    std::cout << "Depth 42: " << engine.position_value_full_depth() << std::endl;

    load_position(engine, "33"); 
    print_board(engine);
    std::cout << "Depth 42: " << engine.position_value_full_depth() << std::endl;
}

void test_transposition_table()
{
//    using namespace Engine;
//    TranspositionTable tt;
//    tt.set_beta_cutuff_move("123", 1);
//    std::cout << tt.beta_cutoff_move_available("123") << std::endl;
//    std::cout << tt.beta_cutoff_move_available("1234") << std::endl;
//    std::cout << tt.get_beta_cutoff_move("123") << std::endl;
//    tt.set_lower_bound("111222", 20);
//    std::cout << tt.lower_bound_available("111222") << std::endl;
//    std::cout << tt.lower_bound_available("1234") << std::endl;
//    std::cout << tt.get_lower_bound("111222") << std::endl;

//    GameState game_state;
//    load_position(game_state, "01231234233");
//    std::string key;
//    key = game_state.get_key();
//    tt.set_lower_bound(key, 1000);
//    std::cout << tt.lower_bound_available(key) << std::endl;
//    std::cout << tt.get_lower_bound(key) << std::endl;
//    tt.reset();
//    std::cout << tt.lower_bound_available("123") << std::endl;
//    std::cout << tt.lower_bound_available("111222") << std::endl;
//    std::cout << tt.lower_bound_available(key) << std::endl;
}

void test_position(Engine::EngineAPI& engine, std::string move_string, int expected_move)
{
    std::chrono::steady_clock::time_point t0;
    std::chrono::steady_clock::time_point t1;
    std::chrono::steady_clock::duration move_time;
    load_position(engine, move_string);
    t0 = std::chrono::steady_clock::now();
    int move = engine.engine_move();
    t1 = std::chrono::steady_clock::now();
    move_time = t1 - t0;
    std::cout << "Position: " <<  move_string << std::endl;
    std::cout << "Engine move: " <<  move;
    std::cout << ", Expected move: " <<  expected_move;
    std::cout << ", Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(move_time).count()
              << " ms" << std::endl;
    if (move == expected_move)
    {
        std::cout << "Test successful!" << std::endl;
    }
    else
    {
        std::cout << "Test failed!" << std::endl;
    }
    std::cout << std::endl;
}

void test_position_value(Engine::EngineAPI& engine, std::string move_string,
                         int expected_value, const int use_opening_book=true)
{
    std::chrono::steady_clock::time_point t0;
    std::chrono::steady_clock::time_point t1;
    std::chrono::steady_clock::duration move_time;
    load_position(engine, move_string);
    t0 = std::chrono::steady_clock::now();
    int value = engine.position_value_full_depth(use_opening_book);
    t1 = std::chrono::steady_clock::now();
    move_time = t1 - t0;
    std::cout << "Position: " <<  move_string << std::endl;
    std::cout << "Value: " << value;
    std::cout << ", Expected value: " <<  expected_value;
    std::cout << ", Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(move_time).count()
              << " ms" << std::endl;
    if (value == expected_value)
    {
        std::cout << "Test successful!" << std::endl;
    }
    else
    {
        std::cout << "Test failed!" << std::endl;
    }
    std::cout << std::endl;
}

void engine_vs_engine(Engine::EngineAPI& engine, TestEngine::EngineAPI& test_engine, int number_of_games,
                bool display_move_times)
// Let engine and test_engine play against each other.
{
    using namespace Engine;
    int engine_wins = 0;
    int test_engine_wins = 0;
    int draws = 0;
    bool engine_begin = true;
    int result;
    int number_of_moves;

    bool engine_to_play = engine_begin;
    int move;
    std::chrono::steady_clock::time_point t0;
    std::chrono::steady_clock::time_point t1;
    std::chrono::steady_clock::duration move_time;
    std::chrono::steady_clock::duration game_time_engine =
                 std::chrono::steady_clock::duration::zero();
    std::chrono::steady_clock::duration game_time_test_engine =
                 std::chrono::steady_clock::duration::zero();
    std::chrono::steady_clock::duration total_time_engine =
                 std::chrono::steady_clock::duration::zero();
    std::chrono::steady_clock::duration total_time_test_engine =
                 std::chrono::steady_clock::duration::zero();
    std::chrono::steady_clock::duration max_move_time_engine =
                 std::chrono::steady_clock::duration::zero();
    std::chrono::steady_clock::duration max_move_time_test_engine =
                 std::chrono::steady_clock::duration::zero();

    for (int n=1; n<=number_of_games; n++)
    {
        engine.new_game();
        test_engine.new_game();
        engine_to_play = engine_begin;
        number_of_moves = 0;
        game_time_engine = std::chrono::steady_clock::duration::zero();
        game_time_test_engine = std::chrono::steady_clock::duration::zero();
        std::cout << std::endl << "Game " << n << ":" << std::endl;

        while (true)
        {
            number_of_moves ++;
            if (engine_to_play)
            {
                t0 = std::chrono::steady_clock::now();
                move = engine.engine_move();
                t1 = std::chrono::steady_clock::now();
                move_time = t1 - t0;
                if (display_move_times)
                {
                    std::cout << "Engine made a move.      It took "
                    << std::chrono::duration_cast<std::chrono::microseconds>(move_time).count()
                    << " microseconds" << std::endl;
                }
                if (move_time > max_move_time_engine)
                {
                    max_move_time_engine = move_time;
                }
                game_time_engine += move_time;
                engine.make_move(move);
                test_engine.make_move(move);
                if (engine.four_in_a_row())
                {
                    std::cout << "Engine win. Engine time: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(game_time_engine).count()
                    << " ms. Test engine time: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(game_time_test_engine).count()
                    << " ms" << std::endl;
                    std::cout << "Number or moves: " << number_of_moves << std::endl;
                    engine_wins++;
                    total_time_engine += game_time_engine;
                    total_time_test_engine += game_time_test_engine;
                    break;
                }
                if (engine.board_full())
                {
                    std::cout << "Draw. Engine time: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(game_time_engine).count()
                    << " ms. Test engine time: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(game_time_test_engine).count()
                    << " ms" << std::endl;
                    std::cout << "Number or moves: " << number_of_moves << std::endl;
                    draws++;
                    total_time_engine += game_time_engine;
                    total_time_test_engine += game_time_test_engine;
                    break;
                }
            }
            else
            {
                t0 = std::chrono::steady_clock::now();
                move = test_engine.engine_move();
                t1 = std::chrono::steady_clock::now();
                move_time = t1 - t0;
                if (display_move_times)
                {
                    std::cout << "Test engine made a move. It took "
                    << std::chrono::duration_cast<std::chrono::microseconds>(move_time).count()
                    << " microseconds" << std::endl;
                }
                if (move_time > max_move_time_test_engine)
                {
                    max_move_time_test_engine = move_time;
                }
                game_time_test_engine += move_time;
                engine.make_move(move);
                test_engine.make_move(move);
                if (engine.four_in_a_row())
                {
                    std::cout << "Test engine win. Engine time: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(game_time_engine).count()
                    << " ms. Test engine time: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(game_time_test_engine).count()
                    << " ms" << std::endl;
                    std::cout << "Number or moves: " << number_of_moves << std::endl;
                    test_engine_wins++;
                    total_time_engine += game_time_engine;
                    total_time_test_engine += game_time_test_engine;
                    break;
                }
                if (engine.board_full())
                {
                    std::cout << "Draw. Engine time: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(game_time_engine).count()
                    << " ms. Test_engine time: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(game_time_test_engine).count()
                    << " ms" << std::endl;
                    std::cout << "Number or moves: " << number_of_moves << std::endl;
                    draws++;
                    total_time_engine += game_time_engine;
                    total_time_test_engine += game_time_test_engine;
                    break;
                }
            }
            engine_to_play = not engine_to_play;
        }
        if (display_move_times) {std::cout << std::endl;}
        engine_begin = not engine_begin;
    }
    if (not display_move_times) {std::cout << std::endl;}
    std::cout << "Number of games: " << number_of_games << std::endl;
    std::cout << "Engine wins: " << engine_wins << std::endl;
    std::cout << "Test engine wins: " << test_engine_wins << std::endl;
    std::cout << "Draws: " << draws << std::endl;
    std::cout << "Max Engine move time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(max_move_time_engine).count()
              << " ms" << std::endl;
    std::cout << "Total Engine time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(total_time_engine).count()
              << " ms" << std::endl;
    std::cout << "Max Test engine move time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(max_move_time_test_engine).count()
              << " ms" << std::endl;
    std::cout << "Total Test engine time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(total_time_test_engine).count()
              << " ms" << std::endl;
}

void benchmark(Engine::EngineAPI& engine)
{
//    test_position(engine, "256555226", 6); Extremely show and have high memory usage.
    test_position(engine, "333345550", 3); //1 and 2 are as good.
    test_position(engine, "333333010", 0);
    test_position(engine, "334233650026", 5);
    test_position(engine, "01234560660", 3);
    test_position(engine, "00000055551", 2);
    test_position(engine, "2222223456", 3);
    test_position(engine, "333333561", 0);
    test_position(engine, "33423365002630", 1);
    test_position(engine, "3000011243563", 3);
}

void benchmark_position_values(Engine::EngineAPI& engine)
{
    test_position_value(engine, "333033112", 1);
    test_position_value(engine, "155233161", -6);
    test_position_value(engine, "002230532", -32);
    test_position_value(engine, "242222334", 7);
    test_position_value(engine, "3366455", 0);
    test_position_value(engine, "336645", 0);
    test_position_value(engine, "33664", 0);
    test_position_value(engine, "2543", 0);
    test_position_value(engine, "300", 0);
    test_position_value(engine, "", 2);
    test_position_value(engine, "0", 3);
    test_position_value(engine, "1", 1);
    test_position_value(engine, "2", 0);
    test_position_value(engine, "3", -2);
    test_position_value(engine, "33432", -36);
    test_position_value(engine, "3343", 36);
    test_position_value(engine, "33423365002", 0);
    test_position_value(engine, "05216116610", 5);
//    test_position_value(engine, "3366455", 0, false);
}

int main()
{
    std::srand(time(NULL)); // Initialize the random number generator.

//    test_game_state();
//    test_engine_API();

    Engine::EngineAPI engine(91635);
    engine.set_difficulty_level(3);
    TestEngine::EngineAPI test_engine(35790);
    test_engine.set_difficulty_level(3);

//    Engine::EngineAPI engine;
//    engine.set_difficulty_level(3);
//    TestEngine::EngineAPI test_engine;
//    test_engine.set_difficulty_level(3);

//    benchmark(engine);
//    benchmark_position_values(engine);
    engine_vs_engine(engine, test_engine, 10, false);

    return 0;
}
