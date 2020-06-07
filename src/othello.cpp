/*
 * Othello game
 * ver 0.01
 */

#include "othello.h"

int GameBoard[boardTiles][boardTiles];
int CurrentDiskColor;

int testPosition(const int x, const int y);

// game initialization
void OthelloInit()
{
    //Disk place mask
    for(int x = 0; x < boardTiles; ++x) {
        for(int y = 0; y < boardTiles; ++y) {
            GameBoard[x][y] = Empty;
        }
    }
    //Start placement
    GameBoard[3][3] = White;
    GameBoard[4][4] = White;
    GameBoard[3][4] = Black;
    GameBoard[4][3] = Black;
    CurrentDiskColor = White;


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
void OnTileClicked(int y, int x)
{
    //Game mask update
    if(GameBoard[x][y] == Empty) {
        //Only Empty is allowed
        if(testPosition(x, y) > 0) {
            GameBoard[x][y] = CurrentDiskColor;
            if(CurrentDiskColor == White)
                CurrentDiskColor = Black;
            else
                CurrentDiskColor = White;
        }
    }
    //Testing
    //if(GameBoard[x][y] == White) {
    //    GameBoard[x][y] = Black;
    //} else if(GameBoard[x][y] == Black) {
    //    GameBoard[x][y] = White;
    //} else {
    //    //Empty
    //}
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
    ImColor diskColor;

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

                //Game mask
                if(GameBoard[i][j] == White) {
                    diskColor = diskColorWhite;
                    drawList->AddCircleFilled(diskPos, diskRadius, diskColor, 30);
                } else if(GameBoard[i][j] == Black) {
                    diskColor = diskColorBlack;
                    drawList->AddCircleFilled(diskPos, diskRadius, diskColor, 30);
                } else {
                    //Empty location
                }
            }
        }
    }
    ImGui::End();
}

int testPosition(const int x, const int y)
{
    int reply = 0;
    //Is there disck around current pos
    if(x > 0) {
        if(GameBoard[x - 1][y] != Empty)
            ++reply;
        if(y > 0) {
            if(GameBoard[x - 1][y - 1] != Empty)
                ++reply;
        }
        if(y < (boardTiles - 1)) {
            if(GameBoard[x - 1][y + 1] != Empty)
                ++reply;
        }
    }
    if(y > 0) {
        if(GameBoard[x][y - 1] != Empty)
            ++reply;
    }
    if(y < (boardTiles - 1)) {
        if(GameBoard[x][y + 1] != Empty)
            ++reply;
    }
    if(x < (boardTiles - 1)) {
        if(GameBoard[x + 1][y] != Empty)
            ++reply;
        if(y > 0) {
            if(GameBoard[x + 1][y - 1] != Empty)
                ++reply;
        }
        if(y < (boardTiles - 1)) {
            if(GameBoard[x + 1][y + 1] != Empty)
                ++reply;
        }
    }
    //Return count of disks around point(x,y)
    return reply;
}
