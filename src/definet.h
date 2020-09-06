/*
 * Definet
 * definet.h
 * ver 1.0
 */

#ifndef _DEFINET_H_
#define _DEFINET_H_

//enum pcs {Empty, White, Black, Hint};
enum player {Human_Local, Human_Remote, AI_Local, AI_Remote};
enum game_style {LocalGame, ClientGame, ServerGame};
enum Tiles {EMPTY, WHITE, BLACK, HINT};
enum Tiles_AI {AI_EMPTY = 0, AI_WHITE = -1, AI_BLACK = 1};

/* Sets constants */
#define WIDTH                   1000     // Window width
#define HEIGHT                  900     // Window height
#define BOARD_TILES             8       // Number of tiles in a row/column
#define USE_DEBUG               0       //1 = In use, 0 = Not used


//Communication settings
#define AddressString       "127.0.0.1"
#define PORT                8080

//Client and server status flags
//ClStatus and SrvStatus
#define ERR_OPEN_SOCKET     0x0001
#define ERR_CREATE_SOCKET   0x0002
#define ERR_SOCKET_OPTIONS  0x0004
#define ERR_BINDING         0x0008
#define ERR_LISTEN          0x0010
#define ERR_ACCEPTING       0x0020
#define ERR_CONNECTING      0x0040
#define ERR_INVALID_ADDRESS 0x0080


//Communication flags to separate AI and human messages

#define AI_FLAG             0x0001  //For future AI handle
#define AI_MOVE             0x0002  //Send AI's next move
#define AI_SOMETHING        0x0004  //For future use
#define AI_ILLEGAL_MOVE     0x0008  //Last move of AI was illegal, local and remote games out of synch

#define CHAT_TEXT           0x0010  //Send text field to other players chat screen
#define HUMAN_MOVE          0x0020  //Send human players move
#define HUMAN_SOMETHING     0x0040  //For future use
#define HUMAN_ILLEGAL_MOVE  0x0080  //Last move of human player was illegal, local and remote games out of synch

#define GAME_COMMAND        0x4000  //Send command to other game
#define RESYNCH_GAMETABLE   0x8000  //Synch gametables

#define CMND_REST_GAME      "RESET"         //Command remote game to reset
#define CMND_INIT_8X8       "GAME_8X8"      //Init remote game as 8x8 table
#define CMND_INIT_10X10     "GAME_10X10"    //Init remote game as 10x10 table
#define CMND_INIT_12X12     "GAME_12X12"    //Init remote game as 12x12 table
//Player 1 commands
#define CMND_P1_LOCAL       "P1L"           //P1 is local in remote end
#define CMND_P1_REMOTE      "P1R"           //P1 is remote in remote end
#define CMND_P1_HUMAN       "P1H"           //P1 is human in remote end
#define CMND_P1_AI          "P1AI"          //P1 is AI in remote end
//Player 2 commands
#define CMND_P2_LOCAL       "P2L"           //P2 is local in remote end
#define CMND_P2_REMOTE      "P2R"           //P2 is remote in remote end
#define CMND_P2_HUMAN       "P2H"           //P2 is human in remote end
#define CMND_P2_AI          "P2AI"          //P2 is AI in remote end

#endif