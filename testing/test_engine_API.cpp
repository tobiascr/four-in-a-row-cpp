#include <algorithm>
#include <fstream>
#include "test_engine_API.h"
#include "test_game_state.h"

namespace TestEngine
{

EngineAPI::EngineAPI()
{
    // Initialize the random number generator.
    std::random_device rd;
    random_generator.seed(rd());
    difficulty_level_ = 2;
    load_opening_book("opening_book_3_move");
    load_opening_book("opening_book_6_move");
    load_opening_book("opening_book_9_move");
}

EngineAPI::EngineAPI(unsigned int seed)
{
    // Initialize the random number generator.
    random_generator.seed(seed);
    difficulty_level_ = 2;
    load_opening_book("opening_book_3_move");
    load_opening_book("opening_book_6_move");
    load_opening_book("opening_book_9_move");
}

void EngineAPI::load_opening_book(std::string file_name)
{
    std::ifstream file_to_read(file_name);
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
 The value is based on the number of open four in a rows.*/
{
//    if(game_state.column_not_full(move))
    {
        int player = game_state.get_number_of_moves() % 2;
        game_state.make_move(move);
        int value = EngineAPI::open_four_in_a_row_count(player);
        game_state.undo_move(move);
        return value;
    }
//    else
//    {
//        return 0;
//    }
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

//short int EngineAPI::negamax(const short int depth, short int alpha, short int beta)
short int EngineAPI::negamax(const short int depth, short int alpha, short int beta,
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
    short int original_alpha = alpha;

    if (game_state.get_number_of_moves() == depth - 1)
    {
        return 0;
    }

    if(use_opening_book)
    {
        if (game_state.get_number_of_moves() <= max_number_of_moves_in_opening_book)
        {
            key = game_state.get_key();
            if (opening_book.count(key) == 1)
            {
                return opening_book[key];
            }
        }
    }

    const bool use_transposition_table = game_state.get_number_of_moves() < depth - 6;
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
            short int tt_type = tt_entry & 1;
            short int tt_value = ((tt_entry & 0b11111110) >> 1) - 50;
            short int tt_depth = (tt_entry & 0b11111100000000) >> 8;

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

    // Move order.
    std::array<int,7> moves = {3, 2, 4, 1, 5, 0, 6};
    if (game_state.get_number_of_moves() < depth - 15)
    if (game_state.get_number_of_moves() < 22) //22
    {
        moves = move_order_open_four_in_a_row();
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

    for (int i=0; i<=6; i++)
    {
        short int move = moves[i];
        if (non_losing_moves[move])
        {
            game_state.make_move(move);
            short int value = -negamax(depth, -beta, -alpha, use_opening_book);
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

std::array<int,2> EngineAPI::root_negamax(const short int depth,
                  std::array<int,7> move_order, short int alpha, short int beta,
                  const bool use_opening_book)
/* Return a move (0 to 6) and a value for the current game state computed
with the negamax algorithm. Depth is counted as the move number at which
the search is stopped. This function can only be used on if the
game state have no four in a row and the player in turn can't make a four
in a row.*/
{
    int new_value, move, best_move;

    for (int n=0; n<=6; n++)
    {
        move = move_order[n];
        if (game_state.column_not_full(move))
        {
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
//            if (new_value >= beta) // Fail hard beta-cutoff.
//            {
//                return {move, beta};
//            }
            if (new_value > alpha)
            {
                alpha = new_value;
                best_move = move;
            }
        }
    }
    return {best_move, alpha};
}

std::array<int,2> EngineAPI::iterative_deepening(const short int depth,
                  std::array<int,7> move_order_, short int alpha, short int beta,
                  const bool use_opening_book)
{
    int d = game_state.get_number_of_moves() + 2;
    while (d < depth)
    {
        std::array<int,2> values = root_negamax(d, move_order_, alpha, beta,
                                                use_opening_book);
        int best_move = values[0];
        move_order_ = move_order(best_move);
        if(values[1] != 0)
        {
            alpha == values[1];
        }
        d++;
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
        if (game_state.get_number_of_moves() <= max_number_of_moves_in_opening_book)
        {
            return negamax(max_number_of_moves_in_opening_book + 2, -1000, 1000, true);
        }
    }

    short int alpha = -1000, beta = 1000, depth = 42;
    std::array<int,7> moves = move_order(3);
    std::array<int,2> values = iterative_deepening(depth, moves, alpha, beta,
                                                   use_opening_book);
    return values[1];
}

int EngineAPI::engine_move(const short int depth, const bool use_opening_book)
/* Return an integer from 0 to 6 that represents a best move made by the engine
at the given depth level. Depth is counted as the move number at which the search
is stopped. For example, depth=42 give a maximum depth search.*/
{
    short int alpha = -1000;
    short int beta = 1000;

//    alpha = -1;
//    beta = 1;

    std::array<int,7> moves = move_order();

    if (depth == 42)
    {
        moves = move_order(3);
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
        if (game_state.get_number_of_moves() < max_number_of_moves_in_opening_book)
        {
            moves = move_order();
            std::array<int,2> values = iterative_deepening(
                  max_number_of_moves_in_opening_book + 2, moves, alpha, beta, true);
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

int EngineAPI::random_win_not_lose_move()
{
    return 0;
}

int EngineAPI::random_best_opening_move()
/* Return a randomly chosen move with the highest possible position value.
This function is indented to be used for ply levels that are completely
covered by the opening book.*/
{
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
    if (game_state.get_number_of_moves() < 9)
    {
        return random_best_opening_move();
    }
    int moves = game_state.get_number_of_moves();
    short int depth;
    if (moves < 10)
    {
        depth = moves + 4;
    }
    else
    {
        depth = moves + 2;
    }
    if (depth > 42) {depth = 42;}
    return engine_move(depth, true);
}

int EngineAPI::engine_move_medium()
{
    short int depth = game_state.get_number_of_moves() + 10;
    if (depth > 42) {depth = 42;}
    return engine_move(depth, true);
}

int EngineAPI::engine_move_hard()
{
    if (game_state.get_number_of_moves() < 9)
    {
        return random_best_opening_move();
    }
    return engine_move(42, true);
}
}
