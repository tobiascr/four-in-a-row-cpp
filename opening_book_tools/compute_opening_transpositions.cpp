#include <iostream>
#include <fstream>
#include <string>
#include "../engine_API.h"

/* This program takes text files with lists of transpositions as input. Each line should
   have one transposition. The transpositions should be described in form of strings of
   moves ('0', '1', ..., '6'). For example move_string = "334". It gives the position that
   arises from first making move 3, then move 3 and then move 4.
   Output is given in a file where each line is a transposition the same format as in the
   input file followed by a space and then followed by a move value.
   A win for the player in turn at move 42 give the value 1, a win at move 41 give a
   the value 2 etc, and similarly for losses but with negative values.

   Compilation and linking:
   g++ -O3 -c compute_opening_transpositions.cpp
   g++ -o compute_opening_transpositions compute_opening_transpositions.o ../engine_API.o ../game_state.o ../opening_book.o
*/

void load_position(Engine::EngineAPI& engine, std::string move_string)
/* Load a position to the given EngineAPI object. A position is described
   as a string of moves ('0', '1', ..., '6'). For example move_string = "334".
   It gives the position that arises from first making move 3, then move 3 and then move 4. */
{
    using namespace Engine;
    engine.new_game();
    int n;
    for (n=0; n<move_string.length(); n++)
    {
        if (move_string[n] == '0')
            engine.make_move(0);
        if (move_string[n] == '1')
            engine.make_move(1);
        if (move_string[n] == '2')
            engine.make_move(2);
        if (move_string[n] == '3')
            engine.make_move(3);
        if (move_string[n] == '4')
            engine.make_move(4);
        if (move_string[n] == '5')
            engine.make_move(5);
        if (move_string[n] == '6')
            engine.make_move(6);
    }
}

int main()
{
    using namespace Engine;
    EngineAPI engine;

    std::string file_with_transpositions;
    std::string file_to_write_to;

    std::cout << "File with transpositions to compute: ";
    std::cin >> file_with_transpositions;
    std::cout << "File to write to: ";
    std::cin >> file_to_write_to;

    std::string transposition_move_string;
    std::ifstream file_to_read(file_with_transpositions);
    std::ofstream file_to_write;
    file_to_write.open(file_to_write_to);
    int count = 0;

    while (std::getline(file_to_read, transposition_move_string))
    {
        load_position(engine, transposition_move_string);
        int value = engine.position_value_full_depth();
        file_to_write << transposition_move_string << " " << std::to_string(value)  << std::endl;
        count++;
        std::cout << count << " transpositions computed" << std::endl;
    }
    file_to_read.close();
    file_to_write.close();

    return 0;
}
