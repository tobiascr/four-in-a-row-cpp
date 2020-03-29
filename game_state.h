#ifndef GAMESTATE_H
#define GAMESTATE_H

/*  A board configuration is stored as a list of lengths 72. Empty positions are stored as '0'.
    The players are called '1' and '2', where '1' always make the first move.

    Some of the entries in the list is always '0'. They represent the positions outside of the
    board, as in the following diagram.

    000000000
    0xxxxxxx0
    0xxxxxxx0
    0xxxxxxx0
    0xxxxxxx0
    0xxxxxxx0
    0xxxxxxx0
    000000000

    The positions with x are the positions on the board. The position in the lower left
    corner have index 10, the next index 11 etc.

    Many times in this program, rows and columns are refered to. Rows are counted from
    below and numbered 0, 1, ..., 5. Columns are counted from the left and are numbered
    0, 1, ..., 6. Moves are represented by the corresponding columns the moves are made to.*/

class GameState
{
public:
    GameState();

    void reset();

    char get_value(int column, int row) const;
    // Return '0', '1' or '2'.

    bool column_not_full(int column) const;

    void make_move(int column);

    void undo_move(int column);

    bool four_in_a_row(int column) const;
    // True iff there is a four in a row that goes through the position
    // of upmost disk in column.

    bool board_full() const;

    int get_number_of_moves() const;

private:
    char board[72];
    int column_height[7];
    int number_of_moves;
};

#endif
