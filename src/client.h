/*
 * Client
 * client.h
 * ver 0.20     //1st include to project
 * ver 0.25     //change from class to subroutine
 */

/* Sources
https://www.geeksforgeeks.org/socket-programming-cc/
https://www.bogotobogo.com/cplusplus/sockets_server_client.php
https://www.youtube.com/watch?v=0Zr_0Jy8mWE     TCP
https://www.youtube.com/watch?v=uIanSvWou1M     UDP

*/
#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <thread>       //https://en.cppreference.com/w/cpp/thread
#include "definet.h"


using str = std::string;
using str_vector = std::vector<str>;

//ver 0.25 change from class to subroutine
//class Client {
//    public:
//        Client();
//        ~Client();
//        int  Connect();
//        int  Disconnect();
//        int  PutMessage(const str text, const uint16_t flags);
//        bool GetMessage(str& text);
//        uint16_t GetClientStatus();
//        void Serving();
//    private:
//        struct cMsg {
//            int         id;
//            uint16_t    status;
//            str         cMessage;
//        };
//        int message_id;
//        std::vector<cMsg> MessagesIn;
//        std::vector<cMsg> MessagesOut;
//        struct sockaddr_in Server_addr;
//        struct sockaddr_in Client_addr;
//        int ServerSocket = 0;
//        int ValRead = 0;
//        int ServerPort = 8080;
//        uint16_t ClStatus;
//        bool isConnected;
//        char buffer[1024];
//        std::thread srv;
//};

//ver 0.25
//Messages
struct cMsg {
    int         id;
    uint16_t    status;
    str         cMessage;
};
void Client_Initialize();
void Client_Close();
int  Client_Connect();
int  Client_Disconnect();
int  Client_PutMessage(const str text, const uint16_t flags);
bool Client_GetMessage(str& text);
uint16_t GetClientStatus();
void Client_Serving(uint16_t KillTime);

#endif     //end _CLIENT_H_
