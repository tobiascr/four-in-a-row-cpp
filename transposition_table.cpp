#include "transposition_table.h"

void TranspositionTable::reset()
{
    tt_move_score.clear();
    tt_best_move.clear();
}

bool TranspositionTable::move_score_available(std::string key)
{
    return tt_move_score.count(key) == 1;
}

void TranspositionTable::set_move_score(std::string key, int move_score)
{
    tt_move_score[key] = move_score;
}

int TranspositionTable::get_move_score(std::string key)
{
    return tt_move_score[key];
}

bool TranspositionTable::best_move_available(std::string key)
{
    return tt_best_move.count(key) == 1;
}

void TranspositionTable::set_best_move(std::string key, int best_move)
{
    tt_best_move[key] = best_move;
}

int TranspositionTable::get_best_move(std::string key)
{
    return tt_best_move[key];
}
