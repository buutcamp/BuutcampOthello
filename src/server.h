/*
 * Server
 * server.h
 * ver 0.20     //1st include to project
 * ver 0.25     //Server will be subclass for class Game
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
#include "othello.h"
#include "definet.h"

using str = std::string;
using str_vector = std::vector<str>;

//ver 0.25 change from class to subroutine
class Game;
class Server {
    Game* game;
    public:
        Server();
        ~Server();
        int  Server_Start(const int port);
        int  Server_Stop();
        int  Server_send(const str text, const uint16_t flags);
        bool Server_recv(str& text);
        uint16_t GetServerStatus();
        //void Server_Serving(uint16_t KillTime);
        void Server_Serving();
    private:
        //Messages
        struct sMsg {
            int         id;
            uint16_t    status;
            str         sMessage;
        };
        int Srv_message_id;
        std::vector<sMsg> Srv_MessagesIn;
        std::vector<sMsg> Srv_MessagesOut;
        //TCP/IP
        struct sockaddr_in Srv_Server_addr;
        struct sockaddr_in Srv_Client_addr;
        int Srv_addrlen = sizeof(Srv_Server_addr);
        int Srv_ServerSocket = 0;
        int Srv_ServerPort = 8080;
        int Srv_ClientSocket = 0;
        int Srv_SocketOptions = 1;
        int Srv_ValRead = 0;
        uint16_t SrvStatus;
        bool Srv_isRunning = false;
        char Srv_buffer[1024];
        std::thread Server_srv;
};

#endif     //end _SERVER_H_
