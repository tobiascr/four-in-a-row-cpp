#include <bitset>
#include "game_state.h"

namespace Engine
{

GameState::GameState()
{
    reset();
}

void GameState::reset()
{
    bitboard[0] = 0;
    bitboard[1] = 0;
    for (int n=0; n<=6; n++)
        column_height[n] = 0;
    number_of_moves = 0;
    player_in_turn = 0;
    next_moves = 0b0000001000000100000010000001000000100000010000001;
}

char GameState::get_value(int column, int row) const
{
    if ((bitboard[0] >> (column * 7 + row)) & one == 1)
    {
        return '1';
    }
    if ((bitboard[1] >> (column * 7 + row)) & one == 1)
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
    const uint64_t nm = next_move(column);
    history[number_of_moves] = bitboard[player_in_turn];
    next_moves_history[number_of_moves] = next_moves;
    bitboard[player_in_turn] |= nm;
    column_height[column]++;
    number_of_moves++;
    player_in_turn = 1 - player_in_turn;
    next_moves ^= nm | (nm << 1);
}

void GameState::undo_move(int column)
{
    column_height[column]--;
    number_of_moves--;
    player_in_turn = 1 - player_in_turn;
    bitboard[player_in_turn] = history[number_of_moves];
    next_moves = next_moves_history[number_of_moves];
}

int GameState::player_in_turn_() const
{
    return number_of_moves % 2;
}

uint64_t GameState::next_move(int column) const
{
    return one << (column * 7 + column_height[column]);
}

bool GameState::four_in_a_row() const
{
    return four_in_a_row(bitboard[1 - player_in_turn]);
}

bool GameState::four_in_a_row(uint64_t bitboard) const
{
    /* Looking for four in a rows is done in two steps. The first step produces
    a bitboard with a three in a row if there is a four in a row. The second step
    checks if there exist points that are two steps distant from each other.*/

    uint64_t a;
    const int shifts[4] = {6, 8, 7, 1};

    for (int n=0; n<=3; n++)
    {
        a = (bitboard << shifts[n]) & bitboard;
        if (a & (a << (shifts[n] * 2))) {return true;}
    }

    return false;
}

bool GameState::four_in_a_row_no_vertical(uint64_t bitboard) const
{
    /* Looking for four in a rows is done in two steps. The first step produces
    a bitboard with a three in a row if there is a four in a row. The second step
    checks if there exist points that are two steps distant from each other.*/

    uint64_t a;
    const int shifts[3] = {6, 8, 7};

    for (int n=0; n<=2; n++)
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
    const uint64_t b = bitboard[player_in_turn];

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

bool GameState::opponent_four_in_a_row_above(int column) const
{
    if (column_height[column] >= 5) {return false;}
    return four_in_a_row_no_vertical(bitboard[1 - player_in_turn] | (next_move(column) << 1));
}

bool GameState::is_blocking_move(int column) const
{
    return four_in_a_row(bitboard[1 - player_in_turn] | next_move(column));
}

uint64_t GameState::get_opponent_winning_positions_bitboard() const
{
    return get_winning_positions_bitboard(bitboard[1 - player_in_turn]);
}

uint64_t GameState::get_winning_positions_bitboard(uint64_t bitboard) const
/* Can also include already occupied positions and positions outside the board.*/
{
    uint64_t winning_positions = 0;

    // Vertical direction
    winning_positions = (bitboard & (bitboard << 1) & (bitboard << 2)) << 1;

    // Horizontal direction
    winning_positions |= (bitboard & (bitboard << 7) & (bitboard << 14)) << 7; // ooox
    winning_positions |= (bitboard & (bitboard << 7) & (bitboard << 14)) >> 21; // xooo
    winning_positions |= ((bitboard & (bitboard << 21)) & (bitboard << 14)) >> 7; // ooxo
    winning_positions |= ((bitboard & (bitboard << 21)) & (bitboard << 7)) >> 14; // oxoo

    // Diagonal direction 1
    winning_positions |= (bitboard & (bitboard << 6) & (bitboard << 12)) << 6; // ooox
    winning_positions |= (bitboard & (bitboard << 6) & (bitboard << 12)) >> 18; // xooo
    winning_positions |= ((bitboard & (bitboard << 18)) & (bitboard << 12)) >> 6; // ooxo
    winning_positions |= ((bitboard & (bitboard << 18)) & (bitboard << 6)) >> 12; // oxoo

    // Diagonal direction 2
    winning_positions |= (bitboard & (bitboard << 8) & (bitboard << 16)) << 8; // ooox
    winning_positions |= (bitboard & (bitboard << 8) & (bitboard << 16)) >> 24; // xooo
    winning_positions |= ((bitboard & (bitboard << 24)) & (bitboard << 16)) >> 8; // ooxo
    winning_positions |= ((bitboard & (bitboard << 24)) & (bitboard << 8)) >> 16; // oxoo

    return winning_positions;
}

std::array<bool,7> GameState::get_non_losing_moves()
{
    const uint64_t opponent_winning_positions =
              get_winning_positions_bitboard(bitboard[1 - player_in_turn])
              & board_mask;
    uint64_t possible_moves;
    const uint64_t blocking_moves = opponent_winning_positions & next_moves;

    if(blocking_moves)
    {
        std::bitset<64> bl = blocking_moves;
        int number_of_blocking_moves = bl.count();

        if(number_of_blocking_moves > 1)
        {
            return {false, false, false, false, false, false, false};
        }
        else
        {
            if((opponent_winning_positions >> 1) & blocking_moves)
            {
                return {false, false, false, false, false, false, false};
            }
            else
            {
                possible_moves = blocking_moves;
            }
        }
    }
    else
    {
        possible_moves = next_moves & board_mask & (~(opponent_winning_positions >> 1));
    }

    return {possible_moves & 0b0000000000000000000000000000000000000000000111111,
            possible_moves & 0b0000000000000000000000000000000000001111110000000,
            possible_moves & 0b0000000000000000000000000000011111100000000000000,
            possible_moves & 0b0000000000000000000000111111000000000000000000000,
            possible_moves & 0b0000000000000001111110000000000000000000000000000,
            possible_moves & 0b0000000011111100000000000000000000000000000000000,
            possible_moves & 0b0111111000000000000000000000000000000000000000000};
}

int GameState::open_four_in_a_row_count(int player) const
{
    std::bitset<64> winning_positions = get_winning_positions_bitboard(bitboard[player])
                                 & board_mask & (~(bitboard[0] | bitboard[1]));
    return winning_positions.count();
}

bool GameState::board_full() const
{
    return number_of_moves == 42;
}

int GameState::get_number_of_moves() const
{
    return number_of_moves;
}

int GameState::position_value_40_ply()
{
    int value = 0;
    const uint64_t fifth_row = 0b0010000001000000100000010000001000000100000010000;

    // If there is only one column that is not filled.
    if (next_moves ^ fifth_row)
    {
        if (four_in_a_row_no_vertical(bitboard[1] | ((next_moves & board_mask) << 1)))
        {
            return -1;
        }
    }

    // If there are two columns that are not filled.

    // Testing both last moves at the same time.
    if (not four_in_a_row(bitboard[1] | (next_moves & board_mask)))
    {
        return 0;
    }

    // To rule out false positives, test the last moves one at a time.
    for (int col=0; col<=6; col++)
    {
        if (column_height[col] < 6)
        {
            if (four_in_a_row(bitboard[1] | next_move(col)))
            {
                return -1;
            }
        }
    }

    return 0;
}

uint64_t GameState::get_key() const
{
    return bitboard[0] | next_moves;
}
}
