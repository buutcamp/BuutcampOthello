/*
 * Server
 * server.h
 * ver 0.20     //1st include to project
 * ver 0.25     //change from class to subroutine
 */

/* Sources
https://www.geeksforgeeks.org/socket-programming-cc/
https://www.bogotobogo.com/cplusplus/sockets_server_client.php
https://www.youtube.com/watch?v=WDn-htpBlnU     TCP
https://www.youtube.com/watch?v=uIanSvWou1M     UDP

*/
#ifndef _SERVER_H_
#define _SERVER_H_

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <vector>
#include <thread>       //https://en.cppreference.com/w/cpp/thread
#include "definet.h"

using str = std::string;
using str_vector = std::vector<str>;

//ver 0.25 change from class to subroutine
//class Server {
//    public:
//        Server();
//        ~Server();
//        int  Start(const int port);
//        int  Stop();
//        int  PutMessage(const str text, const uint16_t flags);
//        bool GetMessage(str& text);
//        uint16_t GetServerStatus();
//        void Serving();
//    private:
//        //Messages
//        struct sMsg {
//            int         id;
//            uint16_t    status;
//            str         sMessage;
//        };
//        int message_id;
//        std::vector<sMsg> MessagesIn;
//        std::vector<sMsg> MessagesOut;
//        //TCP/IP
//        struct sockaddr_in Server_addr;
//        struct sockaddr_in Client_addr;
//        int addrlen = sizeof(Server_addr);
//        int ServerSocket = 0;
//        int ServerPort = 8080;
//        int ClientSocket = 0;
//        int SocketOptions = 1;
//        int ValRead = 0;
//        uint16_t SrvStatus;
//        bool isRunning = false;
//        char buffer[1024];
//        std::thread srv;
//};

//ver 0.25
//Messages
struct sMsg {
    int         id;
    uint16_t    status;
    str         sMessage;
};

void Server_Initialize();
void Server_Close();
int  Server_Start(const int port);
int  Server_Stop();
int  Server_PutMessage(const str text, const uint16_t flags);
bool Server_GetMessage(str& text);
uint16_t GetServerStatus();
void Server_Serving(uint16_t KillTime);

#endif     //end _SERVER_H_
