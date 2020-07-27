#include <iostream>
#include <fstream>
#include <string>
#include "../engine_API.h"

/* This program filters transposition files to smaller files.

   Compilation and linking:
   g++ -O3 -c transposition_list_filter.cpp
   g++ -o transposition_list_filter transposition_list_filter.o ../engine_API.o ../game_state.o
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

    std::string file_with_transpositions = "transposition_list_11_move";
    std::string file_to_write_to = "tr_list_test";

    std::string transposition_move_string;
    std::ifstream file_to_read(file_with_transpositions);
    std::ofstream file_to_write;
    file_to_write.open(file_to_write_to);

    while (std::getline(file_to_read, transposition_move_string))
    {
        load_position(engine, transposition_move_string);
        if(engine.get_value(3, 4) != '0' and (
           engine.get_value(1, 1) != '0' or engine.get_value(2, 1) != '0'
           or engine.get_value(4, 1) != '0' or engine.get_value(5, 1) != '0'))
        {
            file_to_write << transposition_move_string << std::endl;
        }
    }
    file_to_read.close();
    file_to_write.close();

    return 0;
}
