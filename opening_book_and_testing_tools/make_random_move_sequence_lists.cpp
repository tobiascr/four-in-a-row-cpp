#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include "../engine_API.h"

/* Compilation and linking:
   g++ -O3 -c make_random_move_sequence_lists.cpp
   g++ -o make_random_move_sequence_lists make_random_move_sequence_lists.o ../engine_API.o ../game_state.o ../opening_book.o
*/

std::string get_random_move_sequence()
{
    Engine::EngineAPI engine;
    std::string move_sequence_string;
    int move;

    std::mt19937 random_generator;
    std::random_device rd;
    random_generator.seed(rd());
    std::uniform_int_distribution<> uid(0, 9);

    while(true)
    {
        if(move_sequence_string.length() <= 4)
        {
            engine.set_difficulty_level(4);
        }
        if(move_sequence_string.length() >= 5 and move_sequence_string.length() <= 7)
        {
            engine.set_difficulty_level(3);
        }
        if(move_sequence_string.length() >= 8 and move_sequence_string.length() <= 30)
        {
            engine.set_difficulty_level(2);
        }
        if(move_sequence_string.length() >= 31)
        {
            engine.set_difficulty_level(3);
        }
        if(uid(random_generator) == 0)
        {
            engine.set_difficulty_level(4);
        }

        move = engine.engine_move();
        engine.make_move(move);

        if(engine.four_in_a_row())
        {
            break;
        }

        move_sequence_string.append(std::to_string(move));

        if(engine.board_full())
        {
            break;
        }
    }

    return move_sequence_string;
}

int main()
{
    std::string file_to_write_to;
    std::cout << "File to write to: ";
    std::cin >> file_to_write_to;
    std::ofstream file_to_write;
    file_to_write.open(file_to_write_to);

    for(int n=1; n<=40000; n++)
    {
        file_to_write << get_random_move_sequence() << std::endl;
        std::cout << n << " sequences written" << std::endl;
    }
    file_to_write.close();

    return 0;
}
