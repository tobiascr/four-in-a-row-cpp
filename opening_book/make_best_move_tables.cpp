#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "../engine_API.h"

/* This program takes text files with lists of transpositions as input. Each line should
   have one transposition. The transpositions should be described in form of strings of
   moves ('0', '1', ..., '6'). For example move_string = "334". It gives the position that
   arises from first making move 3, then move 3 and then move 4.

   Compilation and linking:
   g++ -O3 -c make_best_move_tables.cpp
   g++ -o make_best_move_tables make_best_move_tables.o ../engine_API.o ../game_state.o
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
// Return a string with the best moves for the given transposition.
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
            if(engine.position_value_full_depth() == -value)
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

    std::chrono::steady_clock::time_point t0;
    std::chrono::steady_clock::time_point t1;
    std::chrono::steady_clock::duration move_time;

    while (std::getline(file_to_read, transposition_move_string))
    {
        load_position(engine, transposition_move_string);
//        t0 = std::chrono::steady_clock::now();
        int value = engine.position_value_full_depth();
        if (value != 33)
        {
//        t1 = std::chrono::steady_clock::now();
//        move_time = t1 - t0;
        file_to_write << transposition_move_string // << " " << std::to_string(value)
                      << " " << best_moves(engine, transposition_move_string);
//        file_to_write << " " << std::chrono::duration_cast<std::chrono::milliseconds>(move_time).count();
        file_to_write << std::endl;
        count++;
        std::cout << count << " transpositions computed" << std::endl;
        }
    }
    file_to_read.close();
    file_to_write.close();

    return 0;
}
