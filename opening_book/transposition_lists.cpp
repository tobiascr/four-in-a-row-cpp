#include <iostream>
#include <unordered_map>
#include "../game_state.h"

// This program produces lists of transpositions. Mirrored versions of transpositions
// are not included.

// Compilation and linking:
// g++ -O3 -c transposition_lists.cpp
// g++ -o transposition_lists transposition_lists.o ../game_state.o

int main()
{
    Engine::GameState game_state;
    std::unordered_map<uint64_t, int> transpositions;
    std::unordered_map<uint64_t, int> tr_1; // 1 move transpositions.
    std::unordered_map<uint64_t, int> tr_10; // 10 move transpositions.

    int number_of_transpositions = 0;


//    for (int move=0; move<=6; move++)
//    {
//    
//    }


    for (int m1=0; m1<=6; m1++)
    {
     std::cout << m1/7.0 << " %" << std::endl;
     for (int m2=0; m2<=6; m2++)
      for (int m3=0; m3<=6; m3++)
       for (int m4=0; m4<=6; m4++)
        for (int m5=0; m5<=6; m5++)
         for (int m6=0; m6<=6; m6++)
//          for (int m7=0; m7<=6; m7++)
//           for (int m8=0; m8<=6; m8++)
//            for (int m9=0; m9<=6; m9++)
//             for (int m10=0; m10<=6; m10++)
             {
                 // Produce a game state.
                 game_state.reset();
                 game_state.make_move(m1);
                 game_state.make_move(m2);
                 game_state.make_move(m3);
                 game_state.make_move(m4);
                 game_state.make_move(m5);
                 game_state.make_move(m6);
//                 game_state.make_move(m7);
//                 if(game_state.four_in_a_row()) {continue;};
//                 game_state.make_move(m8);
//                 if(game_state.four_in_a_row()) {continue;};
//                 game_state.make_move(m9);
//                 if(game_state.four_in_a_row()) {continue;};
//                 game_state.make_move(m10);
//                 if(game_state.four_in_a_row()) {continue;};
//                 if(game_state.can_win_this_move()) {continue;;};
                 uint64_t key = game_state.get_key();

                 // Produce a mirrored game state.
                 game_state.reset();
                 game_state.make_move(6 - m1);
                 game_state.make_move(6 - m2);
                 game_state.make_move(6 - m3);
                 game_state.make_move(6 - m4);
                 game_state.make_move(6 - m5);
                 game_state.make_move(6 - m6);
//                 game_state.make_move(6 - m7);
//                 game_state.make_move(6 - m8);
//                 game_state.make_move(6 - m9);
//                 game_state.make_move(6 - m10);

                 uint64_t mirrored_key = game_state.get_key();

                 if ((transpositions.count(key) == 0) and (transpositions.count(mirrored_key) == 0))
                 {
                     transpositions[key] = 0;
                     number_of_transpositions++;
                 }
             }
    }

    std::cout << "Number of transpositions: " << number_of_transpositions << std::endl;

    return 0;
}
