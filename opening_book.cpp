#include "opening_book.h"
#include "game_state.h"

namespace Engine
{

OpeningBook::OpeningBook()
{
    load_opening_book_file("/usr/local/share/four_in_a_row_opening_book/opening_book_3_ply_values", true);
    load_opening_book_file("/usr/local/share/four_in_a_row_opening_book/opening_book_6_ply_values", true);
    load_opening_book_file("/usr/local/share/four_in_a_row_opening_book/opening_book_8_ply_values", true);
    load_opening_book_file("/usr/local/share/four_in_a_row_opening_book/opening_book_8_ply_best_moves", false);
    load_opening_book_file("/usr/local/share/four_in_a_row_opening_book/opening_book_9_ply_best_moves", false);
    load_opening_book_file("/usr/local/share/four_in_a_row_opening_book/opening_book_10_ply_best_moves", false);
//    load_opening_book_file("opening_book/tr_list_pl_1_any_pl_2_best_11_ply_-2_0_1_best_moves", false);
//    load_opening_book_file("opening_book/tr_list_pl_1_any_pl_2_best_11_ply_3_best_moves", false);
//    load_opening_book_file("opening_book/tr_list_pl_1_best_pl_2_any_12_ply_2_4_6_best_moves", false);
//    load_opening_book_file("opening_book/tr_list_pl_1_any_pl_2_best_13_ply_-2_best_moves", false);
//    load_opening_book_file("opening_book/tr_list_pl_1_any_pl_2_best_13_ply_0_best_moves", false);
//    load_opening_book_file("opening_book/tr_list_pl_1_any_pl_2_best_13_ply_1_best_moves", false);
//    load_opening_book_file("opening_book/best_move_transpositions_14_ply_best_moves", false);
//    load_opening_book_file("opening_book/best_move_transpositions_15_ply_best_moves", false);
//    load_opening_book_file("opening_book/best_move_transpositions_16_ply_best_moves", false);
//    load_opening_book_file("/usr/local/share/opening_book_12_move", true);
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
        key = game_state.get_key();
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
    game_state.reset();
}
}
