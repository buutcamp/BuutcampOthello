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

    std::cout << "Legal moves:" << this->moves.size() << std::endl;

    for (auto keyval : this->moves) {
        index2coord(keyval.first, colNum, rowNum);
        //std::cout << "\t" << moveNum++ << "\t" << colCoord[colNum] << rowCoord[rowNum];
        std::cout << keyval.first << " x:" << colNum << " y:" << rowNum << " will flip: ";
        flippedDiscs = keyval.second;

        for (int n : flippedDiscs) {
            std::cout << n << ' ';
        }
        //for (int disc : flippedDiscs) {
        //    index2coord(disc, colNum, rowNum);
        //    //std::cout << colCoord[colNum] << rowCoord[rowNum] << " ";
        //    std::cout << "x:" << colNum << "y: " << rowNum << " ";
        //}
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

int OthelloBoard::calcWeight(int posit)
{
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
    if (positions[0] != 0) {
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

    if (positions[7] != 0) {
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

    if (positions[56] != 0) {
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

    if (positions[63] != 0) {
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
    return weights[posit];
}
