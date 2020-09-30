A four in a row game programmed with C++. It is developed for Linux. It can
compute moves to full depth and can make perfect moves.

I uses bitboards, negamax, alpha beta pruning, transposition table,
iterative deepening, move sorting based on heuristics, and an opening book.

To compile the program run make. Then type ./four_in_a_row_command_line to run it.

The opening book consists of the following files:

opening_book_3_ply_values
opening_book_6_ply_values
opening_book_8_ply_values
opening_book_8_ply_best_moves
opening_book_9_ply_best_moves
opening_book_10_ply_best_moves

They are textfiles where each row consists of one string describing a transposition
followed by either a value for the position or a string of best moves.
The transpositions are described in form of strings of moves ('0', '1', ..., '6').
For example move_string = "334". It gives the position that
arises from first making move 3, then move 3 and then move 4.
Values are defined as follows: A win for the player in turn at move 42 give the value 1,
a win at move 41 give a the value 2 etc, and similarly for losses but with negative values.
A draw has value 0.

In the opening book files there are no transpositions with a four in a row, and
no transposition in which the player in turn can make a four in a row.

The value-files consists of all transpositions that can be reached by legal moves,
except for transpositions with a four in a row, and transpositions in which the
player in turn can make a four in a row.

The best_move-files consists of transpositions that can be reached when the player
in turn made all their moves in the best way and the opponent made any legal move,
with exceptions for transpositions with a four in a row, and transpositions in which the
player in turn can make a four in a row. A best move is defined as any move that lead to
a position with a maximum value.
