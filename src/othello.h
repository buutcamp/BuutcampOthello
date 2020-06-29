/*
 * Othello game
 * by BuutcampOthello
 * ver 0.20
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


#define SERVER_MODULE_IN_USE        //Comment this line if not used
#define CLIENT_MODULE_IN_USE        //Comment this line if not used

#ifdef SERVER_MODULE_IN_USE
#include "server.h"
#endif

#ifdef CLIENT_MODULE_IN_USE
#include "client.h"
#endif

/* Sets constants */
#define WIDTH                   1000     // Window width
#define HEIGHT                  800     // Window height
#define BOARD_TILES             8       // Number of tiles in a row/column
//#define USE_HINT_MASK           1       //1 = Used in game, 0 = Not in use
#define USE_DEBUG               0       //1 = In use, 0 = Not used

/*
 * 
 */

enum pcs {Empty, White, Black, Hint};

class Game {
    public: 
        Game(int disk_color);
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

        int  TestDirection(const int x, const int y, const int dir_x, const int dir_y);
        int  TestPosition(const int x, const int y);
        void FlipDisks(const int x, const int y);
       // #if (USE_HINT_MASK == 1)
        void UpdateHintMask(void);
       // #endif

    private:
        const int diskRadius;
        const int tileSize;
        
        int boardTiles;
        std::vector<std::vector<int>> GameBoard;
        const int tileSpacing;
        const int boardSize;

        const ImColor buttonColor;
        const ImColor buttonHoverColor;
        const ImColor buttonActiveColor;
        const ImColor boardColor;
        const ImColor diskColorWhite;
        const ImColor diskColorBlack;
       // #if (USE_HINT_MASK == 1)
        const ImColor diskColorHint;
        std::vector<std::vector<int>> HintMask;
       // #endif
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

        SDL_Window* window;
        SDL_GLContext gl_context;
};

#if (USE_DEBUG == 1)
#include <iostream>
void dbMessage(const std::string &s, bool crlf);
#endif // end if USE_DEBUG

#endif      //end othello.h

