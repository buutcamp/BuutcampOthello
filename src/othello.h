/*
 * Othello game
 * by BuutcampOthello
 * ver 0.01
 */

#ifndef _OTHELLO_H_
#define _OTHELLO_H_

#include <imgui.h>
#include <string>
#include <vector>
#include <list>
#include <tuple>
#include <algorithm>
#include <unordered_map>
/*
 * 
 */

enum pcs {Empty, White, Black};

const int diskRadius    = 29;
const int tileSize      = 64;
const int boardTiles    = 8;
const int tileSpacing   = 2;
const int boardSize     = (boardTiles * tileSize);

const ImColor buttonColor       = ImColor(0.0f, 0.5f, 0.0f);
const ImColor buttonHoverColor  = ImColor(0.0f, 0.7f, 0.0f);
const ImColor buttonActiveColor = ImColor(0.0f, 0.85f, 0.0f);
const ImColor boardColor        = ImColor(0.0f, 0.25f, 0.0f);
const ImColor diskColorWhite    = ImColor(1.0f, 1.0f, 1.0f);
const ImColor diskColorBlack    = ImColor(0.15f, 0.15f, 0.15f);

void OthelloInit();
void OthelloFrame(float deltaTime);
void OthelloRender(int width, int height);

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
