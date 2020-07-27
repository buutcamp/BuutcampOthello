#ifndef _BOARD_H_
#define _BOARD_H_

#include "othello.h"
#include "definet.h"

class Game;
//class OthelloHeuristic;

/*
 * class othelloBoard
 */
class OthelloBoard {
    Game* game;
    //OthelloHeuristic* AI;

    public:
        OthelloBoard();
        ~OthelloBoard();
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

#endif
