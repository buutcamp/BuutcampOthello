#include "othello.h"

/*
 * class othelloBoard
 */
OthelloBoard::OthelloBoard()
{
    /*
     * Iitialize public variables... and private?
     */
    //std::vector<int> positions;//Positions show all pieces on the board
    //int discsOnBoard = 4;
    //float timeLimit = 0.0;
    //bool passes[2] = {false, false};//passes[0] and passes[1] are true if the most recent/second most recent ply was a pass, resp.
    //std::unordered_map<int, std::list<int>> moves;//list of all pieces to be flipped are values.
}

OthelloBoard::~OthelloBoard()
{
    //
}

void OthelloBoard::displayBoard(int color)
{
    //
}

void OthelloBoard::displayLegalMoves()
{
    //
}

void OthelloBoard::findLegalMoves(int color, std::unordered_map<int, std::list<int>> *pMoves)
{
    // Finds all legal moves
}

void OthelloBoard::findLegalMoveInDirection(int &disc, int &color, int direction, std::unordered_map<int, std::list<int>> *pMoves)
{
    //Helper function to find a legal move given a disc, its color and a direction.
}

void OthelloBoard::updateBoard(int color, std::pair<int, std::list<int>> move)
{
    // Update board after a move
}

bool OthelloBoard::terminalState()
{
    //
    return true;
}

void OthelloBoard::index2coord(int index, int &colNum, int &rowNum)
{
    // Helper function to convert board square index to coordinate strings
}
