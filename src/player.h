/*
 * Player
 * player.h
 * ver 1.0
 */

#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "definet.h"
#include <vector>
#include <tuple>
#include <iostream>

enum Tiles {EMPTY, WHITE, BLACK, HINT};
class Player {
    friend class Game;
    public:
    Player(/*parameters*/);
    ~Player();
    
    void OnTileClicked(int x, int y);
    int  TestDirection(const int x, const int y, const int dir_x, const int dir_y);
    int  TestPosition(const int x, const int y);
    void FlipDisks(const int x, const int y);
    int updateBlackScore();
    int updateWhiteScore();
    void update();
    void OnResetButtonClicked();
    void OnChangeBoardSize();
    void onGameOver();
    int setCurrentDiskColor();
    void UpdateHintMask(void);
    void updatePlayerTurn();
    bool reset_Game();
    bool game_Over();
    void updateShowHint(const bool hintFlag);
    void updateBoardTiles(const int tiles);
    void updateBoardSize(std::vector<std::vector<int>> boardSize);
    void updateGameHint(std::vector<std::vector<int>> hintButton);
    bool boardSizeChanged();
    void initializeGameBoard();
    

    private:
    int boardTiles;
    int current_disc_color;
    std::vector<std::vector<int>> GameBoard;
    std::vector<std::vector<int>> HintMask;
    int hintCount;
    int passed_gameTurn_counter; 
    int scoreWhite;
    int scoreBlack;
    int playerTurn;

    bool board_size_changed;
    bool reset_game;
    bool pass_turn;
    bool show_hint;
    bool game_over;

};

#endif      //end _PLAYER_H_

