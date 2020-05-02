#include "game_state.h"

namespace Engine
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
        bitboard_1 &= ~a;
    }
    else
    {
        bitboard_2 &= ~a;
    }
}

bool GameState::four_in_a_row(int column) const
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

    // Diagonals
    a = (bitboard << 6) & bitboard;
    if (a & (a << 12)) {return true;}

    a = (bitboard << 8) & bitboard;
    if (a & (a << 16)) {return true;}

    // Columns
    a = (bitboard << 1) & bitboard;
    if (a & (a << 2)) {return true;}

    // Rows
    a = (bitboard << 7) & bitboard;
    if (a & (a << 14)) {return true;}

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

std::string GameState::get_key() const
{
    return "0123";
}
}
