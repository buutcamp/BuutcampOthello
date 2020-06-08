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

/*
 * 
 */

enum pcs {Empty, White, Black};

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
        int testPosition(const int x, const int y);
       
        
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
        SDL_Window* window;
        SDL_GLContext gl_context;
};

#endif      //end othello.h
