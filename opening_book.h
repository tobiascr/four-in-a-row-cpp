#ifndef OPENING_BOOK_H
#define OPENING_BOOK_H

#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

namespace Engine
{

class OpeningBook
{
public:
    OpeningBook();

/*    bool can_find_best_moves_from_opening_book() const;*/

/*    std::vector<int> get_best_moves();*/

    std::unordered_map<uint64_t, std::string> opening_book_values;
    std::unordered_map<uint64_t, std::string> opening_book_moves;

private:
    void load_opening_book_file(std::string file_name, bool values);

    const int max_ply_for_values_in_opening_book = 8;
};
}

#endif
