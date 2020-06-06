/*
 * Othello game
 * by BuutcampOthello
 * ver 0.01
 */

#ifndef _OTHELLO_H_
#define _OTHELLO_H_

#include <imgui.h>
#include <string>

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

/*
 * 
 */
void OthelloInit();
void OthelloFrame(float deltaTime);
void OthelloRender(int width, int height);

#endif      //end othello.h
