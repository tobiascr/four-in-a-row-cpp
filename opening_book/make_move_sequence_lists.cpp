#include <iostream>
#include <fstream>
#include <string>
#include "../engine_API.h"

/* This program takes text files with lists of sequences as input. Each line should
   have one sequence. The sequences should be described in form of strings of
   moves ('0', '1', ..., '6'). For example move_string = "334". It gives the sequence
   of first making move 3, then move 3 and then move 4.

   The program gives new one step longer sequences as output.

   Compilation and linking:
   g++ -O3 -c make_move_sequence_lists.cpp
   g++ -o make_move_sequence_lists make_move_sequence_lists.o ../engine_API.o ../game_state.o
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

std::string best_moves(Engine::EngineAPI& engine, std::string transposition_move_string)
// Return a string with the best moves for the given transposition, except for
// moves that gives a four in a row.
{
    load_position(engine, transposition_move_string);
    int value = engine.position_value_full_depth();
    std::string moves = "0123456";
    std::string return_string = "";
    for(int col=0; col<=6; col++)
    {
        if(engine.get_value(col, 5) == '0') // If the column is not full.
        {
            load_position(engine, transposition_move_string + moves[col]);
            if(not engine.four_in_a_row())
            {
                if(engine.position_value_full_depth() == -value)
                {
                    return_string += moves[col];
                }
            }
        }
    }
    return return_string;
}

std::string legal_moves(Engine::EngineAPI& engine, std::string transposition_move_string)
// Return a string with legal moves for the given transposition, except for
// moves that gives a four in a row.
{
    load_position(engine, transposition_move_string);
    std::string moves = "0123456";
    std::string return_string = "";
    for(int col=0; col<=6; col++)
    {
        if(engine.get_value(col, 5) == '0') // If the column is not full.
        {
            if(not engine.four_in_a_row())
            {
               return_string += moves[col];
            }
        }
    }
    return return_string;
}

int main()
{
    using namespace Engine;
    EngineAPI engine;

    std::string file_with_sequences;
    std::string file_to_write_to;

    std::cout << "File with sequences to start with: ";
    std::cin >> file_with_sequences;
    std::cout << "File to write to: ";
    std::cin >> file_to_write_to;

    std::string move_sequence_string;
    std::ifstream file_to_read(file_with_sequences);
    std::ofstream file_to_write;
    file_to_write.open(file_to_write_to);
    int count = 0;

    while (std::getline(file_to_read, move_sequence_string))
    {
        load_position(engine, move_sequence_string);

//        // Add best moves that does not give a four in a row.
//        std::string moves = best_moves(engine, move_sequence_string);
//        for(char move : moves)
//        {
//            file_to_write << move_sequence_string << move << std::endl;
//            count++;
//            std::cout << count << " sequences written" << std::endl;
//        }

        // Add any move that does not give a four in a row.
        std::string moves = legal_moves(engine, move_sequence_string);
        for(char move : moves)
        {
            file_to_write << move_sequence_string << move << std::endl;
            count++;
            std::cout << count << " sequences written" << std::endl;
        }
    }
    file_to_read.close();
    file_to_write.close();

    return 0;
}
