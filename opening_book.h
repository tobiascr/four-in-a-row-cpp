#ifndef OPENING_BOOK_H
#define OPENING_BOOK_H

#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include "game_state.h"

namespace Engine
{

class OpeningBook
{
public:
    OpeningBook();

    std::vector<int> get_best_moves(Engine::GameState& game_state);
    /* Return a vector with moves found in the opening book. If no moves can be
    found from the book, an empty vector is returned.*/

    int can_get_value(Engine::GameState& game_state) const;

    int get_value(Engine::GameState& game_state);
    /* Return a positive integer for a winning game_state for the player in turn,
    0 for a draw or unknown outcome and a negative integer for a loss. A win at move 42
    gives the value 1, a win at move 41 gives the value 2 etc, and vice versa for losses.*/

private:
    void load_opening_book_file(std::string file_name, bool values);

    int negamax(Engine::GameState& game_state);

    const int max_ply_for_values_in_opening_book = 8;
    std::unordered_map<uint64_t, std::string> opening_book_moves;
    std::unordered_map<uint64_t, std::string> opening_book_values;
};
}

#endif
