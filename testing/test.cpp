#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
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

    std::cout << "Test open_four_in_a_row_count" << std::endl;
    load_position(game_state, "2244550");
    print_board(game_state);
    std::cout << "Open four in a row count player 0: " <<
                  game_state.open_four_in_a_row_count(0) << std::endl;
    std::cout << "Open four in a row count player 1: " <<
                  game_state.open_four_in_a_row_count(1) << std::endl;

    load_position(game_state, "1115433452");
    print_board(game_state);
    std::cout << "Open four in a row count player 0: " <<
                  game_state.open_four_in_a_row_count(0) << std::endl;
    std::cout << "Open four in a row count player 1: " <<
                  game_state.open_four_in_a_row_count(1) << std::endl;

    load_position(game_state, "3323133");
    print_board(game_state);
    std::cout << "Open four in a row count player 0: " <<
                  game_state.open_four_in_a_row_count(0) << std::endl;
    std::cout << "Open four in a row count player 1: " <<
                  game_state.open_four_in_a_row_count(1) << std::endl;

    load_position(game_state, "225361134123");
    print_board(game_state);
    std::cout << "Open four in a row count player 0: " <<
                  game_state.open_four_in_a_row_count(0) << std::endl;
    std::cout << "Open four in a row count player 1: " <<
                  game_state.open_four_in_a_row_count(1) << std::endl;

    load_position(game_state, "523012231");
    print_board(game_state);
    std::cout << "Open four in a row count player 0: " <<
                  game_state.open_four_in_a_row_count(0) << std::endl;
    std::cout << "Open four in a row count player 1: " <<
                  game_state.open_four_in_a_row_count(1) << std::endl;

    load_position(game_state, "0045002523412413");
    print_board(game_state);
    std::cout << "Open four in a row count player 0: " <<
                  game_state.open_four_in_a_row_count(0) << std::endl;
    std::cout << "Open four in a row count player 1: " <<
                  game_state.open_four_in_a_row_count(1) << std::endl;

    std::cout << std::endl;
    std::cout << "Test keys" << std::endl;
    std::cout << std::endl;
    load_position(game_state, "0045002523412413");
    print_bitboard(game_state.get_key());
    std::cout << std::endl;
    print_bitboard(game_state.get_mirror_key());
    std::cout << std::endl;
    load_position(game_state, "0013444");
    print_bitboard(game_state.get_key());
    std::cout << std::endl;
    print_bitboard(game_state.get_mirror_key());
    std::cout << std::endl;
    load_position(game_state, "3");
    print_bitboard(game_state.get_key());
    std::cout << std::endl;
    print_bitboard(game_state.get_mirror_key());
    std::cout << std::endl;
    load_position(game_state, "11466");
    print_bitboard(game_state.get_key());
    std::cout << std::endl;
    print_bitboard(game_state.get_mirror_key());
    std::cout << std::endl;
    load_position(game_state, "55200");
    print_bitboard(game_state.get_key());

    std::cout << std::endl;
    std::cout << "Test own_threat_above" << std::endl;
    std::cout << std::endl;
    load_position(game_state, "11466");
    print_board(game_state);
    std::cout << game_state.own_threat_above(0) << std::endl;
    load_position(game_state, "222343344");
    print_board(game_state);
    std::cout << game_state.own_threat_above(0) << std::endl;
    std::cout << game_state.own_threat_above(1) << std::endl;
    std::cout << game_state.own_threat_above(2) << std::endl;
    std::cout << game_state.own_threat_above(3) << std::endl;
    std::cout << game_state.own_threat_above(4) << std::endl;
    std::cout << game_state.own_threat_above(5) << std::endl;
    std::cout << game_state.own_threat_above(6) << std::endl;
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

    std::cout << "Test opening book" << std::endl;
    engine.set_difficulty_level(3);
    load_position(engine, "000003130");
    print_board(engine);
    std::cout << engine.engine_move() << std::endl;
    load_position(engine, "000003130");
    print_board(engine);
    std::cout << engine.engine_move() << std::endl;
    load_position(engine, "000003130");
    print_board(engine);
    std::cout << engine.engine_move() << std::endl;
    load_position(engine, "000003130");
    print_board(engine);
    std::cout << engine.engine_move() << std::endl;
    std::cout << std::endl;
    std::cout << "Expected: 456" << std::endl;
    for(int n=0; n<=20; n++)
    {
        load_position(engine, "30314220");
        std::cout << engine.engine_move() << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Expected: 15" << std::endl;
    for(int n=0; n<=20; n++)
    {
        load_position(engine, "31153255");
        std::cout << engine.engine_move() << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Expected: 0134" << std::endl;
    for(int n=0; n<=20; n++)
    {
        load_position(engine, "33313612");
        std::cout << engine.engine_move() << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Expected: 035" << std::endl;
    for(int n=0; n<=20; n++)
    {
        load_position(engine, "33353022");
        std::cout << engine.engine_move() << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Expected: 4" << std::endl;
    for(int n=0; n<=20; n++)
    {
        load_position(engine, "3032335654");
        std::cout << engine.engine_move() << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Expected: 01456" << std::endl;
    for(int n=0; n<=20; n++)
    {
        load_position(engine, "010045110");
        std::cout << engine.engine_move() << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Expected: 12" << std::endl;
    for(int n=0; n<=20; n++)
    {
        load_position(engine, "3033334544");
        std::cout << engine.engine_move() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Test 33333 moves"<< std::endl;
    engine.set_difficulty_level(3);
    for(int i; i<100; i++)
    {
        load_position(engine, "33333");
        std::cout << engine.engine_move() << std::endl;
    }
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
    std::string move_sequence_string = "";

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
                    std::cout << number_of_moves << ". ";
                    std::cout << "Engine made move " << move << ".      It took "
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
                move_sequence_string += std::to_string(move);
                if (engine.four_in_a_row())
                {
                    std::cout << "Engine win. Engine time: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(game_time_engine).count()
                    << " ms. Test engine time: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(game_time_test_engine).count()
                    << " ms" << std::endl;
                    std::cout << "Number or moves: " << number_of_moves << std::endl;
                    if(engine_begin) {std::cout << "Engine made the first move. " << std::endl;}
                    else {std::cout << "Test engine made the first move. " << std::endl;}
                    std::cout << "Moves: " << move_sequence_string << std::endl;
                    move_sequence_string = "";
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
                    if(engine_begin) {std::cout << "Engine made the first move. " << std::endl;}
                    else {std::cout << "Test engine made the first move. " << std::endl;}
                    std::cout << "Moves: " << move_sequence_string << std::endl;
                    move_sequence_string = "";
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
                    std::cout << number_of_moves << ". ";
                    std::cout << "Test engine made move " << move << ". It took "
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
                move_sequence_string += std::to_string(move);
                if (engine.four_in_a_row())
                {
                    std::cout << "Test engine win. Engine time: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(game_time_engine).count()
                    << " ms. Test engine time: "
                    << std::chrono::duration_cast<std::chrono::milliseconds>(game_time_test_engine).count()
                    << " ms" << std::endl;
                    std::cout << "Number or moves: " << number_of_moves << std::endl;
                    if(engine_begin) {std::cout << "Engine made the first move. " << std::endl;}
                    else {std::cout << "Test engine made the first move. " << std::endl;}
                    std::cout << "Moves: " << move_sequence_string << std::endl;
                    move_sequence_string = "";
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
                    if(engine_begin) {std::cout << "Engine made the first move. " << std::endl;}
                    else {std::cout << "Test engine made the first move. " << std::endl;}
                    std::cout << "Moves: " << move_sequence_string << std::endl;
                    move_sequence_string = "";
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
    }std::chrono::steady_clock::duration::zero();
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
    test_position(engine, "351524", 0);
    test_position(engine, "443555541", 2);
//    test_position(engine, "256555226", 6); // slow
    test_position(engine, "333345550", 3); // 1 and 2 are as good.
    test_position(engine, "333333010", 0);
    test_position(engine, "334233650026", 5); // A draw
    test_position(engine, "01234560660", 3);
    test_position(engine, "00000055551", 2);
    test_position(engine, "2222223456", 3);
    test_position(engine, "333333561", 0);
    test_position(engine, "33423365002630", 1);
    test_position(engine, "3000011243563", 3);
    test_position(engine, "166553001163", 4);
}

void benchmark_position_values(Engine::EngineAPI& engine)
{
    test_position_value(engine, "333033112", 1);
    test_position_value(engine, "333633554", 1);
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
    test_position_value(engine, "166553001163", -5); 
    test_position_value(engine, "333330211113655", -2);
}

void benchmark_position_values_no_opening_book(Engine::EngineAPI& engine)
{
//    test_position_value(engine, "3366455", 0, false);
//    test_position_value(engine, "336645", 0, false);
//    test_position_value(engine, "334232", -5, false);
//    test_position_value(engine, "3342", -3, false);
//    test_position_value(engine, "3342000", 7, false);
//    test_position_value(engine, "333", -2, false);
//    test_position_value(engine, "33", 2, false);
////    test_position_value(engine, "3", -2, false); // The program crashes with this.
//    test_position_value(engine, "00343", -4, false);
//    test_position_value(engine, "3563", -3, false);
}

void opening_test()
{
    Engine::EngineAPI engine_1;
    engine_1.set_difficulty_level(4);
    Engine::EngineAPI engine_2;
    engine_2.set_difficulty_level(3);

    for(int k=1; k<=1000; k++)
    {
        engine_1.new_game();
        engine_2.new_game();
        int c = 0;
        while(true)
        {
            int move = engine_1.engine_move();
            engine_1.make_move(move);
            engine_2.make_move(move);
            if (engine_1.four_in_a_row())
            {
                break;
            }
            c++;
            if(c == 9) {break;}
            move = engine_2.engine_move();
            engine_1.make_move(move);
            engine_2.make_move(move);
            if (engine_2.four_in_a_row())
            {
                break;
            }
            c++;
            if(c == 9) {break;}
        }
        print_board(engine_1);
    }
}

void test_from_file_values(std::string file_name, bool detailed_output = false)
/* Test the engine by comparing it's output from moves in a text file. The file should have one line
per transpositon. Each line should first have a transposition described as a move string followed
by a space and then a value.*/
{
    std::ifstream file_to_read(file_name);
    std::string line, move, c;
    uint64_t key;
    Engine::EngineAPI engine;
    std::chrono::steady_clock::time_point t0;
    std::chrono::steady_clock::time_point t1;
    std::chrono::steady_clock::duration move_time;
    std::chrono::steady_clock::duration total_time =
                 std::chrono::steady_clock::duration::zero();

    if(not file_to_read.is_open())
    {
        std::cerr << "Can't open " << file_name << std::endl;
        return;
    }

    std::cout << "Testing " << file_name << std::endl;
    if(detailed_output)
    {
        std::cout << std::endl << "********************************************************" << std::endl;
    }

    while (std::getline(file_to_read, line))
    {
        // Load position.
        engine.new_game();
        int space_index;
        std::string move_string = "";
        for (int n=0; n<line.length(); n++)
        {
            if (line[n] == ' ')
            {
                space_index = n;
                break;
            }
            move = line[n];
            engine.make_move(std::stoi(move));
            move_string.append(move);
        }

        // Find value.
        std::string value_string = "";
        for (int n=space_index+1; n<line.length(); n++)
        {
            c = line[n];
            value_string.append(c);
        }

        if(detailed_output)
        {
            print_board(engine);
            std::cout << "Position: " <<  move_string << std::endl;
        }

        int expected_value = std::stoi(value_string);
        t0 = std::chrono::steady_clock::now();
        bool use_opening_book = true;
        int value = engine.position_value_full_depth(use_opening_book);
        t1 = std::chrono::steady_clock::now();
        move_time = t1 - t0;
        total_time += move_time;

        if(detailed_output or value != expected_value)
        {
            std::cout << "Value: " << value;
            std::cout << ", Expected value: " <<  expected_value;
            std::cout << ", Time: "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(move_time).count()
                      << " ms" << std::endl;
            if (value == expected_value)
            {
                std::cout << "Test successful!" << std::endl;
                std::cout << std::endl << "********************************************************" << std::endl;
            }
            else
            {
                std::cout << "Test failed!" << std::endl << std::endl;
                std::cout << "The testing was interrupted." << std::endl << std::endl;
                return;
            }
        }
    }
    if(detailed_output)
    {
        std::cout << std::endl;
    }
    std::cout << "All tests were successful!" << std::endl << std::endl;
    std::cout << "Total time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(total_time).count()
              << " ms" << std::endl << std::endl;
    file_to_read.close();
}

void test_from_file_best_moves(std::string file_name, bool detailed_output = false)
/* Test the engine by comparing it's output from moves in a text file. The file should have one line
per transpositon. Each line should first have a transposition described as a move string followed
by a space and then a string with best moves.*/
{
    std::ifstream file_to_read(file_name);
    std::string line, move, c;
    uint64_t key;
    Engine::EngineAPI engine;
    engine.set_difficulty_level(3);
    std::chrono::steady_clock::time_point t0;
    std::chrono::steady_clock::time_point t1;
    std::chrono::steady_clock::duration move_time;
    std::chrono::steady_clock::duration total_time =
                 std::chrono::steady_clock::duration::zero();

    if(not file_to_read.is_open())
    {
        std::cerr << "Can't open " << file_name << std::endl;
        return;
    }

    std::cout << "Testing " << file_name << std::endl;
    if(detailed_output)
    {
        std::cout << std::endl << "********************************************************" << std::endl;
    }

    while (std::getline(file_to_read, line))
    {
        // Load position.
        engine.new_game();
        int space_index;
        std::string move_string = "";
        for (int n=0; n<line.length(); n++)
        {
            if (line[n] == ' ')
            {
                space_index = n;
                break;
            }
            move = line[n];
            engine.make_move(std::stoi(move));
            move_string.append(move);
        }

        // Find moves.
        std::string best_move_string = "";
        std::vector<int> best_moves;
        for (int n=space_index+1; n<line.length(); n++)
        {
            move = line[n];
            best_move_string.append(move);
            best_moves.push_back(std::stoi(move));
        }

        if(detailed_output)
        {
            print_board(engine);
            std::cout << "Position: " <<  move_string << std::endl;
        }

        t0 = std::chrono::steady_clock::now();
        bool use_opening_book = true;
        int engine_move = engine.engine_move();
        t1 = std::chrono::steady_clock::now();
        move_time = t1 - t0;
        total_time += move_time;

        bool found_move = false;
        for(int move : best_moves)
        {
            if(engine_move == move)
            {
                found_move = true;
            }
        }

        if(detailed_output or not found_move)
        {
            std::cout << "Move: " << engine_move;
            std::cout << ", Expected moves: " <<  best_move_string;
            std::cout << ", Time: "
                      << std::chrono::duration_cast<std::chrono::milliseconds>(move_time).count()
                      << " ms" << std::endl;
            if (found_move)
            {
                std::cout << "Test successful!" << std::endl;
                std::cout << std::endl << "********************************************************" << std::endl;
            }
            else
            {
                std::cout << "Test failed!" << std::endl << std::endl;
                std::cout << "The testing was interrupted." << std::endl << std::endl;
                return;
            }
        }
    }
    if(detailed_output)
    {
        std::cout << std::endl;
    }
    std::cout << "All tests were successful!" << std::endl << std::endl;
    std::cout << "Total time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(total_time).count()
              << " ms" << std::endl << std::endl;
    file_to_read.close();
}

int main()
{
    std::srand(time(NULL)); // Initialize the random number generator.

//    test_game_state();
//    test_engine_API();
//    opening_test();

//    test_from_file_values("./testing/test_transpositions/large.values", true);
//    test_from_file_values("./testing/test_transpositions/medium.values", true);
//    test_from_file_values("./testing/test_transpositions/small.values", true);
//    test_from_file_values("./testing/test_transpositions/speed_test.values", true);

//    test_from_file_best_moves("./testing/test_transpositions/large.best_moves", true);
//    test_from_file_best_moves("./testing/test_transpositions/medium.best_moves", true);
//    test_from_file_best_moves("./testing/test_transpositions/small.best_moves", true);
//   test_from_file_best_moves("./testing/test_transpositions/speed_test.best_moves", true);

    Engine::EngineAPI engine(9126);
    engine.set_difficulty_level(3);
    TestEngine::EngineAPI test_engine(2541154);
    test_engine.set_difficulty_level(2);

//    Engine::EngineAPI engine;
//    engine.set_difficulty_level(3);
//    TestEngine::EngineAPI test_engine;
//    test_engine.set_difficulty_level(3);

    engine_vs_engine(engine, test_engine, 50, false);

    return 0;
}
