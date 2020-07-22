/*
 * Othello game
 * ver 0.20
 */

#include "othello.h"

#if (USE_DEBUG == 1)
std::string txt;
#endif

std::string ver_txt = "ver 0.20";      


Game::Game(int diskColor) : 
            diskRadius(29), 
            tileSize(64),   
            boardTiles(BOARD_TILES), 
            GameBoard{{0}},
            tileSpacing(2), 
            boardSize(boardTiles * tileSize),
            buttonColor(ImColor(0.10f, 0.5f, 0.20f)), 
            buttonHoverColor(ImColor(0.10f, 0.3f, 0.251)),
            buttonActiveColor(ImColor( 0.0f, 0.7f, 0.0f)),
            boardColor(ImColor(.30f, 0.30f, 0.30f)),
            diskColorWhite(ImColor(1.0f, 1.0f, 1.0f)),
            diskColorBlack(ImColor(0.0f, 0.0f, 0.0f)),
            diskColorHint(ImColor(0.80f, 0.50f, 0.0f)),
            HintMask{{0}},
            //scoreWhite(2),
            //scoreBlack(2),
            //playerTurn(Black),
            //passed_gameTurn_counter(0),
            // hintCount(0),
            CurrentDiskColor(diskColor),
            //reset_game(false)
            //boardSizeChanged(false),
            showHint(true)
            //game_over(false)
            // pass_turn(false)
            {
                GameBoard = std::vector<std::vector<int> >(boardTiles, std::vector<int>(boardTiles));
                HintMask = std::vector<std::vector<int> >(boardTiles, std::vector<int>(boardTiles));
            }

Game::~Game() {}

// Window intialization
void Game::InitSdl()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        window = SDL_CreateWindow("Othello", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (!window)
            throw std::runtime_error("Failed to create SDL window");
        gl_context = SDL_GL_CreateContext(window);
        SDL_GL_MakeCurrent(window, gl_context);
        SDL_GL_SetSwapInterval(1);
        isRunning = true;
    }
    else
    {
        isRunning = false;
        throw std::runtime_error("Failed to intialize SDL");
    }
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

// game initialization
void Game::OthelloInit()
{
    //Disk place mask
    for(int y = 0; y < boardTiles; ++y) {
        for(int x = 0; x < boardTiles; ++x) {
            GameBoard[x][y] = Empty;
            if (showHint)
                HintMask[x][y] = Empty;
        }//Start placement
    //This way we can make 8*8, 10*10 and 12*12 board as different levels
    GameBoard[(boardTiles / 2) - 1][(boardTiles / 2) - 1] = White;
    GameBoard[boardTiles / 2][boardTiles / 2] = White;
    GameBoard[(boardTiles / 2) - 1][boardTiles / 2] = Black;
    GameBoard[boardTiles / 2][(boardTiles / 2) - 1] = Black;
    CurrentDiskColor = Black; // Player with Black discs begin game
    }
    

    bPlayer.initializeGameBoard();
    wPlayer.initializeGameBoard();

    /*
    bPlayer.setCurrentDiskColor(CurrentDiskColor);
    wPlayer.setCurrentDiskColor(CurrentDiskColor);

    if (showHint)
    {
       bPlayer.UpdateHintMask();   
       wPlayer.UpdateHintMask();   
    }*/
    if (showHint)
    {
       bPlayer.UpdateHintMask();   
       HintMask = bPlayer.HintMask;
       wPlayer.UpdateHintMask();   
       HintMask= wPlayer.HintMask;
    }               
                   
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
    style.WindowPadding = ImVec2(200, 30);// padding within the window
}

// game logic goes here, deltaTime is the time in seconds since last call to this function
void Game::OthelloFrame(float deltaTime)
{
    
}
/*
// called when a tile was clicked
void Game::OnTileClicked(int x, int y)
{
    #if (USE_DEBUG == 1)
    txt = "Button X:" + std::to_string(x) + " Y:" + std::to_string(y);
    dbMessage(txt, true);
    #endif

    static int discs_counter = 4; // total number of discs placed on the board
    //Game mask update
    if(GameBoard[x][y] == Empty) {
        //Only Empty is allowed
        if(TestPosition(x, y) > 0) {
            if(playerTurn == White)
            {
                scoreWhite += (TestPosition(x, y) + 1);
                scoreBlack -= TestPosition(x, y);
                playerTurn = Black;
            }
            else
            {
                scoreBlack += (TestPosition(x, y) + 1);
                scoreWhite -= TestPosition(x, y);
                playerTurn = White;
            }
            GameBoard[x][y] = CurrentDiskColor;
            FlipDisks(x, y);
            ++discs_counter;
            if(discs_counter == (boardTiles * boardTiles)) // all discs placed on gameboard
            {
                game_over = true;
                discs_counter = 4;
            } 

            if(CurrentDiskColor == White)
                CurrentDiskColor = Black;
            else
                CurrentDiskColor = White; 
            if (showHint)
                UpdateHintMask();
        }  
        else if(discs_counter == (boardTiles * boardTiles -1) && pass_turn == true) // all except one disc placed and player turn switched
        {
           game_over = true;
           discs_counter = 4;
        }     
    }
}*/

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
void Game::OthelloRender(int width, int height/*, Game game*/)
{
    ImColor diskColor;
    // the main imgui window uses all the space available
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)width, (float)height));

    // creates a window with no visible frames
    ImGui::Begin("Othello ", nullptr, ImGuiWindowFlags_NoDecoration);
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList(); 
       
        // draw score discs
        drawList->AddCircleFilled(ImVec2(320, 30), diskRadius * 0.75, diskColorBlack, 30);
        drawList->AddCircleFilled(ImVec2(640, 30), diskRadius * 0.75, diskColorWhite, 30);

        ImGui::Dummy(ImVec2(300,45)); 
        ImGui::Spacing();
        ImGui::SameLine(315, 0);
       // ImGui::TextColored(ImVec4(1,1,0,1), "%02d", bPlayer.updateBlackScore()/*scoreWhite*/);
        ImGui::TextColored(ImVec4(1,1,0,1), "%02d", bPlayer.scoreBlack/*scoreWhite*/);
        ImGui::SameLine(635, 0);
        ImGui::TextColored(ImVec4(1,1,0,1), "%02d", wPlayer.scoreWhite/*scoreBlack*/);
        
        //Draw player turn disc
        ImGui::SameLine(420, 0);
        ImGui::TextColored(ImVec4(1,1,0,1), "TURN");
        if(wPlayer.setCurrentDiskColor() == White)
        {
            drawList->AddCircleFilled(ImVec2(480, 80), diskRadius * 0.75, diskColorWhite, 30);
        }
            else if (bPlayer.setCurrentDiskColor() == Black)
        {
            drawList->AddCircleFilled(ImVec2(480, 80), diskRadius *0.75, diskColorBlack, 30);
        }
        
        ImGui::NewLine();     
        const ImVec2 boardStartPosition = ImGui::GetCursorScreenPos();
        ImGui::Dummy(ImVec2(0,-1));  
        // draw the buttons
        for (int y = 0; y < boardTiles; ++y)
        {
            for (int x = 0; x < boardTiles; ++x)
            {
                ImGui::SameLine(0, (float)tileSpacing);
                if (OthelloButton(x, y))
                {
                    bPlayer.OnTileClicked(x, y);
                    GameBoard = bPlayer.GameBoard;
                    HintMask = bPlayer.HintMask;

                    wPlayer.OnTileClicked(x, y);
                    GameBoard = wPlayer.GameBoard;
                    HintMask = wPlayer.HintMask;
                }
                    
            }
            ImGui::NewLine();
        }

        // draw the pieces over the buttons
        #if (USE_DEBUG == 1)
        txt = "Board";
        dbMessage(txt, true);
        #endif
        for (int y = 0; y < boardTiles; ++y)
        {
            for (int x = 0; x < boardTiles; ++x)
            {
                const ImVec2 diskOffset = ImVec2(((tileSize+tileSpacing) * x) + (tileSize * 0.5f) + 2.0f, ((tileSize+tileSpacing) * y) + (tileSize * 0.5f));
                const ImVec2 diskPos = ImVec2(boardStartPosition.x + diskOffset.x, boardStartPosition.y + diskOffset.y);

                //Game mask
                if(GameBoard[x][y] == White) {
                    diskColor = diskColorWhite;
                    drawList->AddCircleFilled(diskPos, diskRadius, diskColor, 30);
                    #if (USE_DEBUG == 1)
                    txt = "W";
                    dbMessage(txt, false);
                    #endif
                } else if(GameBoard[x][y] == Black) {
                    diskColor = diskColorBlack;
                    drawList->AddCircleFilled(diskPos, diskRadius, diskColor, 30);
                    #if (USE_DEBUG == 1)
                    txt = "B";
                    dbMessage(txt, false);
                    #endif
                } else {
                    //Empty location or hint
                    #if ((USE_DEBUG == 1) && (showHint == false))
                    txt = "_";
                    dbMessage(txt, false);
                    #endif
                    if (showHint){
                        if(HintMask[x][y] == Hint) {
                            //Place hint here
                            diskColor = diskColorHint;
                            drawList->AddCircleFilled(diskPos, (diskRadius / 4), diskColor, 15);
                            #if (USE_DEBUG == 1)
                                txt = "H";
                                dbMessage(txt, false);
                            #endif
                        } else {
                        #if (USE_DEBUG == 1)
                            txt = "_";
                            dbMessage(txt, false);
                        #endif
                        }
                    }
                }
            }
            #if (USE_DEBUG == 1)
            txt = " ";
            dbMessage(txt, true);
            #endif

        }        
        //draw combo dropbox to select game board size
        static int item = 0;
        int current_item = item;
        const char* items[] = {"8x8", "10x10", "12x12"};
       
        ImGui::Dummy(ImVec2(0.0f, 1.0f));
        ImGui::PushItemWidth(100);
        ImGui::SameLine(230, 0);
        ImGui::Combo("BOARD SIZE", &item, items, IM_ARRAYSIZE(items)); 

        // set number of tiles/ board size based on the selected item
        if(current_item != item)
        {
            switch(item)
            {
                case 0: 
                    current_item = 0;
                    boardTiles = BOARD_TILES;
                    break;
                case 1: 
                    current_item = 1;
                    boardTiles = BOARD_TILES + 2;
                    break;
                case 2:
                    current_item = 2;
                    boardTiles = BOARD_TILES + 4;
                    break;
            }
            bPlayer.updateBoardTiles(boardTiles);
            wPlayer.updateBoardTiles(boardTiles);

            //update flag for board size change
            //boardSizeChanged = true;
            bPlayer.OnChangeBoardSize();
            wPlayer.OnChangeBoardSize();

            GameBoard.clear();
            GameBoard.resize(boardTiles,std::vector<int>(boardTiles));
            bPlayer.updateBoardSize(GameBoard);
            wPlayer.updateBoardSize(GameBoard);

            if (showHint)
            {
               HintMask.clear();
               HintMask.resize(boardTiles,std::vector<int>(boardTiles));
               bPlayer.updateGameHint(HintMask);
               wPlayer.updateGameHint(HintMask);
            }
        }
        
        // Draw Reset button  
         ImGui::SameLine(450, 0);
        if(ImGui::Button("RESET"))
        {
            //reset_game = true;
            bPlayer.OnResetButtonClicked();
            wPlayer.OnResetButtonClicked();
        }
        // draw checkbox
        bool current_hint = showHint;
        ImGui::SameLine(570, 0);
        ImGui::Checkbox("SHOW NEXT MOVE HINT", &showHint);
       
        if(current_hint != showHint)
        {
            bPlayer.updateShowHint(showHint);
            wPlayer.updateShowHint(showHint);
        }
        
       //draw combo dropbox to select players option (H-H, H-AI, AI-AI, AI-H)
        ImGui::NewLine();
        static int item2 = 0;
        int current_item2 = item2;
        const char* items2[] = {"Human", "AI"};
       
       // ImGui::Dummy(ImVec2(0.0f, 1.0f));
        ImGui::PushItemWidth(120);
        ImGui::SameLine(230, 0);
        ImGui::Combo("PLAYERS", &item2, items2, IM_ARRAYSIZE(items2)); 
        
        // set number of tiles/ board size based on the selected item
        if(current_item2 != item2)
        {
            switch(item2)
             {
                case 0: 
                    current_item2 = 0;
                    // code definition for human players
                    break;
                case 1: 
                    current_item2= 1;
                     // code definition for Human (local) - AI (remote) players
                    break;
             }
        }
    }
    ImGui::End();
}
/*
int Game::TestDirection(const int x, const int y, const int dir_x, const int dir_y)
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
        if(GameBoard[pos_x][pos_y] == Empty) {
            //No disks to flip in this direction
            reply = 0;
            break;
        } else if(GameBoard[pos_x][pos_y] == CurrentDiskColor) {
            //Found endpoint in this direction
            end_point = true;
            break;
        } else if(GameBoard[pos_x][pos_y] != CurrentDiskColor) {
            //Possible filippable disk, if there is endpoint
            ++reply;
        }
        pos_x += dir_x;
        pos_y += dir_y;
    }
    #if (USE_DEBUG == 1)
    txt = " Delta sigma = " + std::to_string(reply);
    dbMessage(txt, true);
    #endif

    //Do we have valid endpoint?
    if(end_point == true)
        return reply;
    else
        return 0;
}

int Game::TestPosition(const int x, const int y)
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

void Game::FlipDisks(const int x, const int y)
{
    int end_x, end_y;

    end_x = x - TestDirection(x, y, -1, 0);
    while (end_x != x) {
        GameBoard[end_x++][y] = CurrentDiskColor;
    }

    end_x = x + TestDirection(x, y, 1, 0);
    while (end_x != x) {
        GameBoard[end_x--][y] = CurrentDiskColor;
    }

    end_y = y - TestDirection(x, y, 0, -1);
    while (end_y != y) {
        GameBoard[x][end_y++] = CurrentDiskColor;
    }

    end_y = y + TestDirection(x, y, 0, 1);
    while (end_y != y) {
        GameBoard[x][end_y--] = CurrentDiskColor;
    }

    end_x = x - TestDirection(x, y, -1, -1);
    end_y = y - TestDirection(x, y, -1, -1);
    while (end_x != x) {
        GameBoard[end_x++][end_y++] = CurrentDiskColor;
    }

    end_x = x + TestDirection(x, y, 1, 1);
    end_y = y + TestDirection(x, y, 1, 1);
    while (end_y != y) {
        GameBoard[end_x--][end_y--] = CurrentDiskColor;
    }
    end_x = x + TestDirection(x, y, 1, -1);
    end_y = y - TestDirection(x, y, 1, -1);
    while (end_x != x) {
        GameBoard[end_x--][end_y++] = CurrentDiskColor;
    }

    end_x = x - TestDirection(x, y, -1, 1);
    end_y = y + TestDirection(x, y, -1, 1);
    while (end_y != y) {
        GameBoard[end_x++][end_y--] = CurrentDiskColor;
    }
}
*//*
void Game::UpdateHintMask(void)
{
    int x, y;
    std::cout << std::endl;
    for(y = 0; y < boardTiles; ++y) {
        for(x = 0; x < boardTiles; ++x) {
            if(GameBoard[x][y] == White) {
                //HintMask[x][y] = White;
                std::cout << 'W';
            } else if(GameBoard[x][y] == Black) {
                //HintMask[x][y] = Black;
                std::cout << 'B';
            } else if(TestPosition(x, y) > 0) {
                ++hintCount;
                HintMask[x][y] = Hint;
                std::cout << 'X';
            } else {
                HintMask[x][y] = Empty;
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
*/
void Game::update(/*Game object*/)
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
    OthelloRender(width, height/*, object*/);
        
    // let imgui handle rest of the rendering process
    ImGui::Render();

    glViewport(0, 0, width, height);
    //glClearColor(1, 0, 0, 1);
    //glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(window);
}

/*bool Game::resetGame()
{
    if(reset_game)
    {
        reset_game = false;
        scoreWhite = 2;
        scoreBlack = 2;
        playerTurn = Black;
        return true;
    }
    return false;
}*/
bool Game::resetGame()
{
    if(bPlayer.reset_Game() || wPlayer.reset_Game())
    {
        return true;
    }
    return false;
}
/*
bool Game::changeBoardsize()
{
   if(boardSizeChanged)
    {
        boardSizeChanged = false;
        scoreWhite = 2;
        scoreBlack = 2;
        playerTurn = Black;
        return true;
    }
    return false;
}
*/
bool Game::changeBoardsize()
{
   if(bPlayer.boardSizeChanged() || wPlayer.boardSizeChanged())
    {
        return true;
    }
    return false;
}

/*bool Game::gameOver()
{
   if(game_over)
    {
        game_over = false;
       // if(Play_more)
       //reset_game = true;
        // reset_game();
       // scoreWhite = 2;
        //scoreBlack = 2;
        //else
        // close game --- call main.cpp on if events

        std::cout << "" << "GAME OVER!!" << "\n";
        if(scoreBlack > scoreWhite)
            std::cout << "Winner is Black!" << "\n";
        else if(scoreBlack == scoreWhite)
        {
            std::cout << "Draw, No winner!" << "\n";
        }  
        else
        {
            std::cout << "Winner is White!" << "\n";
        }  
        return true;
    }
    return false;
}
*/
bool Game::gameOver()
{
   if(bPlayer.game_Over() || wPlayer.game_Over())
    {
       // if(Play_more)
       //reset_game = true;
        // reset_game();
       // scoreWhite = 2;
        //scoreBlack = 2;
        //else
        // close game --- call main.cpp on if events

        std::cout << "" << "GAME OVER!!" << "\n";
        if(bPlayer.updateBlackScore() > wPlayer.updateWhiteScore())
            std::cout << "Winner is Black!" << "\n";
        else if(bPlayer.updateBlackScore() == wPlayer.updateWhiteScore())
        {
            std::cout << "Game is Draw, No winner!" << "\n";
        }  
        else
        {
            std::cout << "Winner is White!" << "\n";
        }  
        return true;
    }
    return false;
}
void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);

    // SDL events are passed to imgui
     ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT
        || (event.type == SDL_WINDOWEVENT
        && event.window.event == SDL_WINDOWEVENT_CLOSE
        && event.window.windowID == SDL_GetWindowID(window)))
    {
        isRunning = false;
    }
}

bool Game::gameRunning()
{
    return isRunning;
}

void Game::clean()
{
    ImGui_ImplSDL2_Shutdown();
    ImGui_ImplOpenGL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
/*
void Game::updateScore()
{
    std::cout << "Black score: " << scoreBlack << "\n";
    std::cout << "White score: " << scoreWhite << "\n";
}
*/
/*
void Game::updatePlayerTurn()
{
    if(pass_turn == true)
    {   if(CurrentDiskColor == White)
        {
            playerTurn = Black;
            CurrentDiskColor = Black;
        }
        else
        {
            playerTurn = White;
            CurrentDiskColor = White;
        }
        
        pass_turn = false;
    }
    
}
*/

#if (USE_DEBUG == 1)
void dbMessage(const std::string &s, bool crlf)
{
    std::cout << s;
    if(crlf)
        std::cout << std::endl;
}
#endif

