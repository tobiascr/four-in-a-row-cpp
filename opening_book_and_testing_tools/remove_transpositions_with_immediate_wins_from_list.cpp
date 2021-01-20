#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "../game_state.h"

/* This program takes text files with lists of sequences as input. Each line should
   have one sequence. The sequences should be described in form of strings of
   moves ('0', '1', ..., '6'). For example move_string = "334". It gives the sequence
   of first making move 3, then move 3 and then move 4.

   The program give a transposition list as output in forms of move sequences. Any
   transpositions where a four in a row can be made by the player in turn
   are being excluded.

   Compilation and linking:
   g++ -O3 -c remove_transpositions_with_immediate_wins_from_list.cpp
   g++ -o remove_transpositions_with_immediate_wins_from_list remove_transpositions_with_immediate_wins_from_list.o ../game_state.o
*/

void load_position(Engine::GameState& game_state, std::string move_string)
/* Load a position to the given game_state object. A position is described
   as a string of moves ('0', '1', ..., '6'). For example move_string = "334".
   It gives the position that arises from first making move 3, then move 3 and then move 4. */
{
    game_state.reset();
    int n;
    for (n=0; n<move_string.length(); n++)
    {
        if (move_string[n] == '0')
            game_state.make_move(0);
        if (move_string[n] == '1')
            game_state.make_move(1);
        if (move_string[n] == '2')
            game_state.make_move(2);
        if (move_string[n] == '3')
            game_state.make_move(3);
        if (move_string[n] == '4')
            game_state.make_move(4);
        if (move_string[n] == '5')
            game_state.make_move(5);
        if (move_string[n] == '6')
            game_state.make_move(6);
    }
}

int main()
{
    using namespace Engine;
    Engine::GameState game_state;

    std::unordered_map<uint64_t, bool> transpositions;

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
        load_position(game_state, move_sequence_string);

        if (not game_state.can_win_this_move())
        {
            file_to_write << move_sequence_string << std::endl;
            count++;
            std::cout << count << " sequences written" << std::endl;
        }
    }
    file_to_read.close();
    file_to_write.close();

    return 0;
}
