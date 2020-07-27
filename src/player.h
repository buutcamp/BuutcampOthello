/*
 * Player
 * player.h
 * ver 1.0
 */

#ifndef _PLAYERH
#define _PLAYERH
#include "definet.h"
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
        int  PlayerType = -1;
        int  PlayerColor = Empty;
        int GetPlayerNumber() {
            if(PlayerColor == Black) {
                return 1;   //Black player is every time Player1
            } else if(PlayerColor == White) {
                return 2;   //White player is every time Player2
            } else {
                return 0;   //Not selected yet
            }
        }
    private:
        std::vector<std::vector<int>> GameBoard;
        int scoreWhite;
        int scoreBlack;
        int playerTurn;

        bool reset_game;
        bool pass_turn;

};

#endif      //end _PLAYERH
