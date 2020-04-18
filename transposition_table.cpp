#include "transposition_table.h"

void TranspositionTable::reset()
{
    tt_lower_bound.clear();
    tt_beta_cutoff_move.clear();
}

bool TranspositionTable::lower_bound_available(std::string key)
{
    return tt_lower_bound.count(key) == 1;
}

void TranspositionTable::set_lower_bound(std::string key, int lower_bound)
{
    tt_lower_bound[key] = lower_bound;
}

int TranspositionTable::get_lower_bound(std::string key)
{
    return tt_lower_bound[key];
}

bool TranspositionTable::beta_cutoff_move_available(std::string key)
{
    return tt_beta_cutoff_move.count(key) == 1;
}

void TranspositionTable::set_beta_cutuff_move(std::string key, int beta_cutoff_move)
{
    tt_beta_cutoff_move[key] = beta_cutoff_move;
}

int TranspositionTable::get_beta_cutoff_move(std::string key)
{
    return tt_beta_cutoff_move[key];
}
