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
    uint64_t move = one << (column * 7 + column_height[column]);
    history[number_of_moves] = bitboard[player_in_turn];
    bitboard[player_in_turn] |= move;
    column_height[column]++;
    number_of_moves++;
    player_in_turn = 1 - player_in_turn;
}

void GameState::undo_move(int column)
{
    column_height[column]--;
    number_of_moves--;
    player_in_turn = 1 - player_in_turn;
    bitboard[player_in_turn] = history[number_of_moves];
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
    const int shifts[4] = {6, 8, 1, 7};

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

    // This corresponds to the strip on the bitmap above the board. It is used
    // to remove illegal moves.
    const uint64_t top_rank_mask = 0b0111111011111101111110111111011111101111110111111;

    const uint64_t move0 = one << column_height[0];
    const uint64_t move1 = one << (7 + column_height[1]);
    const uint64_t move2 = one << (14 + column_height[2]);
    const uint64_t move3 = one << (21 + column_height[3]);
    const uint64_t move4 = one << (28 + column_height[4]);
    const uint64_t move5 = one << (35 + column_height[5]);
    const uint64_t move6 = one << (42 + column_height[6]);

    if (four_in_a_row(
       (b |move0 | move2 | move4 | move6) & top_rank_mask))
    {
        // Test column 0 and 4.
        if (four_in_a_row((b | move0 | move4) & top_rank_mask)) {return true;}

        // Test column 2 and 6.
        if (four_in_a_row((b | move2 | move6) & top_rank_mask)) {return true;}
    }

    if (four_in_a_row(
       (b |move1 | move3 | move5) & top_rank_mask))
    {
        // Test column 1 and 5.
        if (four_in_a_row((b | move1 | move5) & top_rank_mask)) {return true;}

        // Test column 3.
        if (column_height[3] < 6)
        {
            if (four_in_a_row(b | move3)) {return true;}
        }
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

std::string GameState::get_key() const
{
    return std::to_string(bitboard[0]) + std::to_string(bitboard[1]);
}

std::pair<uint64_t, uint64_t> GameState::get_key_2() const
{
    return {bitboard[0], bitboard[1]};
}
}
