/*
 * Server
 * server.cpp
 * ver 0.20
 */

#include "server.h"
#include <thread>       //https://en.cppreference.com/w/cpp/thread

//g++ -std=c++17 server.cpp -o server
//gcc server.c -o server
//https://en.cppreference.com/w/

/*
 * Server Socket
 *
 *  create a socket - Get the file descriptor!
 *  bind to an address -What port am I on?
 *  listen on a port, and wait for a connection to be established.
 *  accept the connection from a client.
 *  send/recv - the same way we read and write for a file.
 *  shutdown to end read/write.
 *  close to releases data.
 */

using namespace std::literals::chrono_literals;

Server::Server()
{
    MessagesIn = {};
    MessagesOut = {};
    message_id = 0;
    ValRead = 0;
    SrvStatus = 0;
    SocketOptions = 1;
    ClientSocket = 0;

    if((ServerSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        SrvStatus |= ERR_CREATE_SOCKET;
        #if (USE_DEBUG == 1)
        dbMessage("Can't create Server socket!", true);
        #endif
    } else if(setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &SocketOptions, sizeof(SocketOptions))) {
        SrvStatus |= ERR_SOCKET_OPTIONS;
        #if (USE_DEBUG == 1)
        dbMessage("Server socket options error!", true);
        #endif
    }
    Server_addr.sin_family = AF_INET;
    Server_addr.sin_addr.s_addr = INADDR_ANY;
    Server_addr.sin_port = ServerPort;

    memset(buffer, 0, sizeof(buffer));
    isRunning = false;
}

Server::~Server()
{
    close(ClientSocket);
    close(ServerSocket);
    MessagesIn = {};
    MessagesOut = {};
}

int Server::Start(const int port)
{
    ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ServerSocket < 0) {
        SrvStatus |= ERR_OPEN_SOCKET;
        //Error opening socket
        return ServerSocket;
    }
    SrvStatus &= ~(ERR_OPEN_SOCKET);
    explicit_bzero((char *) &Server_addr, sizeof(Server_addr));
    ServerPort = port;

    // Forcefully attaching socket to the port 8080
    if (bind(ServerSocket, (struct sockaddr *)&Server_addr, sizeof(Server_addr)) < 0) {
        SrvStatus |= ERR_BINDING;
        return -1;
    }

    if (listen(ServerSocket, 3) < 0) {
        SrvStatus |= ERR_LISTEN;
        return -1;
    }

    if ((ClientSocket = accept(ServerSocket, (struct sockaddr *)&Server_addr, (socklen_t*)&addrlen)) < 0) {
        SrvStatus |= ERR_ACCEPTING;
        return -1;
    }

    isRunning = true;
    std::thread srv(Serving);
    return 0;
}

int Server::Stop()
{
    //std::thread srv.join();
    close(ClientSocket);
    isRunning = false;
    return 0;
}

int Server::PutMessage(const str text, const uint16_t flags)
{
    sMsg temp_msg;
    temp_msg.sMessage = text;
    temp_msg.id = message_id++;
    temp_msg.status = flags;
    MessagesOut.push_back(temp_msg);
    return 0;
}

bool Server::GetMessage(str& text)
{
    if(MessagesIn.empty()) {
        return false;
    } else {
        text = MessagesIn[0].sMessage;
        //Messages[0].id;
        //Messages[0].status;
        /*
         * Message flags handle here!
         */
        MessagesIn.erase(MessagesIn.begin());
        return true;
    }
}

uint16_t Server::GetServerStatus()
{
    return SrvStatus;
}

/*
 * Message thread
 */
void Server::Serving()
{
    sMsg temp;
    uint32_t KillSwitch;

    KillSwitch = 0;
    std::cout << "Server thread ID=" << std::this_thread::get_id() << std::endl;
    while (isRunning)
    {
        ValRead = read(ClientSocket, buffer, 1024);
        if(ValRead < 0) {
            std::cout << "Error reading socket!" << std::endl;
        } else {
            std::copy(&temp, &temp + 1, reinterpret_cast<sMsg*>(buffer));
            MessagesIn.push_back(temp);
            KillSwitch = 0;
        }

        if(!MessagesOut.empty()) {
            temp = MessagesOut[0];
            MessagesOut.erase(MessagesOut.begin());
            memcpy(buffer, (const unsigned char*)&temp, sizeof(temp));
            send(ClientSocket, buffer, strlen(buffer), 0);
            KillSwitch = 0;
        }
        std::this_thread::sleep_for(1s);

        //We don't want this run eternity here, so what kind time is long enough?
        ++KillSwitch;
        if(KillSwitch > 60000)
            break;
    }
    
}
