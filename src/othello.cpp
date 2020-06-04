#include <imgui.h>
#include <string>

#include "othello.h"

const int diskRadius = 29;
const int tileSize = 64;
const int boardTiles = 8;
const int tileSpacing = 2;
const int boardSize = boardTiles*tileSize;
const ImColor buttonColor = ImColor(0.0f, 0.5f, 0.0f);
const ImColor buttonHoverColor = ImColor(0.0f, 0.7f, 0.0f);
const ImColor buttonActiveColor = ImColor(0.0f, 0.85f, 0.0f);
const ImColor boardColor = ImColor(0.0f, 0.25f, 0.0f);
const ImColor diskColorWhite = ImColor(1.0f, 1.0f, 1.0f);
const ImColor diskColorBlack = ImColor(0.15f, 0.15f, 0.15f);

// game initialization
void OthelloInit()
{
    // adjusts the spacing between buttons
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(tileSpacing, tileSpacing));

    // apply custom style to imgui widgets
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Button] = buttonColor;
    style.Colors[ImGuiCol_ButtonHovered] = buttonHoverColor;
    style.Colors[ImGuiCol_ButtonActive] = buttonActiveColor;
    style.Colors[ImGuiCol_WindowBg] = boardColor;
    style.WindowBorderSize = 0.0f;
    style.WindowRounding = 0.0f;
    style.WindowPadding = ImVec2(5, 5); // padding within the window
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
    std::string label = "";

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
        ImDrawList* drawList = ImGui::GetWindowDrawList(); 
        const ImVec2 boardStartPosition = ImGui::GetCursorScreenPos();

        // draw the buttons
        for (int i=0; i<boardTiles; i++)
        {
            for (int j=0; j<boardTiles; j++)
            {
                ImGui::SameLine(0, (float)tileSpacing);
                if (OthelloButton(j, i))
                    OnTileClicked(j, i);
            }

            ImGui::NewLine();
        }

        // draw the pieces over the buttons
        for (int i=0; i<boardTiles; i++)
        {
            for (int j=0; j<boardTiles; j++)
            {
                const ImVec2 diskOffset = ImVec2(((tileSize+tileSpacing) * j) + (tileSize * 0.5f) + 2.0f, ((tileSize+tileSpacing) * i) + (tileSize * 0.5f));
                const ImVec2 diskPos = ImVec2(boardStartPosition.x + diskOffset.x, boardStartPosition.y + diskOffset.y);

                ImColor diskColor = diskColorWhite;
                drawList->AddCircleFilled(diskPos, diskRadius, diskColor, 30);
            }
        }
    }
    ImGui::End();
}