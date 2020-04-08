#ifndef TRANSPOSTION_TABLE_H
#define TRANSPOSTION_TABLE_H

#include <unordered_map>
#include <string>

class TranspositionTable
{
public:
    void reset();

    bool move_score_available(std::string key);

    void set_move_score(std::string key, int move_score);

    int get_move_score(std::string key);

    bool best_move_available(std::string key);

    void set_best_move(std::string key, int best_move);

    int get_best_move(std::string key);

private:
    std::unordered_map<std::string, int> tt_move_score;
    std::unordered_map<std::string, int> tt_best_move;
};

#endif
