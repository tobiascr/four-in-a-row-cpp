#include "game_state.h"

namespace Engine
{

GameState::GameState()
{
    GameState::reset();
}

void GameState::reset()
{
    bitboard[0] = 0;
    bitboard[1] = 0;
    for (int n=0; n<=6; n++)
        column_height[n] = 0;
    number_of_moves = 0;;
    player_in_turn = 0;
    next_move[0] = 0b1;
    next_move[1] = 0b10000000;
    next_move[2] = 0b100000000000000;
    next_move[3] = 0b1000000000000000000000;
    next_move[4] = 0b10000000000000000000000000000;
    next_move[5] = 0b100000000000000000000000000000000000;
    next_move[6] = 0b1000000000000000000000000000000000000000000;
}

char GameState::get_value(int column, int row) const
{
    const uint64_t a = 1;
    if ((bitboard[0] >> (column * 7 + row)) & a == 1)
    {
        return '1';
    }
    if ((bitboard[1] >> (column * 7 + row)) & a == 1)
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
    history[number_of_moves] = bitboard[player_in_turn];
    bitboard[player_in_turn] |= next_move[column];
    column_height[column]++;
    number_of_moves++;
    player_in_turn = 1 - player_in_turn;
    next_move[column] <<= 1;
}

void GameState::undo_move(int column)
{
    column_height[column]--;
    number_of_moves--;
    player_in_turn = 1 - player_in_turn;
    bitboard[player_in_turn] = history[number_of_moves];
    next_move[column] >>= 1;
}

bool GameState::four_in_a_row() const
{
    return four_in_a_row(bitboard[1 - player_in_turn]);
}

bool GameState::four_in_a_row(uint64_t bitboard) const
{
    // Looking for four in a rows is done in two steps. The first step produces
    // a bitboard with a three in a row if there is a four in a row. The second step
    // checks if there exist points that are two steps distant from each other.

    uint64_t a;
    const int shifts[4] = {6, 8, 7, 1};

    for (int n=0; n<=3; n++)
    {
        a = (bitboard << shifts[n]) & bitboard;
        if (a & (a << (shifts[n] * 2))) {return true;}
    }

    return false;
}

bool GameState::four_in_a_row(int player, int column, int row) const
{
    return four_in_a_row(bitboard[player] | (one << (column * 7 + row)));
}

bool GameState::can_win_this_move() const
{
    uint64_t b = bitboard[player_in_turn];

    const uint64_t next_moves = next_move[0] | next_move[1] | next_move[2] | next_move[3] |
                           next_move[4] | next_move[5] | next_move[6];

    // Masks for various columns.
    const uint64_t mask_0246 = 0b0111111000000001111110000000011111100000000111111;
    const uint64_t mask_04 =   0b0000000000000001111110000000000000000000000111111;
    const uint64_t mask_26 =   0b0111111000000000000000000000011111100000000000000;
    const uint64_t mask_135 =  0b0000000011111100000000111111000000001111110000000;
    const uint64_t mask_15 =   0b0000000011111100000000000000000000001111110000000;
    const uint64_t mask_3 =    0b0000000000000000000000111111000000000000000000000;

    // First a test of 4 moves together that might give a false positive.
    if (four_in_a_row(b | (mask_0246 & next_moves)))
    {
        // To rule out false positives, more tests are done.

        // Test column 0 and 4.
        if (four_in_a_row(b | (mask_04 & next_moves))) {return true;}

        // Test column 2 and 6.
        if (four_in_a_row(b | (mask_26 & next_moves))) {return true;}
    }

    // First a test of 3 moves together that might give a false positive.
    if (four_in_a_row(b | (mask_135 & next_moves)))
    {
        // To rule out false positives, more tests are done.

        // Test column 1 and 5.
        if (four_in_a_row(b | (mask_15 & next_moves))) {return true;}

        // Test column 3.
        if (four_in_a_row(b | (mask_3 & next_moves))) {return true;}
    }

    return false;
}

bool GameState::is_blocking_move(int column) const
{
    return four_in_a_row(bitboard[1 - player_in_turn] | (one << column * 7 + column_height[column]));
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
    const uint64_t mask = ~(bitboard[0] | bitboard[1]);
    return bitboard[0] | (mask ^ (mask << 1));
}

}
