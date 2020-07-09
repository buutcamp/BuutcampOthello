/*
 * Othello game
 * ver 0.25
 */

#include "othello.h"

#if (USE_DEBUG == 1)
std::string txt;
#endif

std::string ver_txt = "ver 0.25";

Game::Game(int diskColor, int game_style) :
            GameStyle(game_style),
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
            scoreWhite(2),
            scoreBlack(2),
            playerTurn(Black),
            passed_gameTurn_counter(0),
            hintCount(0),
            CurrentDiskColor(diskColor),
            reset_game(false),
            boardSizeChanged(false),
            showHint(true),
            LocalLock(false),
            game_over(false),
            pass_turn(false)
            {
                GameBoard = std::vector<std::vector<int> >(boardTiles, std::vector<int>(boardTiles));
                HintMask = std::vector<std::vector<int> >(boardTiles, std::vector<int>(boardTiles));
                if(game_style == ClientGame)
                    client;
                if(game_style == ServerGame)
                    server;
                Player1;
                Player2;
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
        }
    }
    //Start placement
    //This way we can make 8*8, 10*10 and 12*12 board as different levels
    GameBoard[(boardTiles / 2) - 1][(boardTiles / 2) - 1] = White;
    GameBoard[boardTiles / 2][boardTiles / 2] = White;
    GameBoard[(boardTiles / 2) - 1][boardTiles / 2] = Black;
    GameBoard[boardTiles / 2][(boardTiles / 2) - 1] = Black;
    CurrentDiskColor = Black; // Player with Black discs begin game
    if (showHint)
        UpdateHintMask();

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

    /*
     * Client initialize and connect to server
     */
    if(GameStyle == ClientGame) {
        if(client.Client_Connect() == 0) {
            //Client connected
            std::cout << "Client found server." << std::endl;
        } else {
            //Could not connect to server!
            std::cout << "Couldn't connect to server!" << std::endl;
        }
    }

    /*
     * Server initialize and start listening
     */
    if(GameStyle == ServerGame) {
        //if(server.Server_Start(PORT) == 0) {
        //    //Server started and listening
        //    std::cout << "Server is listening port:" << PORT << std::endl;
        //} else {
        //    //Could not start server!
        //    std::cout << "Couldn't start server!" << std::endl;
        //}
    }
}

// game logic goes here, deltaTime is the time in seconds since last call to this function
void Game::OthelloFrame(float deltaTime)
{
    //std::cout << "Delta frame:" << deltaTime << std::endl;
}

// called when a tile was clicked
void Game::OnTileClicked(int x, int y)
{
    #if (USE_DEBUG == 1)
    txt = "Button X:" + std::to_string(x) + " Y:" + std::to_string(y);
    dbMessage(txt, true);
    #endif

    str txt;
    uint16_t flags;
    static int discs_counter = 4; // total number of discs placed on the board

    /*
     * 1st test if valid to make move
     * if (LocalLock == true) {
     *     if we are here from local player => exit
     * }
     * AI and remote games must lock local player out
     * until they have moves made
     */

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

            if(GameStyle == ClientGame) {
                //Set flags, who send this move
                flags = 0;
                //flags |= AI_MOVE;
                flags |= HUMAN_MOVE;
                //Make message from click position
                txt = std::to_string(x) + "," + std::to_string(y);
                //client.PutMessage(txt, flags);
                client.Client_send(txt, flags);
            }
            if(GameStyle == ServerGame) {
                //Set flags, who send this move
                flags = 0;
                //flags |= AI_MOVE;
                flags |= HUMAN_MOVE;
                //Make message from click position
                txt = std::to_string(x) + "," + std::to_string(y);
                //server.PutMessage(txt, flags);
                server.Server_send(txt, flags);
            }

            if(GameStyle == LocalGame) {
                //Local game
            }
            /*
             * Test if now turn for AI or remote
             * if so, LocalLock = true;
             * else LocalLock = false;
             */

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
    ImGui::Begin("Othello ", nullptr, ImGuiWindowFlags_NoDecoration);
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        // draw score discs
        drawList->AddCircleFilled(ImVec2(320, 30), diskRadius * 0.75, diskColorWhite, 30);
        drawList->AddCircleFilled(ImVec2(640, 30), diskRadius * 0.75, diskColorBlack, 30);

        ImGui::Dummy(ImVec2(300,45));
        ImGui::Spacing();
        ImGui::SameLine(315, 0);
        ImGui::TextColored(ImVec4(1,1,0,1), "%02d", scoreWhite);
        ImGui::SameLine(635, 0);
        ImGui::TextColored(ImVec4(1,1,0,1), "%02d", scoreBlack);

        //Draw player turn disc
        ImGui::SameLine(420, 0);
        ImGui::TextColored(ImVec4(1,1,0,1), "TURN");
        if(playerTurn == White)
        {
            drawList->AddCircleFilled(ImVec2(480, 80), diskRadius * 0.75, diskColorWhite, 30);
        }
            else
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
                    OnTileClicked(x, y);
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
        //draw combo dropbox
        static int item = 0;
        int current_item = item;
        const char* items[] = {"8x8", "10x10", "12x12"};

        ImGui::Dummy(ImVec2(0.0f, 1.0f));
        ImGui::PushItemWidth(100);
        ImGui::SameLine(230, 0);
        ImGui::Combo("BOARD SIZE", &item, items, IM_ARRAYSIZE(items));

        // set number of tiles/ board size based on the selected item
        if((GameStyle == LocalGame) || (GameStyle == ServerGame)) {
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
                boardSizeChanged = true;
                GameBoard.clear();
                GameBoard.resize(boardTiles,std::vector<int>(boardTiles));
                if (showHint)
                {
                    HintMask.clear();
                    HintMask.resize(boardTiles,std::vector<int>(boardTiles));
                }
            }
        }

        // Draw Reset button
         ImGui::SameLine(450, 0);
        if(ImGui::Button("RESET"))
        {
            reset_game = true;
        }
        // draw checkbox
        ImGui::SameLine(570, 0);
        ImGui::Checkbox("SHOW NEXT MOVE HINT", &showHint);
    }
    ImGui::End();
}

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

bool Game::resetGame()
{
    if(reset_game)
    {
        reset_game = false;
        scoreWhite = 2;
        scoreBlack = 2;
        playerTurn = Black;
        if(GameStyle == ServerGame) {
            /*
             * Send message to other player if not local game
             * Update new gameboard to remote site
             */
        }
        return true;
    }
    return false;
}

bool Game::changeBoardsize()
{
   if(boardSizeChanged)
    {
        boardSizeChanged = false;
        scoreWhite = 2;
        scoreBlack = 2;
        playerTurn = Black;
        if(GameStyle == ServerGame) {
            /*
             * Send message to other player if not local game
             * Update new gameboard to remote site
             */
        }
        return true;
    }
    return false;
}

bool Game::gameOver()
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
    /*
     * Shut down possible client or server!
     */
    if(GameStyle == ClientGame) {
        client.Client_Disconnect();
    }
    if(GameStyle == ServerGame) {
        server.Server_Stop();
    }
    ImGui_ImplSDL2_Shutdown();
    ImGui_ImplOpenGL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::updateScore()
{
    std::cout << "Black score: " << scoreBlack << "\n";
    std::cout << "White score: " << scoreWhite << "\n";
}

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

void Game::HandleRemoteMessages()
{
    uint16_t flags = 0;
    str text = "";
    int x = 0;
    int y = 0;
    bool new_msg = false;

    //Messages from game master server
    if(client.Client_recv(text, flags) == true) {
        //We have receved new message from remote player
        new_msg = true;
    }
    //Messages from remote player, which is client
    if(server.Server_recv(text, flags) == true) {
        //We have receved new message from remote player
        new_msg = true;
    }
    if(new_msg == false)
        return;

    /*
     * All messages are handled here
     */
    std::cout << "" << std::endl;
    if((flags & GAME_COMMAND) > 0) {
        //Game command
        std::cout << "Game command [" << text << "]" << std::endl;
        if(text == CMND_REST_GAME) {
            //Game master calls reset the game
            std::cout << "Game master reset game!" << std::endl;
        } else if(text == CMND_INIT_8X8) {
            //Set board to 8 x 8 mode
            std::cout << "Board set to 8x8" << std::endl;
        } else if( text == CMND_INIT_10X10) {
            //Set board to 10 x 10 mode
            std::cout << "Board set to 10x10" << std::endl;
        } else if( text == CMND_INIT_12X12) {
            //Set board to 12 x 12 mode
            std::cout << "Board set to 12x12" << std::endl;
        } else if( text == CMND_P1_LOCAL) {
            //Set player 1 as local
            std::cout << "Player 1 is local player" << std::endl;
        } else if( text == CMND_P1_REMOTE) {
            //Set player 1 as remote
            std::cout << "Player 1 is remote player" << std::endl;
        } else if( text == CMND_P1_HUMAN) {
            //Set player 1 as human
            std::cout << "Player 1 is human" << std::endl;
        } else if( text == CMND_P1_AI) {
            //Set player 1 as AI
            std::cout << "Player 1 is AI" << std::endl;
        } else if( text == CMND_P2_LOCAL) {
            //Set player 2 as local
            std::cout << "Player 2 is local player" << std::endl;
        } else if( text == CMND_P2_REMOTE) {
            //Set player 2 as remote
            std::cout << "Player 2 is remote player" << std::endl;
        } else if( text == CMND_P2_HUMAN) {
            //Set player 2 as human
            std::cout << "Player 2 is human" << std::endl;
        } else if( text == CMND_P2_AI) {
            //Set player 2 as AI
            std::cout << "Player 2 is AI" << std::endl;
        } else {
            //Unknown command!
            std::cout << "ERROR! Unknown command [" << text << "]" << std::endl;
        }
        return;
    }

    if((flags & AI_FLAG) > 0) {
        //Where AI want this message to sended?
        std::cout << "AI message: " << text << std::endl;
        //What we do with this in this state of game?
        return;
    }

    //if((flags & AI_MOVE) > 0) {
    //    //Use this, if AI-moves are handled diffrrently from humans moves
    //    //Check move data and call 'Game::OnTileClicked(int x, int y)'
    //    std::cout << "AI move " << text << std::endl;
    //    OnTileClicked(x, y);
    //    return;
    //}
    //if((flags & HUMAN_MOVE) > 0) {
    //    //Use this, if humans moves are handled diffrrently from AI-moves
    //    //Check move data and call 'Game::OnTileClicked(int x, int y)'
    //    std::cout << "Human move " << text << std::endl;
    //    OnTileClicked(x, y);
    //    return;
    //}
    if((flags & (AI_MOVE | HUMAN_MOVE)) > 0) {
        //Use this if human and AI move handling are not different
        //Check move data and call 'Game::OnTileClicked(int x, int y)'
        if(ParseMoveString(text, x, y) == 0) {
            std::cout << "Move " << text << std::endl;
            OnTileClicked(x, y);
        } else {
            std::cout << "Move data " << text << " was illegal!" << std::endl;
            //How we handle illegal move?
            //Other than send message of illegal move (HUMAN_ILLEGAL_MOVE or AI_ILLEGAL_MOVE)
        }
        return;
    }
    if((flags & (AI_ILLEGAL_MOVE | HUMAN_ILLEGAL_MOVE)) > 0) {
        //Illegal move, we must synch game tables
        std::cout << "AI or human made illegal move! " << text << std::endl;
        //How we handle illegal move and move history?
        return;
    }

    if((flags & CHAT_TEXT) > 0) {
        //Where we print chat-text?
        std::cout << "Chat [" << text << "]." << std::endl;
        //Send text to textbox
        return;
    }

    if((flags & RESYNCH_GAMETABLE) > 0) {
        //We get other sides gametable to overwrite this ones
        std::cout << "Gameboard resynch, we write GameBoard with following data: " << text << std::endl;
        return;
    }

    if((flags & HUMAN_SOMETHING) > 0) {
        //For future use of human players message
        std::cout << "Messagetype HUMAN_SOMETHING, text:" << text << std::endl;
        return;
    }

    if((flags & AI_SOMETHING) > 0) {
        //For future use of AI players message
        std::cout << "Messagetype AI_SOMETHING, text:" << text << std::endl;
        return;
    }

    //If we get here, we got something wrong with message
    std::ios_base::fmtflags f(std::cout.flags());
    std::cout << "Unknown parameters in message!" << std::endl;
    std::cout << "Text [" << text << "]" << std::endl;
    std::cout << "Flags = " << std::uppercase << std::showbase << std::hex << flags << std::endl;
    std::cout << std::endl;
    std::cout.flags(f);
}

int Game::ParseMoveString(const str text, int& x, int& y)
{
    str_vector test = {};
    str temp;
    int i;
    temp = "";
    for(i = 0; i < text.length(); ++i) {
        if(text[i] == ' ')
            continue;
        if(text[i] == ',') {
            test.push_back(temp);
            temp = "";
        } else 
            temp += text[i];
    }

    if(test.size() != 2)
        return -1;

    try
    {
        x = std::stoi(test[0]);
        y = std::stoi(test[1]);
    }
    catch(std::invalid_argument const &e)
    {
        std::cout << "Not valid numbers! [" << test[0] << "] or [" << test[1] << "]" << std::endl;
        return -1;
    }
    if((x >= boardTiles) || (y >= boardTiles))
        return -1;

    return 0;
}

#if (USE_DEBUG == 1)
void dbMessage(const std::string &s, bool crlf)
{
    std::cout << s;
    if(crlf)
        std::cout << std::endl;
}
#endif
