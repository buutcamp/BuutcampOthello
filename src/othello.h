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

class Player;

class Game {
    Server server;
    Client client;
    //Player Player1;
    //Player Player2;
    OthelloHeuristic AI;
    OthelloBoard board;

    public:
        Game(int disk_color, int game_style);
        ~Game();

        void OthelloInit(void);
        void InitSdl();
        void InitImgui();
        void OthelloFrame(float deltaTime);
        void OthelloRender(int width, int height /*, Game game*/);
        //void OnTileClicked(int x, int y);
        void update(/*Game game*/);
        void handleEvents();
        void clean();
        //void updateScore();
        //void updatePlayerTurn();
        bool resetGame();
        bool gameOver();
        bool changeBoardsize();
        bool OthelloButton(int x, int y);
        bool gameRunning();
        Player ActivePlayer;
        //int  TestDirection(const int x, const int y, const int dir_x, const int dir_y);
        //int  TestPosition(const int x, const int y);
       // void FlipDisks(const int x, const int y);
       // #if (USE_HINT_MASK == 1)
       // void UpdateHintMask(void);
       // #endif
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
        std::vector<std::vector<int>> HintMask;
        //int scoreWhite;
        //int scoreBlack;
        //int playerTurn;
       // int passed_gameTurn_counter; 
       // int hintCount;
        int CurrentDiskColor;
        //bool reset_game;
        //bool boardSizeChanged;
        bool isRunning;
        bool showHint;
        bool game_over;
      //  bool pass_turn;

        void HandleRemoteMessages();
        int ParseMoveString(const str text, int& x, int& y);

        SDL_Window* window;
        SDL_GLContext gl_context;
        Player bPlayer;
        Player wPlayer;
};


#if (USE_DEBUG == 1)
void dbMessage(const std::string &s, bool crlf);
#endif // end if USE_DEBUG

class othelloBoard {
    public:

        othelloBoard();
        std::vector<int> positions;//Positions show all pieces on the board
        int discsOnBoard = 4;
        float timeLimit = 0.0;
        bool passes[2] = {false, false};//passes[0] and passes[1] are true if the most recent/second most recent ply was a pass, resp.
        std::unordered_map<int, std::list<int>> moves;//list of all pieces to be flipped are values.

        void displayBoard(int color);
        void displayLegalMoves();      
        void findLegalMoves(int color, std::unordered_map<int, std::list<int>> *pMoves);// Finds all legal moves
        void findLegalMoveInDirection(int &disc, int &color, int direction, std::unordered_map<int, std::list<int>> *pMoves);//Helper function to find a legal move given a disc, its color and a direction.
        void updateBoard(int color, std::pair<int, std::list<int>> move);// Update board after a move
        bool terminalState(); 
        void index2coord(int index, int &colNum, int &rowNum);// Helper function to convert board square index to coordinate strings
};
#endif      //end othello.h
