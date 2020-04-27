#include "test_game_state.h"

namespace TestEngine
{

GameState::GameState()
{
    int n;
    for (n=0; n<=71; n++)
        board[n] = '0';
    board[72] = '\0';       // This make it easy to convert the board to a string.
    for (n=0; n<=6; n++)
        column_height[n] = 0;
    number_of_moves = 0;
}

void GameState::reset()
{
    int n;
    for (n=0; n<=71; n++)
        board[n] = '0';
    for (n=0; n<=6; n++)
        column_height[n] = 0;
    number_of_moves = 0;
}

char GameState::get_value(int column, int row) const
{
    return board[10 + column + row * 9];
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
    if (number_of_moves % 2 == 0)
        board[10 + column + column_height[column] * 9] = '1';
    else
        board[10 + column + column_height[column] * 9] = '2';
    column_height[column]++;
    number_of_moves++;
}

void GameState::undo_move(int column)
{
    column_height[column]--;
    board[10 + column + column_height[column] * 9] = '0';
    number_of_moves--;
}

bool GameState::four_in_a_row(int column) const
{
    int position = 1 + column + column_height[column] * 9;
    char player = board[position];

    // Columns
    if (position / 9 >= 4)
        if (board[position - 9] == player)
            if (board[position - 18] == player)
                if (board[position - 27] == player)
                    return true;

    // Rows
    int in_row = 1;
    if (board[position - 1] == player)
    {
        in_row++;
        if (board[position - 2] == player)
        {
            in_row++;
            if (board[position - 3] == player)
                in_row++;
        }
    }
    if (board[position + 1] == player)
    {
        in_row ++;
        if (board[position + 2] == player)
        {
            in_row++;
            if (board[position + 3] == player)
                in_row++;
        }
    }
    if (in_row >= 4)
        return true;

    // Diagonals
    in_row = 1;
    if (board[position - 10] == player)
    {
        in_row++;
        if (board[position - 20] == player)
        {
            in_row++;
            if (board[position - 30] == player)
                in_row++;
        }
    }
    if (board[position + 10] == player)
    {
        in_row ++;
        if (board[position + 20] == player)
        {
            in_row++;
            if (board[position + 30] == player)
                in_row++;
        }
    }
    if (in_row >= 4)
        return true;

    in_row = 1;
    if (board[position - 8] == player)
    {
        in_row++;
        if (board[position - 16] == player)
        {
            in_row++;
            if (board[position - 24] == player)
                in_row++;
        }
    }
    if (board[position + 8] == player)
    {
        in_row ++;
        if (board[position + 16] == player)
        {
            in_row++;
            if (board[position + 24] == player)
                in_row++;
        }
    }
    if (in_row >= 4)
        return true;

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
    return board;
}
}
