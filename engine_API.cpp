#include <algorithm>
#include <fstream>
#include <string>
#include "engine_API.h"
#include "game_state.h"

namespace Engine
{

EngineAPI::EngineAPI()
{
    // Initialize the random number generator.
    std::random_device rd;
    random_generator.seed(rd());
    difficulty_level_ = 2;
    load_opening_book();
}

EngineAPI::EngineAPI(unsigned int seed)
{
    // Initialize the random number generator.
    random_generator.seed(seed);
    difficulty_level_ = 2;
    load_opening_book();
}

void EngineAPI::load_opening_book()
{
    std::ifstream file_to_read("opening_book_9_move");
    std::string line, move, c;
    uint64_t key;
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
        int value = std::stoi(value_string);
        key = game_state.get_key();
        opening_book[key] = value;

        // Load mirrored position.
        game_state.reset();
        for (int n=0; n<line.length(); n++)
        {
            if (line[n] == ' ') break;
            move = line[n];
            game_state.make_move(6 - std::stoi(move));
        }
        key = game_state.get_key();
        opening_book[key] = value;
    }
    file_to_read.close();
    game_state.reset();
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
/*Give a heuristic evaluation in form of a number of how good it would be to make
 the given move to the current game state. The value is higher the better the move.
 Central positions are given higher values. If the move is not legal, the value is 0.
*/
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
/*Give a heuristic evaluation in form of a number of how good it would be to make
 the given move to the current game state. The value is higher the better the move.
 Central positions are given higher values. If the move is not legal, the value is 0.
*/
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

int EngineAPI::open_four_in_a_row_count(int player) const
/* Return the number of unoccupied places on the board that are not in the bottom
   of the columns, that give player a four in a row. player is 0 for the player
   making the first move and 1 for the other player.*/
{
    int count = 0;
    for (int col=0; col<=6; col++)
    {
        for (int row=game_state.get_number_of_disks_in_column(col) + 1; row<=5; row++)
        {
            if (game_state.four_in_a_row(player, col, row))
            {
                count++;
            }
        }
    }
    return count;
}

int EngineAPI::open_four_in_a_row_heuristic(int move)
/* Give a heuristic evaluation in form of a number of how good it would be to make
 the given move to the current game state. The value is higher the better the move.
 The value is based on the number of open four in a rows.
*/
{
    int player = game_state.get_number_of_moves() % 2;
    game_state.make_move(move);
    int value = EngineAPI::open_four_in_a_row_count(player);
    game_state.undo_move(move);
    return value;
}

std::array<int,7> EngineAPI::move_order()
{
    std::array<int,7> moves = {3, 2, 4, 1, 5, 0, 6};
    int values[7];

    // Adding randomness to the move order.
    shuffle (moves.begin(), moves.end(), random_generator);

    for (int n=0; n<=6; n++)
    {
          values[n] = position_heuristic_2(n);
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
    int values[7];

    for (int n=0; n<=6; n++)
    {
          values[n] = open_four_in_a_row_heuristic(n);
    }

    std::stable_sort(moves.begin(), moves.end(),
                     [&values](int i, int j){return values[i] > values[j];});
    return moves;
}

short int EngineAPI::negamax(const short int depth, short int alpha, short int beta)
{
    uint64_t key;
    short int original_alpha = alpha;

    if (game_state.can_win_this_move())
    {
        return 42 - game_state.get_number_of_moves();
    }

    if (game_state.get_number_of_moves() == depth - 1)
    {
        return 0;
    }

    if (game_state.get_number_of_moves() <= max_number_of_moves_in_opening_book)
    {
        key = game_state.get_key();
        if (opening_book.count(key) == 1)
        {
            return opening_book[key];
        }
    }

    const bool use_transposition_table = depth - game_state.get_number_of_moves() > 5;
    if (use_transposition_table)
    {
        key = game_state.get_key();
        if (transposition_table.count(key) == 1)
        {
            std::array<short int, 3> tt_array = transposition_table[key];
            short int tt_depth = tt_array[0], tt_type = tt_array[1], tt_value = tt_array[2];

            if (tt_type == 2)
            {
                return tt_value;
            }

            if (tt_value >= beta and (tt_value != 0 or tt_depth >= depth))
            {
                return beta;
            }
        }
    }

    // Move order.
    std::array<int,7> moves = {3, 2, 4, 1, 5, 0, 6};
    if (depth - game_state.get_number_of_moves() > 15)
    {
        moves = move_order_open_four_in_a_row();
    }

    for (int i=0; i<=6; i++)
    {
        short int move = moves[i];
        if (game_state.column_not_full(move))
        {
            game_state.make_move(move);
            short int value = -negamax(depth, -beta, -alpha);
            game_state.undo_move(move);
            if (value >= beta) // Fail hard beta-cutoff.
            {
                if (use_transposition_table)
                {
                    transposition_table[key] = std::array<short int, 3>{depth, 1, beta};
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
                transposition_table[key] = std::array<short int, 3>{depth, 2, alpha};
            }
        }
    }

    return alpha;
}

int EngineAPI::root_negamax(const short int depth, std::array<int,7> move_order,
                            short int alpha, short int beta)
{
    int new_value, move, result, best_move;

    // Look for a move that makes a four in a row.
    for (int n=0; n<=6; n++)
    {
        move = move_order[n];
        if (game_state.column_not_full(move))
        {
            game_state.make_move(move);
            result = game_state.four_in_a_row();
            game_state.undo_move(move);
            if (result) {return move;}
        }
    }

    // Look for blocking moves.
    for (int n=0; n<=6; n++)
    {
        move = move_order[n];
        if (game_state.column_not_full(move))
        {
            if (game_state.is_blocking_move(move)) {return move;}
        }
    }

    for (int n=0; n<=6; n++)
    {
        move = move_order[n];
        if (game_state.column_not_full(move))
        {
            game_state.make_move(move);
            new_value = -negamax(depth, -beta, -alpha);
            game_state.undo_move(move);
            if (new_value > alpha)
            {
                alpha = new_value;
                best_move = move;
            }
        }
    }
    return best_move;
}

int EngineAPI::position_value(short int depth)
{
    transposition_table.clear();
    return negamax(depth, -1000, 1000);
}


int EngineAPI::engine_move(const short int depth)
{
    short int alpha = -1000;
    short int beta = 1000;

    std::array<int,7> moves = move_order();

    if (depth == 42)
    {
        moves = move_order(3);
    }

    // Clearing the transposition table between moves makes the total move time a little
    // slower. But the maximum move is not likely to be affected, since it's typically
    // the first move. Clearing table makes it easier to use.
    transposition_table.clear();

    // Iterative deepening.
    if (depth == 42)
    {
        int d = game_state.get_number_of_moves() + 2;
        while (d < depth)
        {
            int move = root_negamax(d, moves, alpha, beta);
            moves = move_order(move);
            d++;
        }
    }

    return root_negamax(depth, moves, alpha, beta);
}

int EngineAPI::engine_move_easy()
{
    int moves = game_state.get_number_of_moves();
    short int depth = moves + 2;
    if (depth > 42) {depth = 42;}
    return engine_move(depth);
}

int EngineAPI::engine_move_medium()
{
    int moves = game_state.get_number_of_moves();
    short int depth = moves + 4;
    if (depth > 42) {depth = 42;}
    return engine_move(depth);
}

int EngineAPI::engine_move_hard()
{
    int number_of_moves = game_state.get_number_of_moves();

    // Some opening moves.
    if (number_of_moves < 2) {return 3;}

    //return engine_move(42);

    if (number_of_moves >= 9)
    {
        return engine_move(42);
    }

    int depth = number_of_moves + 14;
    if (depth > 42) {depth = 42;}

    return engine_move(depth);
}
}
