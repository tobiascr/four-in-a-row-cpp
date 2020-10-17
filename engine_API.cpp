#include <algorithm>
#include <fstream>
#include <iostream>
#include "engine_API.h"

namespace Engine
{

EngineAPI::EngineAPI()
{
    // Initialize the random number generator.
    std::random_device rd;
    random_generator.seed(rd());
    difficulty_level_ = 2;
}

EngineAPI::EngineAPI(unsigned int seed)
{
    // Initialize the random number generator.
    random_generator.seed(seed);
    difficulty_level_ = 2;
}

bool EngineAPI::can_find_best_moves_from_opening_book() const
{
    if(game_state.can_win_this_move())
    {
        return false;
    }
    if(game_state.get_number_of_moves() < 8)
    {
        return true;
    }
    return (opening_book.opening_book_moves.count(game_state.get_key()) == 1) or
           (opening_book.opening_book_moves.count(game_state.get_mirror_key()) == 1);
}

std::vector<int> EngineAPI::get_best_moves_from_opening_book() // const
/* Return a vector with moves found in the opening book. If no moves can be
   found from the book, an empty vector is returned.*/
{
    std::string book_string = "";
    std::vector<int> best_moves;
    uint64_t key = game_state.get_key();

    if (opening_book.opening_book_moves.count(key) == 1)
    {
        book_string = opening_book.opening_book_moves[key];
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
    }
    else
    {
        key = game_state.get_mirror_key();
        if (opening_book.opening_book_moves.count(key) == 1)
        {
           book_string = opening_book.opening_book_moves[key];
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
        }
    }

    return best_moves;
}

void EngineAPI::set_difficulty_level(int difficulty_level)
{
    difficulty_level_ = difficulty_level;
}

void EngineAPI::new_game()
{
    game_state.reset();
    transposition_table.clear();
}

bool EngineAPI::legal_move(int column)
{
    return game_state.column_not_full(column);
}

int EngineAPI::engine_move()
{
    if (difficulty_level_ == 1)
        return engine_move_easy();
    if (difficulty_level_ == 2)
        return engine_move_medium();
    if (difficulty_level_ == 3)
        return engine_move_hard();
    if (difficulty_level_ == 4)
        return random_move();
    return 0;
}

void EngineAPI::make_move(int position)
{
    game_state.make_move(position);
}

bool EngineAPI::board_full()
{
    return game_state.board_full();
}

char EngineAPI::get_value(int column, int row)
{
    return game_state.get_value(column, row);
}

bool EngineAPI::four_in_a_row()
{
    return game_state.four_in_a_row();
}

int EngineAPI::position_heuristic(int move) const
/* Give a heuristic evaluation in form of a number of how good it would be to make
the given move to the current game state. The value is higher the better the move.
Central positions are given higher values. If the move is not legal, the value is 0.*/
{
    if (not game_state.column_not_full(move)) {return 0;}
    int row = game_state.get_number_of_disks_in_column(move);
    const int values[6][7] =
        {{0, 0, 0, 0, 0, 0, 0},
         {0, 0, 1, 1, 1, 0, 0},
         {0, 1, 1, 1, 1, 1, 0},
         {0, 1, 1, 1, 1, 1, 0},
         {0, 0, 1, 1, 1, 0, 0},
         {0, 0, 0, 0, 0, 0, 0}};
    return values[row][move];
}

int EngineAPI::position_heuristic_2(int move) const
/* Give a heuristic evaluation in form of a number of how good it would be to make
the given move to the current game state. The value is higher the better the move.
Central positions are given higher values. If the move is not legal, the value is 0.*/
{
    if (not game_state.column_not_full(move)) {return 0;}
    int row = game_state.get_number_of_disks_in_column(move);
    const int values[6][7] =
        {{-1, 0, 0, 1, 0, 0, -1},
         {0,  1, 2, 3, 2, 1,  0},
         {0,  2, 3, 3, 3, 2,  0},
         {0,  2, 3, 3, 3, 2,  0},
         {0,  1, 2, 3, 2, 1,  0},
         {-1, 0, 0, 1, 0, 0, -1}};
    return values[row][move];
}

int EngineAPI::left_right_balance() const
{
    return game_state.get_number_of_disks_in_column(0) +
           game_state.get_number_of_disks_in_column(1) +
           game_state.get_number_of_disks_in_column(2) -
           game_state.get_number_of_disks_in_column(4) -
           game_state.get_number_of_disks_in_column(5) -
           game_state.get_number_of_disks_in_column(6);
}

int EngineAPI::adjacent_filled_position_count(int column) const
{
    int number_of_filled_positions = 0;
    int h = game_state.get_number_of_disks_in_column(column);

    if(h > 0)
    {
        number_of_filled_positions = 1;
    }

    if(column > 0)
    {
        int n = game_state.get_number_of_disks_in_column(column - 1);
        if(n == h - 1)
        {
            number_of_filled_positions += 1;
        }
        if(n == h)
        {
            number_of_filled_positions += 2;
        }
        if(n > h)
        {
            number_of_filled_positions += 3;
        }
    }

    if(column < 6)
    {
        int n = game_state.get_number_of_disks_in_column(column + 1);
        if(n == h - 1)
        {
            number_of_filled_positions += 1;
        }
        if(n == h)
        {
            number_of_filled_positions += 2;
        }
        if(n > h)
        {
            number_of_filled_positions += 3;
        }
    }

    return number_of_filled_positions;
}

std::array<int,7> EngineAPI::move_order()
/* Return a move order for a root negamax search based on a heuristic evaluation
of the current game state. There is some randomness included in the move ordering
for moves that are estimated to be equally strong.*/
{
    std::array<int,7> moves = {3, 2, 4, 1, 5, 0, 6};
    int values[7];

    // Adding randomness to the move order.
    shuffle(moves.begin(), moves.end(), random_generator);

    for (int n=0; n<=6; n++)
    {
          values[n] = position_heuristic_2(n);
//          values[n] = adjacent_filled_position_count(n);
    }

    std::stable_sort(moves.begin(), moves.end(),
                     [&values](int i, int j){return values[i] > values[j];});
    return moves;
}

std::array<int,7> EngineAPI::move_order(int first_move)
{
    switch (first_move)
    {
        case 0: return {0, 3, 2, 4, 1, 5, 6};
        case 1: return {1, 3, 2, 4, 5, 0, 6};
        case 2: return {2, 3, 4, 1, 5, 0, 6};
        case 3: return {3, 2, 4, 1, 5, 0, 6};
        case 4: return {4, 3, 2, 1, 5, 0, 6};
        case 5: return {5, 3, 2, 4, 1, 0, 6};
        case 6: return {6, 3, 2, 4, 1, 5, 0};
    }
    return {3, 2, 4, 1, 5, 0, 6};
}

std::array<int,7> EngineAPI::move_order_open_four_in_a_row()
{
    std::array<int,7> moves = {3, 2, 4, 1, 5, 0, 6};

    int values[7] = {0, 0, 0, 0, 0, 0, 0};
    int player = game_state.get_number_of_moves() % 2;

    for (int move=0; move<=6; move++)
    {
        if(game_state.column_not_full(move))
        {
            game_state.make_move(move);
            values[move] = game_state.open_four_in_a_row_count(player);
            game_state.undo_move(move);
        }
    }

    std::stable_sort(moves.begin(), moves.end(),
                     [&values](int i, int j){return values[i] > values[j];});

    return moves;
}

int EngineAPI::negamax(const int depth, int alpha, int beta,
                             const bool use_opening_book)
/* Compute a value of game_state. Return a positive integer for a winning
game_state for the player in turn, 0 for a draw or unknown outcome and a
negative integer for a loss. A win at move 42 give the value 1, a win at move 41
give a the value 2 etc, and vice versa for losses.
Depth is counted as the move number at which the search is stopped. For example,
depth=42 give a maximum depth search. This function can only be used on if the
game state have no four in a row and the player in turn can't make a four
in a row.*/
{
    uint64_t key;
    int original_alpha = alpha;

    if (use_opening_book)
    {
        if (game_state.get_number_of_moves() <= max_ply_for_values_in_opening_book)
        {
            std::string book_string;
            key = game_state.get_key();

            if (opening_book.opening_book_values.count(key) == 1)
            {
                book_string = opening_book.opening_book_values[key];
                return std::stoi(book_string);
            }
            else
            {
                key = game_state.get_mirror_key();
                if (opening_book.opening_book_values.count(key) == 1)
                {
                    book_string = opening_book.opening_book_values[key];
                    return std::stoi(book_string);
                }
            }
        }
    }

    const bool use_transposition_table = (game_state.get_number_of_moves() < depth - 6);

//    const bool use_transposition_table = (game_state.get_number_of_moves() < 31);

    if (use_transposition_table)
    {
        key = game_state.get_key();
        if (transposition_table.count(key) == 1)
        {
            /* Transposition table data are stored in an unsigned integer.
            The least significant bit is a store type. The next 7 bits store
            value + 50. 50 is added to guarantee that a positive integer is stored.
            The next 6 bits store depth.*/
            uint_fast16_t tt_entry = transposition_table[key];
            int tt_type = tt_entry & 1;
            int tt_value = ((tt_entry & 0b11111110) >> 1) - 50;
            int tt_depth = (tt_entry & 0b11111100000000) >> 8;

            if (tt_type == 0)
            {
                return tt_value;
            }

            if (tt_value >= beta and (tt_value != 0 or tt_depth >= depth))
            {
                return beta;
            }
        }
    }

    std::array<bool,7> non_losing_moves = game_state.get_non_losing_moves();
    int c = 0;
    for (int i=0; i<=6; i++)
    {
        if(non_losing_moves[i])
        {
            c++;
            break;
        }
    }
    if (c == 0) {return game_state.get_number_of_moves() - 41;}

    if (game_state.get_number_of_moves() >= depth - 2)
    {
        return 0;
    }

    // Move order.
    std::array<int,7> moves = {3, 2, 4, 1, 5, 0, 6};
    if (game_state.get_number_of_moves() < depth - 12) // 12
    {
        moves = move_order_open_four_in_a_row();
    }

    for (int i=0; i<=6; i++)
    {
        int move = moves[i];
        if (non_losing_moves[move])
        {
            game_state.make_move(move);
            int value = -negamax(depth, -beta, -alpha, use_opening_book);
            game_state.undo_move(move);
            if (value >= beta) // Fail hard beta-cutoff.
            {
                if (use_transposition_table)
                {
                    transposition_table[key] = (depth << 8) | ((beta + 50) << 1) | 1;
                }
                return beta;
            }
            if (value > alpha)
            {
                alpha = value;
            }
        }
    }

    if (use_transposition_table)
    {
        if (alpha > original_alpha) // Exact values.
        {
            if (alpha != 0)
            {
                transposition_table[key] = (depth << 8) | ((alpha + 50) << 1);
            }
        }
    }

    return alpha;
}

std::array<int,2> EngineAPI::root_negamax(const int depth,
                  std::array<int,7> move_order, int alpha, int beta,
                  const bool use_opening_book)
/* Return a move (0 to 6) and a value for the current game state computed
with the negamax algorithm. Depth is counted as the move number at which
the search is stopped. This function can only be used on if the
game state have no four in a row and the player in turn can't make a four
in a row.*/
{
    int new_value, move, best_move;
    bool flag = true;

    for (int n=0; n<=6; n++)
    {
        move = move_order[n];
        if (game_state.column_not_full(move))
        {
            if (flag)
            {
                best_move = move;
                flag = false;
            }
            game_state.make_move(move);
            if(game_state.can_win_this_move())
            {
                new_value = game_state.get_number_of_moves() - 42;
            }
            else
            {
                new_value = -negamax(depth, -beta, -alpha, use_opening_book);
            }
            game_state.undo_move(move);
            if (new_value >= beta) // Fail hard beta-cutoff.
            {
                return {move, beta};
            }
            if (new_value > alpha)
            {
                alpha = new_value;
                best_move = move;
            }
        }
    }
    return {best_move, alpha};
}

std::array<int,2> EngineAPI::iterative_deepening(const int depth,
                  std::array<int,7> move_order_, int alpha, int beta,
                  const bool use_opening_book)
{
    int d = game_state.get_number_of_moves() + 3; // 3
    while (d < depth)
    {
        std::array<int,2> values = root_negamax(d, move_order_, alpha, beta,
                                                use_opening_book);
        if(values[1] > 0)
        {
            return values;
        }
        if(values[1] != 0)
        {
            if (values[1] > alpha)
            {
                alpha == values[1];
            }
        }
        d = d + 4; //4
    }
    return root_negamax(depth, move_order_, alpha, beta, use_opening_book);
}

int EngineAPI::position_value_full_depth(const bool use_opening_book)
{
    transposition_table.clear();
    if(game_state.can_win_this_move())
    {
        return 42 - game_state.get_number_of_moves();
    }
    if(use_opening_book)
    {
        if (game_state.get_number_of_moves() <= max_ply_for_values_in_opening_book)
        {
            return negamax(max_ply_for_values_in_opening_book + 2, -1000, 1000, true);
        }
    }

    int alpha = -1000, beta = 1000, depth = 42;

    std::array<int,7> moves = move_order(3);
    std::array<int,2> values = iterative_deepening(depth, moves, alpha, beta,
                                                   use_opening_book);
    return values[1];
}

int EngineAPI::engine_move(const int depth, const bool use_opening_book)
/* Return an integer from 0 to 6 that represents a best move made by the engine
at the given depth level. Depth is counted as the move number at which the search
is stopped. For example, depth=42 give a maximum depth search.*/
{
    int alpha = -1000;
    int beta = 1000;

    std::array<int,7> moves = move_order();

    if (depth == 42)
    {
        moves = move_order_open_four_in_a_row();
    }

    /* Clearing the transposition table between moves makes the total move time a
    little slower. But the maximum move is not likely to be affected, since it's
    typically the first move. Clearing table makes it easier to use.*/
    transposition_table.clear();

    // Look for a move that makes a four in a row.
    for (int n=0; n<=6; n++)
    {
        int move = moves[n];
        if (game_state.column_not_full(move))
        {
            game_state.make_move(move);
            int result = game_state.four_in_a_row();
            game_state.undo_move(move);
            if (result) {return move;}
        }
    }

    // Look for blocking moves.
    for (int n=0; n<=6; n++)
    {
        int move = moves[n];
        if (game_state.column_not_full(move))
        {
            if (game_state.is_blocking_move(move)) {return move;}
        }
    }

    if(use_opening_book)
    {
        if (game_state.get_number_of_moves() < max_ply_for_values_in_opening_book)
        {
            std::array<int,2> values = iterative_deepening(
                  max_ply_for_values_in_opening_book + 2, moves, alpha, beta, true);
            return values[0];
        }
    }

    std::array<int,2> values;
    if(depth == 42)
    {
        values = iterative_deepening(depth, moves, alpha, beta, use_opening_book);
    }
    else
    {
        values = root_negamax(depth, moves, alpha, beta, use_opening_book);
    }

    return values[0];
}

int EngineAPI::random_move()
{
    std::array<int,7> moves = {0, 1, 2, 3, 4, 5, 6};
    shuffle(moves.begin(), moves.end(), random_generator);
    for(int move : moves)
    {
        if (game_state.column_not_full(move))
        {
            return move;
        }
    }
    return 0;
}

int EngineAPI::random_best_opening_move()
/* Return a randomly chosen move with the highest possible position value.
This function is indented to be used for ply levels that are completely
covered by the opening book.*/
{
    if(game_state.can_win_this_move())
    {
        return engine_move(42, true);
    }

    std::array<int,7> moves = {0, 1, 2, 3, 4, 5, 6};
    shuffle(moves.begin(), moves.end(), random_generator);
    int value, best_move;
    int best_value = -1000;

    for(int move : moves)
    {
        if(game_state.column_not_full(move))
        {
            if(not game_state.four_in_a_row())
            {
                game_state.make_move(move);
                value = -position_value_full_depth();
                if(value > best_value)
                {
                    best_value = value;
                    best_move = move;
                }
                game_state.undo_move(move);
            }
        }
    }

    return best_move;
}

int EngineAPI::engine_move_random()
{
    return random_move();
}

int EngineAPI::engine_move_easy()
{
    if (game_state.get_number_of_moves() < 8)
    {
        return engine_move(game_state.get_number_of_moves() + 4, false);
    }
    return engine_move(game_state.get_number_of_moves() + 2, false);
}

int EngineAPI::engine_move_medium()
{
    int depth = game_state.get_number_of_moves() + 10;
    if (depth > 42) {depth = 42;}
    return engine_move(depth, true);
}

int EngineAPI::engine_move_hard()
{
    // The transpositions 33331 and 33335 are easy to to play well against even
    // for a weak opponent and are therefore avoided.
    if (game_state.get_number_of_moves() == 5)
    {
        if(game_state.get_number_of_disks_in_column(3) == 5)
        {
            std::vector<int> moves = {0, 2, 3, 4, 6};
            std::uniform_int_distribution<> uid(0, 4);
            return moves[uid(random_generator)];
        }
    }

    if (game_state.get_number_of_moves() < 8)
    {
        return random_best_opening_move();
    }

    if(can_find_best_moves_from_opening_book() and game_state.get_number_of_moves() >= 8)
    {
        std::vector<int> moves = get_best_moves_from_opening_book();
        std::uniform_int_distribution<> uid(0, moves.size() - 1);
        return moves[uid(random_generator)];
    }

    return engine_move(42, true);
}
}
