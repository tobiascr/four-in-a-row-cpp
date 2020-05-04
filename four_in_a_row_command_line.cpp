#include <iostream>
#include "engine_API.h"

using namespace Engine;

void print_board(EngineAPI& engine, bool o_make_first_move)
{
    int row;
    int col;
    std::cout << std::endl;
    for (row=5; row>=0; row--)
    {
        for (col=0; col<=6; col++)
        {
            std::cout << "|";
            if (engine.get_value(col, row) == '0')
                std::cout << " ";
            if (engine.get_value(col, row) == '1')
            {
                if (o_make_first_move)
                    std::cout << "o";
                else
                    std::cout << "*";
            }
            if (engine.get_value(col, row) == '2')
            {
                if (o_make_first_move)
                    std::cout << "*";
                else
                    std::cout << "o";
            }
        }
        std::cout << "|" << std::endl;
    }
    std::cout << " 0 1 2 3 4 5 6" << std::endl << std::endl;
}

int main()
{
    EngineAPI engine;
    char answer[20];
    int move;
    bool player_make_first_move = true;
    bool player_in_turn = true;
    bool game_over = false;
    int player_wins = 0;
    int computer_wins = 0;

    std::cout << "To play four in a row, enter the number for the column of choice." << std::endl;
    std::cout << std::endl;
    std::cout << "Print q to quit." << std::endl;
    std::cout << std::endl;

    while (true)
    {
        std::cout << "Difficulty level (1-3): ";
        std::cin >> answer;
        if (answer[0] == '1')
        {
            engine.set_difficulty_level(1);
            break;
        }
        if (answer[0] == '2')
        {
            engine.set_difficulty_level(2);
            break;
        }
        if (answer[0] == '3')
        {
            engine.set_difficulty_level(3);
            break;
        }
        if (answer[0] == 'q')
            return 0;
    }

    while (true)
    {
        if (player_in_turn)
        {
            // Player makes a move.
            print_board(engine, player_make_first_move);
            move = -1;
            while (true)
            {
                std::cout << "Input: ";
                std::cin >> answer;

                if (answer[0] == '0')
                    move = 0;
                if (answer[0] == '1')
                    move = 1;
                if (answer[0] == '2')
                    move = 2;
                if (answer[0] == '3')
                    move = 3;
                if (answer[0] == '4')
                    move = 4;
                if (answer[0] == '5')
                    move = 5;
                if (answer[0] == '6')
                    move = 6;
                if (answer[0] == 'q')
                    return 0;

                if (0 <= move and move <= 6)
                {
                    if (engine.legal_move(move))
                        break;
                    else
                        std::cout << "Invalid move." << std::endl;
                        move = -1;
                }
            }
            engine.make_move(move);
        }
        else
        {
            // Computer makes a move.
            move = engine.engine_move();
            engine.make_move(move);
        }

        // If four in a row.
        if (engine.four_in_a_row())
        {
            game_over = true;
            print_board(engine, player_make_first_move);
            if (player_in_turn)
            {
                std::cout << "You win!" << std::endl;
                player_wins++;
            }
            else
            {
                std::cout << "Computer win." << std::endl;
                computer_wins++;
            }
        }
        else // If draw.
            if (engine.board_full())
            {
                game_over = true;
                print_board(engine, player_make_first_move);
                std::cout << "Draw" << std::endl;
            }

        player_in_turn = not player_in_turn;

        if (game_over)
        {
            std::cout << std::endl;
            while (true)
            {
                std::cout << "Player - Computer: " << player_wins
                          << " - " << computer_wins <<  std::endl << std::endl;
                std::cout << "Play again (y/n)? ";
                std::cin >> answer;
                if (answer[0] == 'y')
                {
                    game_over = false;
                    engine.new_game();
                    player_make_first_move = not player_make_first_move;
                    if (player_make_first_move)
                        player_in_turn = true;
                    else
                        player_in_turn = false;
                    break;
                }
                if (answer[0] == 'n')
                    return 0;
                if (answer[0] == 'q')
                    return 0;
            }
        }
    }

    return 0;
}
