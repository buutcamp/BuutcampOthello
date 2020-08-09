#ifndef HEURISTIC_HPP
#define HEURISTIC_HPP

#include <numeric>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <algorithm>
//#include "othello.h"
#include "definet.h"
#include "board.h"

class ai {
    //Game* game;         //for main class functions call example: game->OnTileClicked(int x, int y);
    OthelloBoard* board;

  public:
        ai();
        ~ai();
        int evaluate(OthelloBoard &board, int color);
    private:
        std::unordered_set<int> stableDiscs;
        std::unordered_map<int, std::list<int>> pMoves;

        int utility(OthelloBoard &board, int &color);
        int discDifference(OthelloBoard &board, int &color);
        int mobility(OthelloBoard &board, int &color);
        int potentialMobility(OthelloBoard &board, int color);
        int playerPotentialMobility(OthelloBoard &board, int color);
        int stability(OthelloBoard &board, int color);
        void stableDiscsFromCorner(OthelloBoard &board, int corner, int color);
        int parity(OthelloBoard &board);
        int squareWeights(OthelloBoard &board, int &color);
        int corners(OthelloBoard &board, int &color);
};

#endif // AI_HPP
