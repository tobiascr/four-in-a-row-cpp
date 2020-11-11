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

int EngineAPI::negamax(const int depth, int alpha, int beta)
/* Compute a value of game_state. Return a positive integer for a winning
game_state for the player in turn, 0 for a draw or unknown outcome and a
negative integer for a loss. A win at move 42 give the value 1, a win at move 41
give a the value 2 etc, and vice versa for losses.
Depth is counted as the move number at which the search is stopped. For example,
depth=42 give a maximum depth search. This function can only be used if the
game state has no four in a row and the player in turn can't make a four
in a row.*/
{
    uint64_t key;
    int original_alpha = alpha;

    bool use_transposition_table = game_state.get_number_of_moves() < depth - 5;

    if (use_transposition_table)
    {
        key = game_state.get_key();
        if (transposition_table.count(key) == 1)
        {
            /* Transposition table data are stored in an unsigned integer.
            The first 7 bits store value + 50. 50 is added to guarantee that a
            positive integer is stored. The next 6 bits store depth.*/
            uint_fast16_t tt_entry = transposition_table[key];
            int tt_value = (tt_entry & 0b1111111) - 50;
            int tt_depth = (tt_entry & 0b1111110000000) >> 7;

            if(tt_value != 0 or tt_depth >= depth)
            {
                if (tt_value >= beta)
                {
                   return beta;
                }
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
    if (game_state.get_number_of_moves() < depth - 10)
    {
        moves = move_order_open_four_in_a_row();
    }

    int value;
    for(int move : moves)
    {
        if (non_losing_moves[move])
        {
            game_state.make_move(move);
            value = -negamax(depth, -beta, -alpha);
            game_state.undo_move(move);
            if (value >= beta)
            {
                if (use_transposition_table) // Lower bounds
                {
                    transposition_table[key] = (depth << 7) | (beta + 50);
                }
                return beta;
            }
            if (value > alpha)
            {
                alpha = value;
            }
        }
    }

    return alpha;
}

std::array<int,2> EngineAPI::root_negamax(const int depth,
                  std::array<int,7> move_order, int alpha, int beta)
/* Return a move (0 to 6) and a value for the current game state computed
with the negamax algorithm. Depth is counted as the move number at which
the search is stopped. This function can only be used if the
game state has no four in a row and the player in turn can't make a four
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
                new_value = -negamax(depth, -beta, -alpha);
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

int EngineAPI::iterative_deepening_full_depth_value()
/* Return a value for the current game state. It's best to not use for boards that are
almost full, to avoid problematic edge cases. This function can only be used if the
game state has no four in a row and the player in turn can't make a four
in a row.*/
{
    int best_move;
    const int alpha = -1;
    const int beta = 1;
    bool beginning_player_in_turn = game_state.get_number_of_moves() % 2 == 0;
    int d = game_state.get_number_of_moves() + 2;
    int value = negamax(d, alpha, beta);
    if(value < 0)
    {
        return d - 43;
    }

    // Increase d to it's closest larger even number.
    d += 2 - d % 2;

    while (d <= 42)
    {
        value = negamax(d, alpha, beta);

        // If win.
        if(value > 0)
        {
            if(beginning_player_in_turn)
            {
                return 44 - d;
            }
            else
            {
                return 43 - d;
            }
        }

        // If loss.
        if(value < 0)
        {
            if(beginning_player_in_turn)
            {
                return d - 43;
            }
            else
            {
                return d - 44;
            }
        }

        // Only every second ply level can be a win and every second a loss.
        d += 2;
    }
    // If draw.
    return 0;
}

int EngineAPI::iterative_deepening_full_depth_move(std::array<int,7> move_order_)
/* Return a move (0 to 6). It's best to not use for boards that are
almost full, to avoid problematic edge cases. This function can only be used if the
game state has no four in a row and the player in turn can't make a four
in a row.*/
{
    int value, best_move;
    int alpha = -1;
    int beta = 1;
    int d = game_state.get_number_of_moves() + 2;
    std::array<int,2> values = root_negamax(d, move_order_, alpha, beta);
    best_move = values[0];
    value = values[1];

    // If win or loss
    if(value != 0)
    {
        return {best_move};
    }

    d += 1;

    while (d <= 42)
    {
        // Every other ply level it's not possible to win and every other
        // it's not possible to lose.
        if((game_state.get_number_of_moves() + d) % 2)
        {
            // Look for a win
            alpha = 0;
            beta = 1;

            values = root_negamax(d, move_order_, alpha, beta);
            value = values[1];

            if(value > 0)
            {
                return {values[0]};
            }
        }
        else
        {
            // Look to avoid a loss
            alpha = -1;
            beta = 0;

            values = root_negamax(d, move_order_, alpha, beta);
            value = values[1];

            if(value < 0)
            {
                return {best_move};
            }

            best_move = values[0];
        }
        d += 1;
    }
    // If draw.
    return best_move;
}

int EngineAPI::position_value_full_depth(const bool use_opening_book)
{
    transposition_table.clear();

    if(game_state.four_in_a_row())
    {
        return game_state.get_number_of_moves() - 43;
    }

    if(game_state.board_full())
    {
        return 0;
    }

    if(game_state.can_win_this_move())
    {
        return 42 - game_state.get_number_of_moves();
    }

    if(use_opening_book)
    {
        if(opening_book.can_get_value(game_state))
        {
            return opening_book.get_value(game_state);
        }
    }

    int alpha = -1000, beta = 1000, depth = 42;

    // This is a way of taking care of possible edge case problems with
    // iterative deepening.
    if(game_state.get_number_of_moves() > 36)
    {
        return negamax(depth, alpha, beta);
    }

    return iterative_deepening_full_depth_value();
}

int EngineAPI::engine_move(const int depth)
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
    typically the first move. Clearing the table makes it easier to use.*/
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

    std::array<int,2> values;
    if(depth == 42 and game_state.get_number_of_moves() < 37)
    {
        return iterative_deepening_full_depth_move(moves);
    }
    else
    {
        values = root_negamax(depth, moves, alpha, beta);
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

int EngineAPI::engine_move_random()
{
    return random_move();
}

int EngineAPI::engine_move_easy()
{
    if (game_state.get_number_of_moves() < 8)
    {
        return engine_move(game_state.get_number_of_moves() + 4);
    }
    return engine_move(game_state.get_number_of_moves() + 2);
}

int EngineAPI::engine_move_medium()
{
    int depth = game_state.get_number_of_moves() + 10;
    if (depth > 42) {depth = 42;}
    return engine_move(depth);
}

int EngineAPI::engine_move_hard()
{
    // The transpositions 333331 and 333335 are easy to to play well against even
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

    std::vector<int> best_moves = opening_book.get_best_moves(game_state);
    if(not best_moves.empty())
    {
        std::uniform_int_distribution<> uid(0, best_moves.size() - 1);
        return best_moves[uid(random_generator)];
    }

    return engine_move(42);
}
}
