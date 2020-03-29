#ifndef ENGINE_API_H
#define ENGINE_API_H

#include "engine.h"
#include "game_state.h"

class EngineAPI
{
public:
    EngineAPI();

    void set_difficulty_level(int difficulty_level);
    // difficulty_level can be 1, 2 or 3.

    void new_game();

    bool legal_move(int position);

    int engine_move();
    // Return an integer från 0 to 6 that represents a move computed by the engine.

    void make_move(int position);

    bool board_full();

    char get_value(int column, int row);
    /* Return '1', '2' or '0' depending on the state of the corresponding position.
       '1' represents the player making the first move, '2' the player making
       second move and '0' that the position is empty.*/

    bool four_in_a_row(int column);
    /* True iff the given position is a part of a four in a row.*/

private:
    GameState game_state;
    int difficulty_level_;
};

#endif
