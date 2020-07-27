/*
 * Client
 * client.h
 * ver 0.20     //1st include to project
 * ver 0.25     //Client will be subclass for class Game
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
#include "othello.h"
#include "definet.h"


using str = std::string;
using str_vector = std::vector<str>;

//class Game;

class Client {
    //Game* game;
    friend class Game;

    public:
        Client();
        ~Client();
        int  Client_Connect();
        int  Client_Disconnect();
        int  Client_send(const str text, const uint16_t flags);
        bool Client_recv(str& text, uint16_t& flags);
        uint16_t GetClientStatus();
        //void Client_Serving(uint16_t KillTime);
        void Client_Serving();
        int cMsg_size = sizeof(cMsg);
    private:
        //Messages
        struct cMsg {
            int         id;
            uint16_t    status;
            str         cMessage;
        };
        int Cl_message_id;
        std::vector<cMsg> Cl_MessagesIn;
        std::vector<cMsg> Cl_MessagesOut;
        struct sockaddr_in Cl_Server_addr;
        struct sockaddr_in Cl_Client_addr;
        int Cl_ServerSocket = 0;
        int Cl_ValRead = 0;
        int Cl_ServerPort = 8080;
        uint16_t ClStatus;
        bool Cl_isConnected;
        char Cl_buffer[1024];
        std::thread Cl_srv;
};

#endif     //end _CLIENT_H_
