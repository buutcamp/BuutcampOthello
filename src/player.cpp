/*
 * class Player
 * ver 0.01
 * This is just for compiler to prevent errors
 */


 #include "othello.h"
 
Player::Player(/* parameters */)
{
    PlayerColor = Empty;    //Must change in GUI
    PlayerType = -1;        //Must change in GUI
}

Player::~Player()
{
    //
}

void Player::OnTileClicked(int x, int y)
{
    //
}

void Player::updateScore()
{
    //
}

void Player::update()
{
    //
}

void Player::updatePlayerTurn()
{
    //
}

bool Player::resetGame()
{
    PlayerColor = Empty;
    PlayerType = -1;
    return true;
}
