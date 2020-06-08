/*
 * Othello game
 * ver 0.01
 */

#include "othello.h"

Game::Game(int diskColor) : 
            diskRadius(29), 
            tileSize(64),   
            boardTiles(8), 
            tileSpacing(2), 
            boardSize(boardTiles * tileSize),
            buttonColor(ImColor(0.0f, 0.5f, 0.0f)), 
            buttonHoverColor(ImColor(0.0f, 0.7f, 0.0f)),
            buttonActiveColor(ImColor(0.0f, 0.85f, 0.0f)),
            boardColor(ImColor(0.0f, 0.25f, 0.0f)),
            diskColorWhite(ImColor(1.0f, 1.0f, 1.0f)),
            diskColorBlack(ImColor(0.15f, 0.15f, 0.15f)),
            CurrentDiskColor(diskColor),
            GameBoard{{0}}
            {}
Game::~Game() {}

// game initialization
void Game::OthelloInit()
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
    style.WindowPadding = ImVec2(5, 5);// padding within the window
}

// game logic goes here, deltaTime is the time in seconds since last call to this function
void Game::OthelloFrame(float deltaTime)
{
    
}

// called when a tile was clicked
void Game::OnTileClicked(int y, int x)
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

bool Game::OthelloButton(int x, int y)
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
void Game::OthelloRender(int width, int height)
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

int Game::testPosition(const int x, const int y)
{
    int reply = 0;
    //Is there disc around current pos
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

void Game::InitSdl()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Othello", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    if (!window)
        throw std::runtime_error("Failed to create SDL window");

    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);

    SDL_GL_SetSwapInterval(1);
}

void Game::InitImgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, &gl_context);
    ImGui_ImplOpenGL2_Init();
}
void Game::update()
{
    uint64_t ticksLast = SDL_GetPerformanceCounter();

    // prepare new frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        // game specific logic and rendering goes here
        int width, height;
        SDL_GetWindowSize(window, &width, &height);

        uint64_t ticksNow = SDL_GetPerformanceCounter();
        float deltaTime = (ticksNow - ticksLast)*1000 / (float)SDL_GetPerformanceFrequency();
        ticksLast = ticksNow;

        OthelloFrame(deltaTime);
        OthelloRender(width, height);
        
        // let imgui handle rest of the rendering process
        ImGui::Render();

        glViewport(0, 0, width, height);
        //glClearColor(1, 0, 0, 1);
        //glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
}

