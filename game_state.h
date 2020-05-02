#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>

namespace Engine
{
/*  Many times in this program, rows and columns are refered to. Rows are counted from
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

    bool four_in_a_row(int column) const;
    /* True iff there is a four in a row that goes through the position
       of upmost disk in column.*/

    bool board_full() const;

    int get_number_of_moves() const;

    std::string get_key() const;
    // Return a unique key that corresponds to the current game state.

private:
    uint64_t bitboard_1; // Bitboard for the player that make the first move.
    uint64_t bitboard_2; // Bitboard for the player that make the second move.
    int column_height[7];
    int number_of_moves;
    bool player_1_in_turn;
};
}

#endif
