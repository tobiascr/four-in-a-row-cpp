### Four in a row

A four in a row game programmed with C++. It is developed for Linux. It can
compute moves to full depth and can make perfect moves.

I uses bitboards, negamax, alpha beta pruning, transposition table,
iterative deepening, move sorting based on heuristics, and an opening book.

### Installation on Linux

Download the repository as a zip-file. Uncompress it and go to
it's folder in the terminal.

To compile the program run

    make

When it's compiled, run

    sudo make install

to install it.

Then type

    four_in_a_row_command_line

to run it. To uninstall, run

    sudo make uninstall

You can also run

    make test

to compile the test program. It need to have the ordinary program installed,
in order to have access to the opening book.
