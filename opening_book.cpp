#include <iostream>
#include "opening_book.h"
#include "game_state.h"

namespace Engine
{

OpeningBook::OpeningBook()
{
    load_opening_book_file("/usr/local/share/four_in_a_row_opening_book/opening_book_3_ply.values", true);
    load_opening_book_file("/usr/local/share/four_in_a_row_opening_book/opening_book_6_ply.values", true);
    load_opening_book_file("/usr/local/share/four_in_a_row_opening_book/opening_book_8_ply.values", true);
    load_opening_book_file("/usr/local/share/four_in_a_row_opening_book/opening_book_8_ply.best_moves", false);
    load_opening_book_file("/usr/local/share/four_in_a_row_opening_book/opening_book_9_ply.best_moves", false);
    load_opening_book_file("/usr/local/share/four_in_a_row_opening_book/opening_book_10_ply.best_moves", false);
    load_opening_book_file("/usr/local/share/four_in_a_row_opening_book/opening_book_11_ply.best_moves", false);
    load_opening_book_file("/usr/local/share/four_in_a_row_opening_book/opening_book_12_ply.best_moves", false);
    load_opening_book_file("/usr/local/share/four_in_a_row_opening_book/opening_book_13_ply_value_0.best_moves", false);
    load_opening_book_file("/usr/local/share/four_in_a_row_opening_book/opening_book_13_ply_value_1.best_moves", false);
    load_opening_book_file("/usr/local/share/four_in_a_row_opening_book/opening_book_13_ply_value_-2.best_moves", false);
    load_opening_book_file("/usr/local/share/four_in_a_row_opening_book/opening_book_14_ply_value_2.best_moves", false);
    load_opening_book_file("/usr/local/share/four_in_a_row_opening_book/opening_book_15_ply_value_-2.best_moves", false);
    load_opening_book_file("/usr/local/share/four_in_a_row_opening_book/opening_book_16_ply_value_2.best_moves", false);
}

void OpeningBook::load_opening_book_file(std::string file_name, bool values)
/* values should be set to true if the file contains values and false if it contains
   moves.*/
{
    std::ifstream file_to_read(file_name);
    std::string line, move, c;
    uint64_t key;
    Engine::GameState game_state;

    if(not file_to_read.is_open())
    {
        std::cerr << "Can't open " << file_name << std::endl;
    }

    while (std::getline(file_to_read, line))
    {
        // Load position.
        game_state.reset();
        int space_index;
        for (int n=0; n<line.length(); n++)
        {
            if (line[n] == ' ')
            {
                space_index = n;
                break;
            }
            move = line[n];
            game_state.make_move(std::stoi(move));
        }

        // Find value.
        std::string value_string = "";
        for (int n=space_index+1; n<line.length(); n++)
        {
            c = line[n];
            value_string.append(c);
        }
        key = game_state.get_unique_key();
        if(values)
        {
            opening_book_values[key] = value_string;
        }
        else
        {
            opening_book_moves[key] = value_string;
        }
    }
    file_to_read.close();
}

std::vector<int> OpeningBook::get_best_moves(Engine::GameState& game_state)
{
    std::string book_string = "";
    std::vector<int> best_moves;
    uint64_t key = game_state.get_unique_key();

    if (opening_book_moves.count(key) == 1)
    {
        book_string = opening_book_moves[key];
        for(char move : book_string)
        {
            if (move == '0') best_moves.push_back(0);
            if (move == '1') best_moves.push_back(1);
            if (move == '2') best_moves.push_back(2);
            if (move == '3') best_moves.push_back(3);
            if (move == '4') best_moves.push_back(4);
            if (move == '5') best_moves.push_back(5);
            if (move == '6') best_moves.push_back(6);
        }
        return best_moves;
    }

    key = game_state.get_unique_mirror_key();
    if (opening_book_moves.count(key) == 1)
    {
        book_string = opening_book_moves[key];
        for(char move : book_string)
        {
           if (move == '0') best_moves.push_back(6);
           if (move == '1') best_moves.push_back(5);
           if (move == '2') best_moves.push_back(4);
           if (move == '3') best_moves.push_back(3);
           if (move == '4') best_moves.push_back(2);
           if (move == '5') best_moves.push_back(1);
           if (move == '6') best_moves.push_back(0);
        }
        return best_moves;
    }

    if(game_state.get_number_of_moves() < max_ply_for_values_in_opening_book)
    {
        std::vector<int> values = {-1000, -1000, -1000, -1000, -1000, -1000, -1000};
        int best_value = -1000;

        for (int move=0; move<=6; move++)
        {
            if (game_state.column_not_full(move))
            {
                game_state.make_move(move);
                int value = -negamax(game_state);
                game_state.undo_move(move);
                values[move] = value;
                if(value > best_value)
                {
                    best_value = value;
                }
            }
        }

        for (int move=0; move<=6; move++)
        {
            if(values[move] == best_value)
            {
                best_moves.push_back(move);
            }
        }
    }

    return best_moves;
}

int OpeningBook::can_get_value(Engine::GameState& game_state) const
{
    return game_state.get_number_of_moves() <= max_ply_for_values_in_opening_book;
}

int OpeningBook::get_value(Engine::GameState& game_state)
{
    return negamax(game_state);
}

int OpeningBook::negamax(Engine::GameState& game_state)
/* Compute a value of game_state. Return a positive integer for a winning
game_state for the player in turn, 0 for a draw or unknown outcome and a
negative integer for a loss. A win at move 42 gives the value 1, a win at move 41
gives the value 2 etc, and vice versa for losses.
Depth is counted as the move number at which the search is stopped. For example,
depth=42 give a maximum depth search.*/
{
    if(game_state.four_in_a_row())
    {
        return game_state.get_number_of_moves() - 43;
    }

    if(game_state.can_win_this_move())
    {
        return 42 - game_state.get_number_of_moves();
    }

    if(game_state.get_number_of_moves() == max_ply_for_values_in_opening_book + 1)
    {
        return 0;
    }

    std::string book_string;
    uint64_t key = game_state.get_unique_key();

    if (opening_book_values.count(key) == 1)
    {
        book_string = opening_book_values[key];
        return std::stoi(book_string);
    }
    else
    {
       key = game_state.get_unique_mirror_key();
       if (opening_book_values.count(key) == 1)
       {
          book_string = opening_book_values[key];
          return std::stoi(book_string);
       }
    }

    int best_value = -1000;
    for (int move=0; move<=6; move++)
    {
        if (game_state.column_not_full(move))
        {
            game_state.make_move(move);
            int value = -negamax(game_state);
            game_state.undo_move(move);
            if(value > best_value)
            {
                best_value = value;
            }
        }
    }

    return best_value;
}
}
