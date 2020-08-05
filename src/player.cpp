/*
 * class Player
 * ver 1.00
 */

#include "player.h"
#include "othello.h"

Player::Player(/*parametres*/): 
            boardTiles(BOARD_TILES), 
            current_disc_color(WHITE),
            GameBoard{{0}},
            HintMask{{0}},
            hintCount(0),
            passed_gameTurn_counter(0),
            scoreWhite(2),
            scoreBlack(2),
            //playerTurn(WHITE),
            board_size_changed(false),
            //currentTurn(turn),
            reset_game(false),
            pass_turn(false),
            show_hint(true),
            game_over(false),
            PlayerColor(EMPTY),    //Must change in GUI
            PlayerType(-1)        //Must change in GUI
            {
                GameBoard = std::vector<std::vector<int> >(boardTiles, std::vector<int>(boardTiles));
                HintMask = std::vector<std::vector<int> >(boardTiles, std::vector<int>(boardTiles));
            }

Player::~Player(){}

void Player::updatePlayerTurn()
{
    if(pass_turn == true)
    {   if(current_disc_color== WHITE)
        {
            playerTurn = BLACK;
            current_disc_color = BLACK;
        }
        else
        {
            playerTurn = WHITE;
            current_disc_color = WHITE;
        }
        
        pass_turn = false;
    }
    
}
void Player::OnResetButtonClicked()
{
    reset_game = true;
}

void Player::OnChangeBoardSize()
{
   board_size_changed = true;
}

void Player::onGameOver()
{
   game_over = true;
}

int Player::setCurrentDiskColor()
{
    return current_disc_color;
}

void Player::initializeGameBoard()
{
    //Disk place mask
    for(int y = 0; y < boardTiles; ++y) {
        for(int x = 0; x < boardTiles; ++x) {
            GameBoard[x][y] = EMPTY;
            if (show_hint)
                HintMask[x][y] = EMPTY;
        }
    }
    //Start placement
    //This way we can make 8*8, 10*10 and 12*12 board as different levels
    GameBoard[(boardTiles / 2) - 1][(boardTiles / 2) - 1] = WHITE;
    GameBoard[boardTiles / 2][boardTiles / 2] = WHITE;
    GameBoard[(boardTiles / 2) - 1][boardTiles / 2] = BLACK;
    GameBoard[boardTiles / 2][(boardTiles / 2) - 1] = BLACK;
    current_disc_color = BLACK; 
    playerTurn = BLACK;// Player with Black discs begin game
    if (show_hint)
    {
       UpdateHintMask();     
    }
}

void Player::updateBoardTiles(const int tiles)
{
    boardTiles = tiles;
}

void Player::updateBoardSize(std::vector<std::vector<int>> boardSize)
{
    GameBoard = boardSize;
}
void Player::updateGameHint(std::vector<std::vector<int>> hintButton)
{
    HintMask = hintButton;
}
bool Player::reset_Game()
{
    if(reset_game)
    {
        reset_game = false;
        scoreWhite = 2;
        scoreBlack = 2;
        playerTurn = BLACK;
        PlayerColor = EMPTY;
        PlayerType = -1;
        return true;
    }
    return false;
}

void Player::updateShowHint(const bool showHintFlag)
{
    show_hint = showHintFlag;
}

void Player::OnTileClicked(int x, int y)
{
    #if (USE_DEBUG == 1)
    txt = "Button X:" + std::to_string(x) + " Y:" + std::to_string(y);
    Game::dbMessage(txt, true);
    #endif

    int discs_counter = 4; // total number of discs placed on the board
    //Game mask update
    if(GameBoard[x][y] == EMPTY) {
        //Only Empty is allowed
            if(TestPosition(x, y) > 0) {
            if(playerTurn == WHITE)
            {
                scoreWhite += (TestPosition(x, y) + 1);
                scoreBlack -= TestPosition(x, y);
                playerTurn = BLACK;
            }
            else
            {
                scoreBlack += (TestPosition(x, y) + 1);
                scoreWhite -= TestPosition(x, y);
                playerTurn = WHITE;
            }
            GameBoard[x][y] = current_disc_color;
            
            FlipDisks(x, y);
            ++discs_counter;
            if(discs_counter == (boardTiles * boardTiles)) // all discs placed on gameboard
            {
                game_over = true;
                discs_counter = 4;
            } 

            if(current_disc_color == WHITE)
                current_disc_color = BLACK;
            else
                current_disc_color = WHITE; 
            if (show_hint)
               UpdateHintMask();
        }  
        else if(discs_counter == (boardTiles * boardTiles -1) && pass_turn == true) // all except one disc placed and player turn switched
        {
            game_over = true;
            discs_counter = 4;
        }     
    }
   
}

int Player::TestDirection(const int x, const int y, const int dir_x, const int dir_y)
{
    int reply = 0;
    bool end_point = false;
    int pos_x, pos_y;

    //We don't test starting point, but next
    pos_x = x + dir_x;
    pos_y = y + dir_y;

    #if (USE_DEBUG == 1)
    txt = "X:" + std::to_string(x) + " Y:" + std::to_string(y) + " delta X = " + std::to_string(dir_x) + " delta Y = " + std::to_string(dir_y);
    dbMessage(txt, true);
    #endif
    //We must stay inside board
    while ((pos_x >= 0) && (pos_x < boardTiles) && (pos_y >= 0) && (pos_y < boardTiles))
    {
        if(GameBoard[pos_x][pos_y] == EMPTY) {
            //No disks to flip in this direction
            reply = 0;
            break;
        } else if(GameBoard[pos_x][pos_y] == current_disc_color) {
            //Found endpoint in this direction
            end_point = true;
            break;
        } else if(GameBoard[pos_x][pos_y] != current_disc_color) {
            //Possible filippable disk, if there is endpoint
            ++reply;
        }
        pos_x += dir_x;
        pos_y += dir_y;
    }
    #if (USE_DEBUG == 1)
    txt = " Delta sigma = " + std::to_string(reply);
    Game::dbMessage(txt, true);
    #endif

    //Do we have valid endpoint?
    if(end_point == true)
        return reply;
    else
        return 0;
}

int Player::TestPosition(const int x, const int y)
{
    int reply;

    reply = TestDirection(x, y, -1, 0);     //Test to left
    reply += TestDirection(x, y, 1, 0);     //Test to right
    reply += TestDirection(x, y, 0, -1);    //Test to up
    reply += TestDirection(x, y, 0, 1);     //Test to down
    reply += TestDirection(x, y, -1, -1);   //Diagonal test left up
    reply += TestDirection(x, y, 1, 1);     //Diagonal test right down
    reply += TestDirection(x, y, -1, 1);    //Diagonal test left down
    reply += TestDirection(x, y, 1, -1);    //Diagonal test right up

    //Return count of possible flippable disks around point(x,y)
    return reply;
}

void Player::FlipDisks(const int x, const int y)
{
    int end_x, end_y;

    end_x = x - TestDirection(x, y, -1, 0);
    while (end_x != x) {
        GameBoard[end_x++][y] = current_disc_color;
    }

    end_x = x + TestDirection(x, y, 1, 0);
    while (end_x != x) {
        GameBoard[end_x--][y] = current_disc_color;
    }

    end_y = y - TestDirection(x, y, 0, -1);
    while (end_y != y) {
        GameBoard[x][end_y++] = current_disc_color;
    }

    end_y = y + TestDirection(x, y, 0, 1);
    while (end_y != y) {
        GameBoard[x][end_y--] = current_disc_color;
    }

    end_x = x - TestDirection(x, y, -1, -1);
    end_y = y - TestDirection(x, y, -1, -1);
    while (end_x != x) {
        GameBoard[end_x++][end_y++] = current_disc_color;
    }

    end_x = x + TestDirection(x, y, 1, 1);
    end_y = y + TestDirection(x, y, 1, 1);
    while (end_y != y) {
        GameBoard[end_x--][end_y--] = current_disc_color;
    }
    end_x = x + TestDirection(x, y, 1, -1);
    end_y = y - TestDirection(x, y, 1, -1);
    while (end_x != x) {
        GameBoard[end_x--][end_y++] = current_disc_color;
    }

    end_x = x - TestDirection(x, y, -1, 1);
    end_y = y + TestDirection(x, y, -1, 1);
    while (end_y != y) {
        GameBoard[end_x++][end_y--] = current_disc_color;
    }
}

void Player::UpdateHintMask(void)
{
    int x, y;
    std::cout << std::endl;
    for(y = 0; y < boardTiles; ++y) {
        for(x = 0; x < boardTiles; ++x) {
            if(GameBoard[x][y] == WHITE) {
                //HintMask[x][y] = WHITE;
                std::cout << 'W';
            } else if(GameBoard[x][y] == BLACK) {
                //HintMask[x][y] = BLACK;
                std::cout << 'B';
            } else if(TestPosition(x, y) > 0) {
                ++hintCount;
                HintMask[x][y] = HINT;
                std::cout << 'X';
            } else {
                HintMask[x][y] = EMPTY;
                std::cout << 'O';
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    if (hintCount == 0) // if hintCount = 0, switch player's turn
    {
        pass_turn = true;
        ++passed_gameTurn_counter;
        if(passed_gameTurn_counter == 2)
        {
            game_over = true;
        }
    }
    else
    {
        passed_gameTurn_counter = 0;
    }
    
}
int Player::updateBlackScore()
{
    return scoreBlack;
}

int Player::updateWhiteScore()
{
    return scoreWhite;
}

bool Player::game_Over()
{
   if(game_over)
    {
        game_over = false;
        return true;
    }
    return false;
}

bool Player::boardSizeChanged()
{
   if(board_size_changed)
    {
        board_size_changed = false;
        scoreWhite = 2;
        scoreBlack = 2;
        playerTurn = BLACK;
        return true;
    }
    return false;
}