/*
 * Server
 * ver 0.20
 */
/*
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


using str = std::string;
using str_vector = std::vector<str>;

class Server {
    public:
        Server();
        ~Server();
        int  Start(const int port);
        int  Stop();
        int  PutMessage(const str text, const uint16_t flags);
        bool GetMessage(str& text);
        uint16_t GetServerStatus();
        void Serving();
    private:
        //Messages
        struct sMsg {
            int         id;
            uint16_t    status;
            str         sMessage;
        };
        int message_id;
        std::vector<sMsg> MessagesIn;
        std::vector<sMsg> MessagesOut;
        //TCP/IP
        struct sockaddr_in Server_addr;
        struct sockaddr_in Client_addr;
        int addrlen = sizeof(Server_addr);
        int ServerSocket = 0;
        int ServerPort = 8080;
        int ClientSocket = 0;
        int SocketOptions = 1;
        int ValRead = 0;
        uint16_t SrvStatus;
        bool isRunning = false;
        char buffer[1024];
};

#define ERR_OPEN_SOCKET     0x0001
#define ERR_CREATE_SOCKET   0x0002
#define ERR_SOCKET_OPTIONS  0x0004
#define ERR_BINDING         0x0008
#define ERR_LISTEN          0x0010
#define ERR_ACCEPTING       0x0020
#define ERR_CONNECTING      0x0040
#define ERR_INVALID_ADDRESS 0x0080

#endif     //end _SERVER_H_
