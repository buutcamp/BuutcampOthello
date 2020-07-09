/*
 * Player
 * player.h
 * ver 1.0
 */

#ifndef _PLAYERH
#define _PLAYERH
#include "othello.h"

class Game;     //Must tell, who is our master

class Player {
    Game* game;     //for main class functions call example: game->OnTileClicked(int x, int y);

    public:
        Player(/* parameters */);
        ~Player();

        void OnTileClicked(int x, int y);
        void updateScore();
        void update();
        void updatePlayerTurn();
        bool resetGame();
    private:
        std::vector<std::vector<int>> GameBoard;
        int scoreWhite;
        int scoreBlack;
        int playerTurn;

        bool reset_game;
        bool pass_turn;

};

#endif      //end _PLAYERH
