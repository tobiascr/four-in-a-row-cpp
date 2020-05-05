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
    uint64_t a;
    uint64_t bitboard;

    if (player_1_in_turn)
    {
        bitboard = bitboard_2;
    }
    else
    {
        bitboard = bitboard_1;
    }

    // Looking for four in a rows is done in two steps. The first step produces
    // a bitboard with a three in a row if there is a four in a row. The second step
    // checks if there exist points that are two steps distant from each other.

    const int shifts[4] = {6, 8, 1, 7};

    for (int n=0; n<=3; n++)
    {
        a = (bitboard << shifts[n]) & bitboard;
        if (a & (a << (shifts[n] * 2))) {return true;}
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
