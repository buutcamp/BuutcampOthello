/*
 * Othello game
 * by BuutcampOthello
 * ver 0.01
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

#define USE_HINT_MASK           0       //1 = Used in game, 0 = Not in use
#define USE_DEBUG               1       //1 = In use, 0 = Not used

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
        void OnTileClicked(int y, int x);
        void update();

        bool OthelloButton(int x, int y);
        int  TestDirection(const int x, const int y, const int dir_x, const int dir_y);
        int  TestPosition(const int x, const int y);
        void FlipDisks(const int x, const int y);
        #if (USE_HINT_MASK == 1)
        void UpdateHintMask(void);
        #endif

        const int diskRadius;
        const int tileSize;
        const int boardTiles;
        const int tileSpacing;
        const int boardSize;

        const ImColor buttonColor;
        const ImColor buttonHoverColor;
        const ImColor buttonActiveColor;
        const ImColor boardColor;
        const ImColor diskColorWhite;
        const ImColor diskColorBlack;

    private:
        int CurrentDiskColor;
        int GameBoard[8][8]; //int GameBoard[boardTiles][boardTiles];
        #if (USE_HINT_MASK == 1)
        int HintMask[8][8];
        #endif
        SDL_Window* window;
        SDL_GLContext gl_context;
};

#if (USE_DEBUG == 1)
#include <iostream>
void dbMessage(const std::string &s, bool crlf);
#endif
//#if (USE_DEBUG == 1)
//dbMessage(const std::string &s, bool crlf);
//#endif

#endif      //end othello.h
