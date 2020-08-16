/*
 * Player
 * player.h
 * ver 1.0
 */

#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "definet.h"
#include <vector>
#include <iostream>
#include "client.h"
#include "server.h"
#include "othello.h"
#include "board.h"

class Game;

class Player {
    //class Game* game;
    friend class Game;
    friend class OthelloBoard;
    class Client* client;
    class Server* server;

    public:
        Player(/*parameters*/);
        ~Player();

        void OnTileClicked(int x, int y);
        int  TestDirection(const int x, const int y, const int dir_x, const int dir_y);
        int  TestPosition(const int x, const int y);
        void FlipDisks(const int x, const int y);
        int updateBlackScore();
        int updateWhiteScore();
        int getDiscColor();
        void update();
        void OnResetButtonClicked();
        void OnChangeBoardSize();
        void onGameOver();
        int setCurrentDiskColor();
        void UpdateHintMask(void);
        void updatePlayerTurn();
        bool reset_Game();
        bool game_Over();
        bool applyAI(int x_coord,int y_coord);
        int convertXY_to1D(int x, int y);
        void updateShowHint(const bool hintFlag);
        void updateBoardTiles(const int tiles);
        void updateBoardSize(std::vector<std::vector<int>> boardSize);
        void updateGameHint(std::vector<std::vector<int>> hintButton);
        bool boardSizeChanged();
        void initializeGameBoard(int game_style);
        std::vector<std::vector<int>> GetHintMask() {return HintMask;};
        std::vector<std::vector<int>> GetGameBoard() {return GameBoard;};

        int  PlayerType;        //Human_Local, Human_Remote, AI_Local, AI_Remote
        int  PlayerColor;       //EMPTY, WHITE, BLACK

    private:
        int gameStyle;
        int boardTiles;
        int current_disc_color;
        int hintCount;
        int passed_gameTurn_counter;
        int scoreWhite;
        int scoreBlack;
        int playerTurn;
        int discs_counter; // total number of discs placed on the board
        std::vector<std::vector<int>> GameBoard;
        std::vector<std::vector<int>> HintMask;

        bool board_size_changed;
        bool reset_game;
        bool pass_turn;
        bool show_hint;
        bool game_over;
};

#endif      //end _PLAYER_H_
