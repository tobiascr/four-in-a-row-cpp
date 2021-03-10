This folder contains an opening book. It consists of .values and .best_moves files.

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

For some .best_moves files only transpositions with certain values are stored. The
values are specified in the file name.

Some .best_moves files contains slow transpositions that are not included in other files.
