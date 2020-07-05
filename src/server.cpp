/*
 * Server
 * server.cpp
 * ver 0.20     //1st include to project
 * ver 0.25     //Server will be subclass for class Game
 */

#include "othello.h"
//#include "server.h"

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
    Srv_MessagesIn = {};
    Srv_MessagesOut = {};
    Srv_message_id = 0;
    Srv_ValRead = 0;
    SrvStatus = 0;
    Srv_SocketOptions = 1;
    Srv_ClientSocket = 0;
    std::cout << "Server init." << std::endl;
    memset(Srv_buffer, 0, sizeof(Srv_buffer));
    Srv_isRunning = false;
}

Server::~Server()
{
    std::cout << "Close server." << std::endl;
    close(Srv_ClientSocket);
    close(Srv_ServerSocket);
    Srv_MessagesIn = {};
    Srv_MessagesOut = {};
}

int Server::Server_Start(const int port)
{
    std::cout << "Start server." << std::endl;
    if((Srv_ServerSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        SrvStatus |= ERR_CREATE_SOCKET;
        #if (USE_DEBUG == 1)
        dbMessage("Can't create Server socket!", true);
        #endif
        return Srv_ServerSocket;
    } else if(setsockopt(Srv_ServerSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &Srv_SocketOptions, sizeof(Srv_SocketOptions))) {
        SrvStatus |= ERR_SOCKET_OPTIONS;
        #if (USE_DEBUG == 1)
        dbMessage("Server socket options error!", true);
        #endif
        return -1;
    }
    Srv_Server_addr.sin_family = AF_INET;
    Srv_Server_addr.sin_addr.s_addr = INADDR_ANY;
    Srv_Server_addr.sin_port = Srv_ServerPort;

    Srv_ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (Srv_ServerSocket < 0) {
        SrvStatus |= ERR_OPEN_SOCKET;
        //Error opening socket
        return Srv_ServerSocket;
    }
    std::cout << "Server socket:" << Srv_ServerSocket << std::endl;
    SrvStatus &= ~(ERR_OPEN_SOCKET);
    explicit_bzero((char *) &Srv_Server_addr, sizeof(Srv_Server_addr));
    Srv_ServerPort = port;

    // Forcefully attaching socket to the port 8080
    if (bind(Srv_ServerSocket, (struct sockaddr *)&Srv_Server_addr, sizeof(Srv_Server_addr)) < 0) {
        SrvStatus |= ERR_BINDING;
        return -1;
    }

    if (listen(Srv_ServerSocket, 3) < 0) {
        SrvStatus |= ERR_LISTEN;
        return -1;
    }

    if ((Srv_ClientSocket = accept(Srv_ServerSocket, (struct sockaddr *)&Srv_Server_addr, (socklen_t*)&Srv_addrlen)) < 0) {
        SrvStatus |= ERR_ACCEPTING;
        return -1;
    }

    Srv_isRunning = true;
    //std::thread Server_srv(Server_Serving, 60000);
    Server_srv = std::thread(&Server::Server_Serving, this);
    return 0;
}

int Server::Server_Stop()
{
    close(Srv_ClientSocket);
    Srv_isRunning = false;
    return 0;
}

int Server::Server_send(const str text, const uint16_t flags)
{
    sMsg temp_msg;
    temp_msg.sMessage = text;
    temp_msg.id = Srv_message_id++;
    temp_msg.status = flags;
    Srv_MessagesOut.push_back(temp_msg);
    return 0;
}

bool Server::Server_recv(str& text)
{
    uint16_t tst;
    int x = 0;
    int y = 0;
    if(Srv_MessagesIn.empty()) {
        return false;
    } else {
        text = Srv_MessagesIn[0].sMessage;
        //MessagesIn[0].id;
        tst = Srv_MessagesIn[0].status;
        if((tst & RESYNCH_GAMETABLE) > 0) {
            //We get other sides gametable to overwrite this ones
            //if(game::boardSize)
        }
        if((tst & (AI_ILLEGAL_MOVE | HUMAN_ILLEGAL_MOVE)) > 0) {
            //Illegal move, we must synch game tables
            std::cout << "AI made illegal move! " << text << std::endl;
        }
        if((tst & AI_FLAG) > 0) {
            //Where AI want this message to sended?
            std::cout << "AI message: " << text << std::endl;
        }
        if((tst & AI_MOVE) > 0) {
            //Use this, if AI-moves are handled diffrrently from humans moves
            //Check move data and call 'Game::OnTileClicked(int x, int y)'
            std::cout << "AI move " << text << std::endl;
            game->OnTileClicked(x, y);
        }
        if((tst & CHAT_TEXT) > 0) {
            //Where we print chat-text?
            std::cout << "Chat [" << text << "]." << std::endl;
        }
        if((tst & HUMAN_MOVE) > 0) {
            //Use this, if humans moves are handled diffrrently from AI-moves
            //Check move data and call 'Game::OnTileClicked(int x, int y)'
            std::cout << "Human move " << text << std::endl;
            game->OnTileClicked(x, y);
        }
        if((tst & (AI_MOVE | HUMAN_MOVE)) > 0) {
            //Use this if human and AI move handling are not different
            //Check move data and call 'Game::OnTileClicked(int x, int y)'
            std::cout << "Move " << text << std::endl;
        }

        Srv_MessagesIn.erase(Srv_MessagesIn.begin());
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
//void Server::Server_Serving(uint16_t KillTime)
void Server::Server_Serving()
{
    uint16_t KillTime = 60000;
    sMsg temp;
    uint32_t KillSwitch;

    KillSwitch = 0;
    std::cout << "Server thread ID=" << std::this_thread::get_id() << std::endl;
    while (Srv_isRunning)
    {
        Srv_ValRead = read(Srv_ClientSocket, Srv_buffer, 1024);
        if(Srv_ValRead < 0) {
            std::cout << "Error reading socket!" << std::endl;
        } else {
            std::copy(&temp, &temp + 1, reinterpret_cast<sMsg*>(Srv_buffer));
            Srv_MessagesIn.push_back(temp);
            KillSwitch = 0;
        }

        if(!Srv_MessagesOut.empty()) {
            temp = Srv_MessagesOut[0];
            Srv_MessagesOut.erase(Srv_MessagesOut.begin());
            memcpy(Srv_buffer, (const unsigned char*)&temp, sizeof(temp));
            send(Srv_ClientSocket, Srv_buffer, strlen(Srv_buffer), 0);
            KillSwitch = 0;
        }
        std::this_thread::sleep_for(1s);

        //We don't want this run eternity here, so what kind time is long enough?
        ++KillSwitch;
        if(KillSwitch > KillTime)
            break;
    }
}