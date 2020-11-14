#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <array>
#include <stdint.h>

namespace Engine
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

    bool own_threat_above(int column) const;
    /* True iff a move to column make it possible for the opponent to block
    a four in a row in the next move by making a move to the same column.*/

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

    int open_four_in_a_row_count(int player) const;
    /* Return the number of unoccupied places on the board that give player
    a four in a row. player is 0 for the player making the first move and 1
    for the other player.*/

    bool board_full() const;

    int get_number_of_moves() const;

    int position_value_40_ply();
    /* There must be no four in a row in the game state and possibility for the
    player in turn to make a four in a row.*/

    uint64_t get_unique_key() const;
    /* Return a unique key that corresponds to the current game state.*/

    uint64_t get_unique_mirror_key() const;
    /* Return a unique key that corresponds to the mirrored version of the
    current game state.*/

    uint32_t get_zobrist_key() const;

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
    int player_in_turn;  // 0 if it's the beginning player in turn and else 1.
    int column_height[7];
    int number_of_moves;
    uint64_t history[42]; // Bitboards from earlier moves.
    const uint64_t one = 1;
    const uint64_t board_mask = 0b0111111011111101111110111111011111101111110111111;
    uint64_t next_moves;
    uint64_t next_moves_history[42];
    uint32_t zobrist_key;

    // Random numbers to use for Zobrist keys.
    const uint64_t random_numbers[84] =
    {233365778, 323053240, 3156326384, 1619824615, 1408550458, 1689030724,
     3442979491, 2853287807, 1692922701, 658397745, 3303151175, 2104256279,
     2415497163, 1051558874, 2588457125, 3376357842, 1888962657, 3794001249,
     3638795069, 1730276612, 271029656, 2300141826, 2045462692, 2568370873,
     1542508188, 912569306, 1798829996, 3718546274, 2376673007, 3983560801,
     2935658018, 2819072703, 3549663376, 3692456834, 2157380482, 766607675,
     2406353340, 2375700214, 1125740654, 246545968, 2834963847, 3677785623,
     3293175401, 36674243, 3015487058, 1163011859, 50774060, 115476991,
     2327330118, 3598651222, 1279581453, 1725363741, 2711757941, 689007059,
     1650060212, 3726102468, 2800551579, 38614514, 2689893116, 2389681656,
     1539207885, 1265000159, 3003002015, 2805336395, 3946759362, 3360101813,
     3048028889, 2235030666, 1589179138, 2579351938, 1657401944, 2559535683,
     1456382607, 1887196775, 3617756832, 3853077856, 438133448, 3682831322,
     1070045159, 3263680127, 3465052477, 2566710711, 1037354185, 3639958555};

    int player_in_turn_() const;
    /* Return 0 if it's the beginning player in turn and else 1.*/

    uint64_t next_move(int column) const;
    /* Return a bitboard with a 1 at the position of the next move that can
    be made at the given column and 0:s at all other positions.*/

    bool four_in_a_row(uint64_t bitboard) const;
    /* Return true iff there is a four in a row of 1:s on the bitboard.*/

    bool four_in_a_row_no_vertical(uint64_t bitboard) const;

    uint64_t get_winning_positions_bitboard(uint64_t bitboard) const;
    /* Can also include already occupied positions and positions outside the board.*/
};
}

#endif
