#include "test_game_state.h"

namespace TestEngine
{

GameState::GameState()
{
    bitboard_1 = 0;
    bitboard_2 = 0;
    for (int n=0; n<=6; n++)
        column_height[n] = 0;
    number_of_moves = 0;
    player_1_in_turn = true;
}

void GameState::reset()
{
    bitboard_1 = 0;
    bitboard_2 = 0;
    for (int n=0; n<=6; n++)
        column_height[n] = 0;
    number_of_moves = 0;
    player_1_in_turn = true;
}

char GameState::get_value(int column, int row) const
{
    const uint64_t a = 1;
    if ((bitboard_1 >> (column * 7 + row)) & a == 1)
    {
        return '1';
    }
    if ((bitboard_2 >> (column * 7 + row)) & a == 1)
    {
        return '2';
    }
    return '0';
}

bool GameState::column_not_full(int column) const
{
    return column_height[column] < 6;
}

int GameState::get_number_of_disks_in_column(int column) const
{
    return column_height[column];
}

void GameState::make_move(int column)
{
    uint64_t a = 1;
    a <<= column * 7 + column_height[column];

    if (player_1_in_turn)
    {
        bitboard_1 |= a;
    }
    else
    {
        bitboard_2 |= a;
    }

    column_height[column]++;
    number_of_moves++;
    player_1_in_turn = not player_1_in_turn;
}

void GameState::undo_move(int column)
{
    column_height[column]--;
    number_of_moves--;
    player_1_in_turn = not player_1_in_turn;
    uint64_t a = 1;
    a <<= column * 7 + column_height[column];

    if (player_1_in_turn)
    {
        bitboard_1 ^= a;
    }
    else
    {
        bitboard_2 ^= a;
    }
}

bool GameState::four_in_a_row() const
{
    uint64_t bitboard;

    if (player_1_in_turn)
    {
        bitboard = bitboard_2;
    }
    else
    {
        bitboard = bitboard_1;
    }

    return four_in_a_row(bitboard);
}

bool GameState::four_in_a_row(uint64_t bitboard) const
{
    // Looking for four in a rows is done in two steps. The first step produces
    // a bitboard with a three in a row if there is a four in a row. The second step
    // checks if there exist points that are two steps distant from each other.

    uint64_t a;
    const int shifts[4] = {6, 8, 1, 7};

    for (int n=0; n<=3; n++)
    {
        a = (bitboard << shifts[n]) & bitboard;
        if (a & (a << (shifts[n] * 2))) {return true;}
    }

    return false;
}

bool GameState::can_win_this_move() const
{
    uint64_t bitboard;
    uint64_t new_bitboard;
    const uint64_t one = 1;

    if (player_1_in_turn)
    {
        bitboard = bitboard_1;
    }
    else
    {
        bitboard = bitboard_2;
    }

    const int ch0 = column_height[0];
    const int ch1 = column_height[1];
    const int ch2 = column_height[2];
    const int ch3 = column_height[3];
    const int ch4 = column_height[4];
    const int ch5 = column_height[5];
    const int ch6 = column_height[6];

//    // Try making moves in columns 0 and 4.
//    if (ch0 < 6 or ch4 < 6)
//    {
//        new_bitboard = bitboard;
//        if (ch0 < 6) {new_bitboard |= (one << ch0);}
//        if (ch4 < 6) {new_bitboard |= (one << 28 + ch4);}
//        if (four_in_a_row(new_bitboard)) {return true;}
//    }

//    if (ch0 < 6)
//    {
//        new_bitboard = bitboard | (one << ch0);
//        if (ch4 < 6)
//        {
//            new_bitboard |= (one << 28 + ch4);
//            if (four_in_a_row(new_bitboard)) {return true;}
//        }
//    }
//    else if (ch4 < 6)
//    {
//        new_bitboard = bitboard | (one << 28 + ch4);
//        if (four_in_a_row(new_bitboard)) {return true;}
//    }

//    // Try making moves in columns 1 and 5.
//    if (ch1 < 6 or ch5 < 6)
//    {
//        new_bitboard = bitboard;
//        if (ch1 < 6) {new_bitboard |= (one << 7 + ch1);}
//        if (ch5 < 6) {new_bitboard |= (one << 35 + ch5);}
//        if (four_in_a_row(new_bitboard)) {return true;}
//    }

//    // Try making moves in columns 2 and 6.
//    if (ch2 < 6 or ch6 < 6)
//    {
//        new_bitboard = bitboard;
//        if (ch2 < 6) {new_bitboard |= (one << 14 + ch2);}
//        if (ch6 < 6) {new_bitboard |= (one << 42 + ch6);}
//        if (four_in_a_row(new_bitboard)) {return true;}
//    }

//    // Try making a move in column 3.
//    if (ch3 < 6)
//    {
//        new_bitboard = bitboard | (one << 21 + ch3);
//        if (four_in_a_row(new_bitboard)) {return true;}
//    }


    if (ch0 < 6)
    {
        if (four_in_a_row(bitboard | (one << ch0))) {return true;}
    }

    if (ch1 < 6)
    {
        if (four_in_a_row(bitboard | (one << (7 + ch1)))) {return true;}
    }

    if (ch2 < 6)
    {
        if (four_in_a_row(bitboard | (one << (14 + ch2)))) {return true;}
    }

    if (ch3 < 6)
    {
        if (four_in_a_row(bitboard | (one << (21 + ch3)))) {return true;}
    }

    if (ch4 < 6)
    {
        if (four_in_a_row(bitboard | (one << (28 + ch4)))) {return true;}
    }

    if (ch5 < 6)
    {
        if (four_in_a_row(bitboard | (one << (35 + ch5)))) {return true;}
    }

    if (ch6 < 6)
    {
        if (four_in_a_row(bitboard | (one << (42 + ch6)))) {return true;}
    }

    return false;
}

bool GameState::board_full() const
{
    return number_of_moves == 42;
}

int GameState::get_number_of_moves() const
{
    return number_of_moves;
}

uint64_t GameState::get_key() const
{
    return 0;
}
}
