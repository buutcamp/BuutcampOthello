/*
 * Othello game
 * by BuutcampOthello
 * ver 0.25
 */
#include <iostream>

#ifndef _OTHELLO_H_
#define _OTHELLO_H_

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_opengl.h>

#include <imgui.h>
#include <examples/imgui_impl_sdl.h>
#include <examples/imgui_impl_opengl2.h>
#include <string>
#include <vector>
#include "definet.h"
#include "client.h"     //class Client
#include "server.h"     //class Server
#include "player.h"     //class Player
#include "ai.h"
#include "board.h"


/*
 * class Game
 */
class Game {
    Server server;
    Client client;
    Player Player1;
    Player Player2;
    OthelloHeuristic AI;
    OthelloBoard board;

    public:
        Game(int disk_color, int game_style);
        ~Game();

        void OthelloInit();
        void InitSdl();
        void InitImgui();
        void OthelloFrame(float deltaTime);
        void OthelloRender(int width, int height);
        void OnTileClicked(int x, int y);
        void update();
        void handleEvents();
        void clean();
        void updateScore();
        void updatePlayerTurn();
        bool resetGame();
        bool gameOver();
        bool changeBoardsize();
        bool OthelloButton(int x, int y);
        bool gameRunning();
        Player ActivePlayer;

        int  TestDirection(const int x, const int y, const int dir_x, const int dir_y);
        int  TestPosition(const int x, const int y);
        void FlipDisks(const int x, const int y);
        void UpdateHintMask(void);
        bool LocalLock;
        std::vector<std::vector<int>> GetGameBoard() {return GameBoard;}
        std::vector<std::vector<int>> GetHintMask() {return HintMask;}

    private:
        const int diskRadius;
        const int tileSize;
        const int GameStyle;

        int boardTiles;
        std::vector<std::vector<int>> GameBoard;
        std::vector<std::vector<int>> HintMask;
        const int tileSpacing;
        const int boardSize;

        const ImColor buttonColor;
        const ImColor buttonHoverColor;
        const ImColor buttonActiveColor;
        const ImColor boardColor;
        const ImColor diskColorWhite;
        const ImColor diskColorBlack;
        const ImColor diskColorHint;

        int scoreWhite;
        int scoreBlack;
        int playerTurn;
        int passed_gameTurn_counter;
        int hintCount;
        int CurrentDiskColor;
        bool reset_game;
        bool boardSizeChanged;
        bool isRunning;
        bool showHint;
        bool game_over;
        bool pass_turn;

        void HandleRemoteMessages();
        int ParseMoveString(const str text, int& x, int& y);

        SDL_Window* window;
        SDL_GLContext gl_context;
};


#if (USE_DEBUG == 1)
void dbMessage(const std::string &s, bool crlf);
#endif // end if USE_DEBUG

#endif      //end othello.h
