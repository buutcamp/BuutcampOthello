/*
 * Othello game
 * by BuutcampOthello
 * ver 1.0
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
#include <list>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include "definet.h"
#include "client.h"     //class Client
#include "server.h"     //class Server
#include "player.h"     //class Player
#include "ai.h"
#include "board.h"

/*
 * class Game
 */

//class Player;

class Game {
    friend class Server;
    friend class Client;
    //Player wPlayer;
    //Player bPlayer;
    ai AI;
    OthelloBoard board;
    friend class Player;

    public:
        Game(int disk_color, int game_style);
        ~Game();

        void OthelloInit(void);
        void InitSdl();
        void InitImgui();
        void OthelloFrame(float deltaTime);
        void OthelloRender(int width, int height /*, Game game*/);
        void update(/*Game game*/);
        void handleEvents();
        void clean();
        bool resetGame();
        bool gameOver();
        bool changeBoardsize();
        bool OthelloButton(int x, int y);
        bool gameRunning();
        bool LocalLock;

    private:
        int GameStyle;
        const int diskRadius;
        const int tileSize;
        
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
        int CurrentDiskColor;
        bool isRunning;
        bool showHint;
        bool game_over;
        void HandleRemoteMessages();
        int ParseMoveString(const str text, int& x, int& y);
        float SizeMod;

        SDL_Window* window;
        SDL_GLContext gl_context;
        //Player bPlayer;
        //Player wPlayer;
};


#if (USE_DEBUG == 1)
void dbMessage(const std::string &s, bool crlf);
#endif // end if USE_DEBUG
#endif      //end othello.h
