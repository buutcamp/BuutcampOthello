/*
 * Definet
 * definet.h
 * ver 1.0
 */

#ifndef _DEFINET_H_
#define _DEFINET_H_

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
#define CHAT_TEXT           0x0100  //to tell text field sended to chat screen
#define HUMAN_MOVE          0x0200  //to tell human players move


#endif
