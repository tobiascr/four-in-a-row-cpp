#include <algorithm>
#include <stack>
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
}

void EngineAPI::clear_transposition_table()
{
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

int EngineAPI::random_game_heuristic()
/* Give a heuristic evaluation of the current game state based on playing many
random games and count how many is won by the player in turn.*/
{
    int number_of_won_games = 0;
    for(int i = 0; i<500; i++)
    {
        int number_of_moves = 0;
        std::stack<int> move_history;

        // Play a game.
        while(not game_state.board_full())
        {
            int move = random_move();
            game_state.make_move(move);
            move_history.push(move);
            number_of_moves++;
            if(game_state.four_in_a_row())
            {
                if(number_of_moves % 2)
                {
                    number_of_won_games++;
                }
                break;
            }
        }

        // Undo the moves in the game.
        while(not move_history.empty())
        {
            game_state.undo_move(move_history.top());
            move_history.pop();
        }
    }

    return number_of_won_games;
}

std::array<int,7> EngineAPI::move_order()
{
    std::array<int,7> moves = {3, 2, 4, 1, 5, 0, 6};
    int values[7] = {1, 3, 5, 6, 4, 2, 0};
    const int player = game_state.get_player_in_turn();

    for (int move=0; move<=6; move++)
    {
        if(game_state.column_not_full(move))
        {
            game_state.make_move(move);
            values[move] += 100 * game_state.open_four_in_a_row_count(player);
            game_state.undo_move(move);
            if(game_state.own_threat_above(move))
            {
                values[move] = -100;
            }
        }
    }

    std::sort(moves.begin(), moves.end(),
                     [&values](int i, int j){return values[i] > values[j];});
    return moves;
}

std::array<int,7> EngineAPI::move_order(uint64_t moves_bitboard)
{
    std::array<int,7> moves = {3, 2, 4, 1, 5, 0, 6};
    int values[7] = {1, 3, 5, 6, 4, 2, 0};
    const int player = game_state.get_player_in_turn();
    const std::array<uint64_t,7> column = {
        0b0000000000000000000000000000000000000000000111111,
        0b0000000000000000000000000000000000001111110000000,
        0b0000000000000000000000000000011111100000000000000,
        0b0000000000000000000000111111000000000000000000000,
        0b0000000000000001111110000000000000000000000000000,
        0b0000000011111100000000000000000000000000000000000,
        0b0111111000000000000000000000000000000000000000000};

    for (int move=0; move<=6; move++)
    {
        if(moves_bitboard & column[move])
        {
            game_state.make_move(move);
            values[move] += 100 * game_state.open_four_in_a_row_count(player);
            game_state.undo_move(move);
            if(game_state.own_threat_above(move))
            {
                values[move] = -100;
            }
        }
    }

    std::sort(moves.begin(), moves.end(),
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

int EngineAPI::negamax(const int depth, int alpha, int beta)
/* Compute a value of game_state. Return a positive integer for a winning
game_state for the player in turn, 0 for a draw or unknown outcome and a
negative integer for a loss. A win at move 42 give the value 100, a win at move 41
give a the value 200 etc, and vice versa for losses. Heuristic values can be given
in the range from -99 to 99.
Depth is counted as the move number at which the search is stopped. For example,
depth=42 give a maximum depth search. This function can only be used if the
game state has no four in a row and the player in turn can't make a four
in a row.*/
{
    uint64_t unique_key;
    uint64_t key;
    const int original_alpha = alpha;

    bool use_transposition_table = game_state.get_number_of_moves() < depth - 4;
    if(beta < 1)
    {
        use_transposition_table = false;
    }

    if (use_transposition_table)
    {
        unique_key = game_state.get_unique_key();
        key = unique_key % transposition_table.size;
        const uint64_t tt_entry = transposition_table.values[key];
        const uint64_t tt_key = tt_entry  >> 15;
        if(tt_key == unique_key)
        {
            /* Transposition table data are stored in an unsigned integer.
            The first 7 bits store value + 50. 50 is added to guarantee that a
            positive integer is stored. The next 6 bits store depth.
            Bit 14 is one for lower bounds. Bit 15 is one for upper bounds.
            The next 49 bits is used for storing a unique key for the transposition.*/

            const int tt_value = (tt_entry & 0b1111111) - 50;
            const int tt_depth = (tt_entry & 0b1111110000000) >> 7;
            const bool lower_bound = tt_entry & 0b10000000000000;
            const bool upper_bound = tt_entry & 0b100000000000000;

            if(tt_depth <= depth and (tt_value != 0 or tt_depth == depth))
            {
                if(lower_bound)
                {
                    if (tt_value > alpha)
                    {
                        alpha = tt_value;
                    }
                    if(alpha >= beta)
                    {
                        return beta;
                    }
                }
                else if(upper_bound)
                {
                    if (tt_value < beta)
                    {
                        beta = tt_value;
                    }
                    if(alpha >= beta)
                    {
                        return alpha;
                    }
                }
            }
        }
    }

    const uint64_t non_losing_moves_bitboard = game_state.get_non_losing_moves();

    if (non_losing_moves_bitboard == 0) {return game_state.get_number_of_moves() - 41;}

    if (game_state.get_number_of_moves() >= depth - 2)
    {
        return 0;
    }

    const std::array<uint64_t,7> non_losing_moves = {
        non_losing_moves_bitboard & 0b0000000000000000000000000000000000000000000111111,
        non_losing_moves_bitboard & 0b0000000000000000000000000000000000001111110000000,
        non_losing_moves_bitboard & 0b0000000000000000000000000000011111100000000000000,
        non_losing_moves_bitboard & 0b0000000000000000000000111111000000000000000000000,
        non_losing_moves_bitboard & 0b0000000000000001111110000000000000000000000000000,
        non_losing_moves_bitboard & 0b0000000011111100000000000000000000000000000000000,
        non_losing_moves_bitboard & 0b0111111000000000000000000000000000000000000000000};

    // Move order.
    std::array<int,7> moves = {3, 2, 4, 1, 5, 0, 6};
    if (game_state.get_number_of_moves() < depth - 12)
    {
        moves = move_order(non_losing_moves_bitboard);
    }

    int value;
    for(int move : moves)
    {
        if (non_losing_moves[move])
        {
            game_state.make_move_fast(non_losing_moves[move]);
            value = -negamax(depth, -beta, -alpha);
            game_state.undo_move_fast(non_losing_moves[move]);
            if (value >= beta)
            {
                if (use_transposition_table) // Lower bounds
                {
                    transposition_table.values[key] = (unique_key << 15)
                                    | 0b10000000000000 | (depth << 7) | (beta + 50);
                }
                return beta;
            }
            if (value > alpha)
            {
                alpha = value;
            }
        }
    }

    if (use_transposition_table) // Upper bounds
    {
        transposition_table.values[key] = (unique_key << 15) | 0b100000000000000 |
                         (depth << 7) | (alpha + 50);
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
        // In every other ply level it's not possible to win and in every other
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

int EngineAPI::iterative_deepening_full_depth_move_likely_win(std::array<int,7> move_order_)
/* Return a move (0 to 6). It's best to not use for boards that are
almost full, to avoid problematic edge cases. This function can only be used if the
game state has no four in a row and the player in turn can't make a four
in a row. This function is optimized to find a win fast. If there is no win, it might
be slower than other algorithms.*/
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
        // In every other ply level it's not possible to win and in every other
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
        d += 1;
    }

    d = game_state.get_number_of_moves() + 3;

    while (d <= 42)
    {
        // In every other ply level it's not possible to win and in every other
        // it's not possible to lose.
        if((game_state.get_number_of_moves() + d) % 2 == 0)
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
        return iterative_deepening_full_depth_move_likely_win(moves);
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
    if (game_state.get_number_of_moves() == 5)
    {
        if(game_state.get_number_of_disks_in_column(3) == 5)
        {
            std::vector<int> moves = {0, 2, 3, 4, 6};
            std::uniform_int_distribution<> uid(0, 4);
            return moves[uid(random_generator)];
        }
    }

    if (game_state.get_number_of_moves() == 1)
    {
        if(game_state.get_number_of_disks_in_column(0) == 1)
        {
            return 3;
        }
        if(game_state.get_number_of_disks_in_column(6) == 1)
        {
            return 3;
        }
    }

    std::vector<int> best_moves = opening_book.get_best_moves(game_state);
    if(not best_moves.empty())
    {
        std::uniform_int_distribution<> uid(0, best_moves.size() - 1);
        return best_moves[uid(random_generator)];
    }

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

    // 03 and 63 gives rise faster to compute transpositions later in the game
    // and it looks to be better for a weak engine than 01 and 65.
    if (game_state.get_number_of_moves() == 1)
    {
        if(game_state.get_number_of_disks_in_column(0) == 1)
        {
            return 3;
        }
        if(game_state.get_number_of_disks_in_column(6) == 1)
        {
            return 3;
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
