/*
 * Definet
 * definet.h
 * ver 1.0
 */

#ifndef _DEFINET_H_
#define _DEFINET_H_

#define SERVER_MODULE_IN_USE        //Comment this line if not used
#define CLIENT_MODULE_IN_USE        //Comment this line if not used

/* Sets constants */
#define WIDTH                   1000     // Window width
#define HEIGHT                  800     // Window height
#define BOARD_TILES             8       // Number of tiles in a row/column
//#define USE_HINT_MASK           1       //1 = Used in game, 0 = Not in use
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
#define AI_FLAG             0x0001  //for future AI handle
#define AI_MOVE             0x0002  //to tell AI's next move
#define AI_SOMETHING        0x0004  //for future use
#define AI_ILLEGAL_MOVE     0x0008  //last move was illegal, local and remote games out of synch

#define CHAT_TEXT           0x0010  //to tell text field sended to chat screen
#define HUMAN_MOVE          0x0020  //to tell human players move
#define HUMAN_SOMETHING     0x0040  //for future use
#define HUMAN_ILLEGAL_MOVE  0x0080  //last move was illegal, local and remote games out of synch

#define RESYNCH_GAMETABLE   0x8000  //Synch gametables


#endif