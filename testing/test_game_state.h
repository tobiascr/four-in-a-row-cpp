#ifndef TEST_GAMESTATE_H
#define TEST_GAMESTATE_H

#include <array>
#include <stdint.h>

namespace TestEngine
{
/* Many times in this program, rows and columns are refered to. Rows are counted from
below and numbered 0, 1, ..., 5. Columns are counted from the left and are numbered
0, 1, ..., 6. Moves are represented by the corresponding columns the moves are made to.*/

class GameState
{
public:
    GameState();

    void reset();

    char get_value(int column, int row) const;
    /* Return '1', '2' or '0' depending on the state of the corresponding position.
   '1' represents the player making the first move, '2' the player making
    second move and '0' that the position is empty.*/

    bool column_not_full(int column) const;

    int get_number_of_disks_in_column(int column) const;

    void make_move(int column);

    void undo_move(int column);

    bool four_in_a_row() const;
    /* Return true iff a the player that made the last move has a four in a row.*/

    bool four_in_a_row(int player, int column, int row) const;
    /* Return true iff the game state is a four in a row after player has placed a disk at
    the given position. player is 0 if it's the player that makes the first move
    and 1 if it's the player that makes the second move.*/

    bool can_win_this_move() const;
    /* Return true iff a the player in turn can make a move a move that gives
    a four in a row.*/

    bool opponent_four_in_a_row_above(int column) const;
    /* Return true iff the opponent can make a four in a row in column the next move
    if the player in turn makes a move to column.*/

    bool is_blocking_move(int column) const;
    /* Return true iff making a move in column blocks the opponent from making a
    four in a row in the next move. The move must be legal.*/

    uint64_t get_opponent_winning_positions_bitboard() const;
    /* Can also include already occupied positions and positions outside the board.*/

    std::array<bool,7> get_non_losing_moves();
    /*Return a list of values that tells if that the player in turn can make
    a move such that the opponent can't make a four in a row the next move.
    For example if the first value is true, it means that the move is not
    losing.*/

    bool board_full() const;

    int get_number_of_moves() const;

    uint64_t get_key() const;
    // Return a unique key that corresponds to the current game state.

private:
    /* The places where disks are positioned on the board is stored in bitboards
    in the form of 64 bit integers. Each player have one bitboard.

    .  .  .  .  .  .  . . .
    5 12 19 26 33 40 47 . .
    4 11 18 25 32 39 46 . .
    3 10 17 24 31 38 45 . .
    2  9 16 23 30 37 44 . .
    1  8 15 22 29 36 43 . .
    0  7 14 21 28 35 42 . . .

    The diagram shows how the bits on the bitboards that corresponds to positions
    on the board. The first bit is the lowest position to the left. The 48:th bit is
    the highest position to the right.

    If there is a disk at a position it's bit is 1. If not it's value is 0.
    All bits not corresponding to positions are 0.

    Using some shifting techniques it's possible to quickly find four in a rows.
    */

    uint64_t bitboard[2]; // bitboard[0] is for the player that makes the first move.
                          // bitboard[1] is for the player that makes the second move.
    int player_in_turn;  // 0 if it's the beginner player in turn and else 1.
    int column_height[7];
    int number_of_moves;
    uint64_t history[42]; // Bitboards from earlier moves.
    const uint64_t one = 1;
    uint64_t next_move[7];
    uint64_t next_moves;
    uint64_t next_moves_history[42];

    bool four_in_a_row(uint64_t bitboard) const;
    /* Return true iff there is a four in a row of 1:s on the bitboard.*/

    bool four_in_a_row_no_vertical(uint64_t bitboard) const;

    uint64_t get_winning_positions_bitboard(uint64_t bitboard) const;
    /* Can also include already occupied positions and positions outside the board.*/
};
}

#endif
