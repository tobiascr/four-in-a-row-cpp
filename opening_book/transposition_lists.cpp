#include <iostream>
#include <string>
#include <unordered_map>
#include "../game_state.h"

// This program produces lists of transpositions. All transpositions with a certain amount of
// moves made that can occur in a game, including positions with a four in a row, except
// for mirrored versions of transpositions are included. For example if the transposition where
// one move is made to the first column is included, the transposition where a move is made
// to the last column is not included.

// Compilation and linking:
// g++ -O3 -c transposition_lists.cpp
// g++ -o transposition_lists transposition_lists.o ../game_state.o

void print_board(Engine::GameState& game_state)
{
    using namespace Engine;
    int row;
    int col;
    std::cout << std::endl;
    for (row=5; row>=0; row--)
    {
        for (col=0; col<=6; col++)
            std::cout << game_state.get_value(col, row);
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

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

std::string mirror_move_string(std::string move_string)
{
    std::string mirror = "";
    for (int n=0; n<move_string.size(); n++)
    {
        if (move_string[n] == '0')
            mirror.append("6");
        if (move_string[n] == '1')
            mirror.append("5");
        if (move_string[n] == '2')
            mirror.append("4");
        if (move_string[n] == '3')
            mirror.append("3");
        if (move_string[n] == '4')
            mirror.append("2");
        if (move_string[n] == '5')
            mirror.append("1");
        if (move_string[n] == '6')
            mirror.append("0");
    }
    return mirror;
}

std::unordered_map<uint64_t, std::string> transposition_map(
                    std::unordered_map<uint64_t, std::string> original_transposition_map)
// Make a new transposition map starting from original_transposition_map but with one
// more move.
{
    Engine::GameState game_state;
    Engine::GameState mirrored_game_state;
    char move[7] = {'0', '1', '2', '3', '4', '5', '6'};
    auto tr = original_transposition_map;
    std::unordered_map<uint64_t, std::string> new_transposition_map;
    for (auto i = tr.begin(); i != tr.end(); ++i)
    {
        auto element = *i;
        std::string start_transposition = element.second;
        load_position(game_state, start_transposition);
        if(game_state.four_in_a_row()) {continue;};
        for (int n=0; n<=6; n++)
        {
            if (not game_state.column_not_full(n)) {continue;};
            game_state.make_move(n);
            uint64_t key = game_state.get_key();
            game_state.undo_move(n);
            std::string new_transposition = start_transposition + move[n];

            if(new_transposition_map.count(key) == 0)
            {
                // Produce a mirrored move string.
                std::string mirror = mirror_move_string(new_transposition);
                load_position(mirrored_game_state, mirror);
                uint64_t mirror_key = mirrored_game_state.get_key();
                if(new_transposition_map.count(mirror_key) == 0)
                {
                    new_transposition_map[key] = new_transposition;
                }
            }
        }
    }
    return new_transposition_map;
}

int main()
{
    Engine::GameState game_state;

    // Produce a 1 move transposition list.
    std::unordered_map<uint64_t, std::string> tr_1;
    game_state.reset();
    game_state.make_move(0);
    tr_1[game_state.get_key()] = "0";
    game_state.reset();
    game_state.make_move(1);
    tr_1[game_state.get_key()] = "1";
    game_state.reset();
    game_state.make_move(2);
    tr_1[game_state.get_key()] = "2";
    game_state.reset();
    game_state.make_move(3);
    tr_1[game_state.get_key()] = "3";

    auto tr_2 = transposition_map(tr_1);
    auto tr_3 = transposition_map(tr_2);
    auto tr_4 = transposition_map(tr_3);
    auto tr_5 = transposition_map(tr_4);
    auto tr_6 = transposition_map(tr_5);
    auto tr_7 = transposition_map(tr_6);
    auto tr_8 = transposition_map(tr_7);
    auto tr_9 = transposition_map(tr_8);
    auto tr_10 = transposition_map(tr_9);
//    auto tr_11 = transposition_map(tr_10);
//    auto tr_12 = transposition_map(tr_11);

    std::cout << "Size: " << tr_1.size() << std::endl;
    std::cout << "Size: " << tr_2.size() << std::endl;
    std::cout << "Size: " << tr_3.size() << std::endl;
    std::cout << "Size: " << tr_4.size() << std::endl;
    std::cout << "Size: " << tr_5.size() << std::endl;
    std::cout << "Size: " << tr_6.size() << std::endl;
    std::cout << "Size: " << tr_7.size() << std::endl;
    std::cout << "Size: " << tr_8.size() << std::endl;
    std::cout << "Size: " << tr_9.size() << std::endl;
    std::cout << "Size: " << tr_10.size() << std::endl;
//    std::cout << "Size: " << tr_11.size() << std::endl;
//    std::cout << "Size: " << tr_12.size() << std::endl;

    return 0;
}
