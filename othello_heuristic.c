#include <numeric>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <list>
#include <tuple>
#include <algorithm>
#include <othello.h>

//1 shows black disk and -1 shows white disk

std::unordered_set<int> stableDiscs;
std::unordered_map<int, std::list<int>> pMoves;
std::vector<int> position[64];

// Assigns a weight to every square on the board
int squareWeights(othello &board, int &diskcolor){
    std::vector<int> weights = {
         200, -100, 100,  50,  50, 100, -100,  200,
        -100, -200, -50, -50, -50, -50, -200, -100,
         100,  -50, 100,   0,   0, 100,  -50,  100,
          50,  -50,   0,   0,   0,   0,  -50,   50,
          50,  -50,   0,   0,   0,   0,  -50,   50,
         100,  -50, 100,   0,   0, 100,  -50,  100,
        -100, -200, -50, -50, -50, -50, -200, -100,
         200, -100, 100,  50,  50, 100, -100,  200,
    };

    if (position[0] != 0) {
        weights[1]  = 0;
        weights[2]  = 0;
        weights[3]  = 0;
        weights[8]  = 0;
        weights[9]  = 0;
        weights[10] = 0;
        weights[11] = 0;
        weights[16] = 0;
        weights[17] = 0;
        weights[18] = 0;
        weights[24] = 0;
        weights[25] = 0;
    }

    if (position[7] != 0) {
        weights[4]  = 0;
        weights[5]  = 0;
        weights[6]  = 0;
        weights[12] = 0;
        weights[13] = 0;
        weights[14] = 0;
        weights[15] = 0;
        weights[21] = 0;
        weights[22] = 0;
        weights[23] = 0;
        weights[30] = 0;
        weights[31] = 0;
    }

    if (position[56] != 0) {
        weights[32] = 0;
        weights[33] = 0;
        weights[40] = 0;
        weights[41] = 0;
        weights[42] = 0;
        weights[48] = 0;
        weights[49] = 0;
        weights[50] = 0;
        weights[51] = 0;
        weights[57] = 0;
        weights[58] = 0;
        weights[59] = 0;
    }

    if (position[63] != 0) {
        weights[38] = 0;
        weights[39] = 0;
        weights[45] = 0;
        weights[46] = 0;
        weights[47] = 0;
        weights[52] = 0;
        weights[53] = 0;
        weights[54] = 0;
        weights[55] = 0;
        weights[60] = 0;
        weights[61] = 0;
        weights[62] = 0;
    }

    if (diskcolor == 1) {
        return std::inner_product(position[0], position[63], weights.begin(), 0);
    }
    else {
        return -1*std::inner_product(position[0], position[63], weights.begin(), 0);
    }
}

int corners(int &diskcolor){
    std::vector<int> corners = {0, 7, 56, 63};
    int blackCorners = 0;
    int whiteCorners = 0;

    for (int corner : corners) {
        if (position[corner] == 1) {
            blackCorners++;
        }
        else if (position[corner] == -1) {
            whiteCorners++;
        }
    }

    if (diskcolor == 1) {
        return 100 * (blackCorners - whiteCorners)
            / (blackCorners + whiteCorners + 1);
    }
    else {
        return 100 * (whiteCorners - blackCorners)
            / (blackCorners + whiteCorners + 1);
    }
}

int utility(int &diskcolor) {
    int util = std::accumulate(position[0], position[63], 0);

    if (diskcolor == 1) {
        return util;
    }
    else {
        return -1*util;
    }
}
bool EmptyCell(i, j){
 if()
    return true;
 else
    return false;
}
bool IsLegalMove(int &diskcolor,int &pMoves){

  //return i>= 0 && i< 8 && j >= 0 && j < 8 && EmptyCell(i, j);
}
// Number of possible moves
int mobility(int &diskcolor){
    IsLegalMove(1, &pMoves);
    int blackMoves = pMoves.size();
    pMoves.clear();

    IsLegalMove(-1, &pMoves);
    int whiteMoves = pMoves.size();
    pMoves.clear();

    if (diskcolor == 1) {
        return 100 * (blackMoves - whiteMoves) / (blackMoves + whiteMoves + 1);
    }
    else {
        return 100 * (whiteMoves - blackMoves) / (blackMoves + whiteMoves + 1);
    }
}

int potentialMobility(int diskcolor){
    int myPotentialMobility = playerPotentialMobility(diskcolor);
    int opponentPotentialMobility = playerPotentialMobility(-diskcolor);

    return 100 * (myPotentialMobility - opponentPotentialMobility)
        / (myPotentialMobility + opponentPotentialMobility + 1);
}

int playerPotentialMobility(int diskcolor){
    std::vector<int> boardInterior = {18, 19, 20, 21,
                                      26, 27, 28, 29,
                                      34, 35, 36, 37,
                                      42, 43, 44, 45};

    int here = 0, up = 0, down = 0, left = 0, right = 0,
        upperLeft = 0, upperRight = 0, lowerLeft = 0, lowerRight = 0;
    int potentialMobility = 0;

    for (int square : boardInterior) {
        here = position[square];
        up = position[square-8];
        down = position[square+8];
        left = position[square-1];
        right = position[square+1];
        upperLeft = position[square-9];
        upperRight = position[square-7];
        lowerLeft = position[square+7];
        lowerRight = position[square+9];

        if (here == -diskcolor && up == 0)
            potentialMobility++;
        if (here == -diskcolor && down == 0)
            potentialMobility++;
        if (here == -diskcolor && right == 0)
            potentialMobility++;
        if (here == -diskcolor && right == 0)
            potentialMobility++;
        if (here == -diskcolor && upperLeft == 0)
            potentialMobility++;
        if (here == -diskcolor && upperRight == 0)
            potentialMobility++;
        if (here == -diskcolor && lowerLeft == 0)
            potentialMobility++;
        if (here == -diskcolor && lowerRight == 0)
            potentialMobility++;
    }

    std::vector<int> topRow = {10, 11, 12, 13};
    for (int square : topRow) {
        here = position[square];
        left = position[square-1];
        right = position[square+1];
        if (here == -diskcolor && left == 0)
            potentialMobility++;
        if (here == -diskcolor && right == 0)
            potentialMobility++;
    }

    std::vector<int> bottomRow = {50, 51, 52, 53};
    for (int square : bottomRow) {
        here = position[square];
        left = position[square-1];
        right = position[square+1];
        if (here == -diskcolor && left == 0)
            potentialMobility++;
        if (here == -diskcolor && right == 0)
            potentialMobility++;
    }

    std::vector<int> leftColumn = {17, 25, 33, 41};
    for (int square : leftColumn) {
        here = position[square];
        up = position[square-8];
        down = position[square+8];
        if (here == -diskcolor && up == 0)
            potentialMobility++;
        if (here == -diskcolor && down == 0)
            potentialMobility++;
    }

    std::vector<int> rightColumn = {22, 30, 38, 46};
    for (int square : leftColumn) {
        here = position[square];
        up = position[square-8];
        down = position[square+8];
        if (here == -diskcolor && up == 0)
            potentialMobility++;
        if (here == -diskcolor && down == 0)
            potentialMobility++;
    }

    return potentialMobility;
}///////////////////

int evaluate(othello &board, int diskcolor) {
    if (board.terminalState()) {
        return 100000*utility(board, diskcolor);
    }

    if (board.discsOnBoard <= 20) {
        // Opening game
        return 5*mobility(diskcolor)
            + 5*potentialMobility(diskcolor)
            + 20*squareWeights(board, diskcolor)
            + 10000*corners(board, diskcolor)
            + 10000*stability(diskcolor);
    }
    else if (board.discsOnBoard <= 58) {
        // Midgame
        return 10*diskDifference(board, diskcolor)
            + 2*mobility(diskcolor)
            + 2*potentialMobility(diskcolor)
            + 10*squareWeights(board, diskcolor)
            + 100*parity(board)
            + 10000*corners(board, diskcolor)
            + 10000*stability(diskcolor);
    }
    else {
        // Endgame
        return 500*diskDifference(board, diskcolor)
            + 500*parity(board)
            + 10000*corners(board, diskcolor)
            + 10000*stability(diskcolor);
    }
}


// Relative disc difference between the two players
int diskDifference(othello &board, int &diskcolor){
    int blackCount = std::count(board.positions.begin(),
            board.positions.end(), 1);
    int whiteCount = std::count(board.positions.begin(),
            board.positions.end(), -1);

    if (diskcolor == 1) {
        return 100 * (blackCount - whiteCount) / (blackCount + whiteCount);
    }
    else {
        return 100 * (whiteCount - blackCount) / (blackCount + whiteCount);
    }
}

// Computes a lower bound on the number of stable discs
int stability(int diskcolor){
    stableDiscs.clear();

    stableDisksFromCorner(0, diskcolor);
    stableDisksFromCorner(7, diskcolor);
    stableDisksFromCorner(56, diskcolor);
    stableDisksFromCorner(63, diskcolor);

    int myStables = stableDiscs.size();

    stableDisksFromCorner(0, -diskcolor);
    stableDisksFromCorner(7, -diskcolor);
    stableDisksFromCorner(56, -diskcolor);
    stableDisksFromCorner(63, -diskcolor);

    int opponentStables = stableDiscs.size();

    return myStables - opponentStables;
}

// Finds the number of stable discs given a corner
void stableDisksFromCorner(int corner, int diskcolor){
    bool down, right;
    if (corner == 0) {
        down = true;
        right = true;
    }
    else if (corner == 7) {
        down = true;
        right = false;
    }
    else if (corner == 56) {
        down = false;
        right = true;
    }
    else {
        down = false;
        right = false;
    }

    int horizIncr = 1, horizStop = 7, vertIncr = 8, vertStop = 56;
    if (!right) {
        horizIncr *= -1;
        horizStop *= -1;
    }
    if (!down) {
        vertIncr *= -1;
        vertStop *= -1;
    }

    // Iterate horizontally
    for (int i = corner; i != corner + horizIncr + horizStop; i += horizIncr) {
        // If there is a disc of our diskcolor on this square
        if (position[i] == diskcolor) {
            // Iterate vertically
            for (int j = i; j != i + vertStop; j+= vertIncr) {
                // If there is a disc of our diskcolor on this square,
                // and it is not in the set of stable discs
                if (position[j] == diskcolor
                        && stableDiscs.find(j) == stableDiscs.end()) {
                    // Insert it to the set
                    stableDiscs.insert(j);
                }
                // If we come across a disc of the opposite diskcolor, break
                else {
                    break;
                }
            }
        }
        // Otherwise, break
        else {
            break;
        }
    }
}

int parity(othello &board){
    int squaresRemaining = 64 - board.discsOnBoard;

    if (squaresRemaining % 2 == 0) {
        return -1;
    }
    else {
        return 1;
    }
}



/* Heuristic is from the perspective that the calling player is the maximizing player,
   regardless of the player's color */
