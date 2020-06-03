#include <imgui.h>
#include <string>

#include "othello.h"

const int tileSize = 64;
const int boardTiles = 8;
const int boardSize = boardTiles*tileSize;

// game initialization
void OthelloInit()
{

}

// game logic goes here, deltaTime is the time in seconds since last call to this function
void OthelloFrame(float deltaTime)
{
    
}

// called when a tile was clicked
void OnTileClicked(int x, int y)
{

}

bool OthelloButton(int x, int y)
{
    // label for the button
    std::string label = "button";

    // this id must be unique for each button
    std::string id = "othellobutton" + std::to_string(x) + "-" + std::to_string(y);
    
    if (ImGui::Button((label + "##" + id).c_str(), ImVec2(tileSize, tileSize)))
        return true;
    return false;
}

// this function handles all rendering of the GUI
void OthelloRender(int width, int height)
{
    // the main imgui window uses all the space available
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)width, (float)height));

    // creates a window with no visible frames
    ImGui::Begin("Othello", nullptr, ImGuiWindowFlags_NoDecoration);
    {
        // draw the board
        for (int i=0; i<boardTiles; i++)
        {
            for (int j=0; j<boardTiles; j++)
            {
                ImGui::SameLine(0.0f, 0.0f);
                if (OthelloButton(j, i))
                    OnTileClicked(j, i);
            }
            ImGui::NewLine();
        }
    }
    ImGui::End();
}