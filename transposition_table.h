#ifndef TRANSPOSTION_TABLE_H
#define TRANSPOSTION_TABLE_H

#include <unordered_map>
#include <string>

class TranspositionTable
{
public:
    void reset();

    bool lower_bound_available(std::string key);

    void set_lower_bound(std::string key, int lower_bound);

    int get_lower_bound(std::string key);

    bool beta_cutoff_move_available(std::string key);

    void set_beta_cutuff_move(std::string key, int beta_cutoff_move);

    int get_beta_cutoff_move(std::string key);

private:
    std::unordered_map<std::string, int> tt_lower_bound;
    std::unordered_map<std::string, int> tt_beta_cutoff_move;
};

#endif
