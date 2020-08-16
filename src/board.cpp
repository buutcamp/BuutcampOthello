#include "othello.h"
#include "board.h"

/*
 * class othelloBoard
 */
OthelloBoard::OthelloBoard()
{
    this->positions.resize(64, 0);
}

OthelloBoard::~OthelloBoard()
{
    //
}

void OthelloBoard::displayBoard(int color)
{
    //if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
    //    window = SDL_CreateWindow("Othello", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    //    if (!window)
    //        throw std::runtime_error("Failed to create SDL window");
    //    gl_context = SDL_GL_CreateContext(window);
    //    SDL_GL_MakeCurrent(window, gl_context);
    //    SDL_GL_SetSwapInterval(1);
    //    isRunning = true;
    //} else {
    //    isRunning = false;
    //    throw std::runtime_error("Failed to intialize SDL");
    //}
}

void OthelloBoard::displayLegalMoves()
{
    int colNum = 0, rowNum = 0;
    //int moveNum = 1;
    std::list<int> flippedDiscs;

    std::cout << "Legal moves:" << std::endl;

    for (auto keyval : this->moves) {
        index2coord(keyval.first, colNum, rowNum);
        //std::cout << "\t" << moveNum++ << "\t" << colCoord[colNum] << rowCoord[rowNum];

        flippedDiscs = keyval.second;
        std::cout << " will flip: ";

        for (int disc : flippedDiscs) {
            index2coord(disc, colNum, rowNum);
            //std::cout << colCoord[colNum] << rowCoord[rowNum] << " ";
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

void OthelloBoard::findLegalMoves(int color, std::unordered_map<int, std::list<int>> *pMoves)
{
    // Clear legal moves from previous ply
    this->moves.clear();

    for (int i = 0; i < 64; i++) {
        if (this->positions[i] == color) {
            // Check rows
            findLegalMoveInDirection(i, color, -1, pMoves);
            findLegalMoveInDirection(i, color, 1, pMoves);

            // Check columns
            findLegalMoveInDirection(i, color, -8, pMoves);
            findLegalMoveInDirection(i, color, 8, pMoves);

            // Check diagonals
            findLegalMoveInDirection(i, color, -9, pMoves);
            findLegalMoveInDirection(i, color, 9, pMoves);
            findLegalMoveInDirection(i, color, -7, pMoves);
            findLegalMoveInDirection(i, color, 7, pMoves);
        }
    }
}

void OthelloBoard::findLegalMoveInDirection(int &disc, int &color, int direction, std::unordered_map<int, std::list<int>> *pMoves)
{
    std::pair<int, std::list<int>> legalMove;
    std::list<int> flippedDiscs;
    int currentSquare = 0;
    int row1 = 0, col1 = 0, row2 = 0, col2 = 0;

    for (int i = disc + direction; i < 64 && i > -1; i += direction) {
        // Guard against wrapping around the board
        index2coord(i-direction, col1, row1);
        index2coord(i, col2, row2);
        if (abs(col1 - col2) > 1 || abs(row1 - row2) > 1) {
            break;
        }

        // Keep moving in given direction, remembering any discs of the
        // opposite color. Break if we see any discs of our color.
        currentSquare = this->positions[i];
        if (currentSquare == color ||
                (currentSquare == 0 && flippedDiscs.empty())) {
            break;
        }
        else if (currentSquare == -color) {
            flippedDiscs.push_front(i);
            continue;
        }
        // If we see an empty square, it is a legal move: insert it into the
        // moves hash table.
        // NB: we must check to see if the move is already in the map.
        // Second condition is to resolve edge case of
        // disc immediately adjacent to original disc.
        else if (currentSquare == 0 && !flippedDiscs.empty()) {
            std::unordered_map<int, std::list<int>>::iterator it = pMoves->find(i);

            if (it != pMoves->end()) {
                it->second.merge(flippedDiscs);
            }
            else {
                legalMove.first = i;
                legalMove.second = flippedDiscs;
                pMoves->insert(legalMove);
            }

            break;
        }
    }
}

void OthelloBoard::updateBoard(int color, std::pair<int, std::list<int>> move)
{
    int square = move.first;
    std::list<int> flippedDiscs = move.second;

    this->positions[square] = color;
    for (auto disc : flippedDiscs) {
        this->positions[disc] = color;
    }
}

bool OthelloBoard::terminalState()
{
    if (this->passes[0] && this->passes[1]) {
        return true;
    }
    return false;
}

void OthelloBoard::index2coord(int index, int &colNum, int &rowNum)
{
    colNum = index % 8;
    rowNum = index / 8;
}
