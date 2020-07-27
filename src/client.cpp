/*
 * Client
 * client.cpp
 * ver 0.20     //1st include to project
 * ver 0.25     //Client will be subclass for class Game
 */

#include "othello.h"
//#include "client.h"

/*
 * Client Socket
 *
 *  create a socket.
 *  bind* - this is probably be unnecessary because you're the client, not the server.
 *  connect to a server.
 *  send/recv - repeat until we have or receive data
 *  shutdown to end read/write.
 *  close to releases data.
 */

using namespace std::literals::chrono_literals;

Client::Client()
{
    Cl_MessagesIn = {};
    Cl_MessagesOut = {};
    Cl_message_id = 0;
    Cl_ValRead = 0;
    ClStatus = 0;
    Cl_ServerSocket = 0;
    Cl_ServerPort = PORT;

    memset(Cl_buffer, 0, sizeof(Cl_buffer));
    //Test start
    //cMsg temp;
    //temp.id = 1354;
    //temp.cMessage = "Hello world!";
    //temp.status = 666;
    //
    //std::cout << "   ID: " << temp.id << std::endl;
    //std::cout << "  Msg: " << temp.cMessage << std::endl;
    //std::cout << "Flags: " << temp.status << std::endl;
    //memcpy(Cl_buffer, (const unsigned char*)&temp, cMsg_size);
    //temp = {};
    //
    //std::copy(&temp, &temp + 1, reinterpret_cast<cMsg*>(Cl_buffer));
    //memcpy(&temp, Cl_buffer, cMsg_size);
    //std::cout << "   ID: " << temp.id << std::endl;
    //std::cout << "  Msg: " << temp.cMessage << std::endl;
    //std::cout << "Flags: " << temp.status << std::endl;
    //Test end

    memset(Cl_buffer, 0, sizeof(Cl_buffer));
    Cl_isConnected = false;
}

Client::~Client()
{
    std::cout << "Close client." << std::endl;
    Client_Disconnect();
    Cl_MessagesIn = {};
    Cl_MessagesOut = {};
}

int Client::Client_Connect()
{
    if ((Cl_ServerSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        ClStatus |= ERR_CREATE_SOCKET;
        std::cout << "Can't create Server socket!" << std::endl;
    } else {
        std::cout << "Client socket:" << Cl_ServerSocket << std::endl;
    }

    Cl_Server_addr.sin_family = AF_INET;
    Cl_Server_addr.sin_port = htons(Cl_ServerPort);

    // Convert IPv4 and IPv6 addresses from text to binary form
    //if(inet_pton(AF_INET, AddressString, &Cl_Server_addr.sin_addr) <= 0) {
    if(inet_pton(AF_INET, "127.0.0.1", &Cl_Server_addr.sin_addr) <= 0) {
        ClStatus |= ERR_INVALID_ADDRESS;
        std::cout << "Invalid address/ Address not supported" << std::endl;
    } else {
        std::cout << "Client have server address." << std::endl;
        //send(Cl_ServerSocket, "Hello from client", strlen("Hello from client"), 0);
    }

    if (connect(Cl_ServerSocket, (struct sockaddr *)&Cl_Server_addr, sizeof(Cl_Server_addr)) < 0) {
        ClStatus |= ERR_CONNECTING;
        Cl_isConnected = false;
        std::cout << "Client couln't connect!" << std::endl;
        return -1;
    } else {
        ClStatus &= ~(ERR_CONNECTING);
        Cl_isConnected = true;
        //std::thread Cl_srv(Client_Serving, 60000);
        std::cout << "Client connected to server." << std::endl;
        Cl_srv = std::thread(&Client::Client_Serving, this);
        return 0;
    }
}

int Client::Client_Disconnect()
{
    int close_ret = close(Cl_ServerSocket);
    std::cout << "Client disconnect. Return val:" << close_ret << std::endl;
    Cl_isConnected = false;
    return 0;
}

int Client::Client_send(const str text, const uint16_t flags)
{
    cMsg temp_msg;
    std::cout << "Client send message [" << text << "] flags:" << flags << std::endl;
    temp_msg.cMessage = text;
    temp_msg.id = Cl_message_id++;
    temp_msg.status = flags;
    Cl_MessagesOut.push_back(temp_msg);
    return 0;
}

bool Client::Client_recv(str& text, uint16_t& flags)
{
    if(Cl_MessagesIn.empty()) {
        std::cout << "Client message buffer empty." << std::endl;
        return false;
    } else {
        text = Cl_MessagesIn[0].cMessage;
        //Cl_MessagesIn[0].id;
        flags = Cl_MessagesIn[0].status;

        std::cout << "Client got new message [" << Cl_MessagesIn[0].cMessage <<
            "] flags:" << Cl_MessagesIn[0].status <<
            "ID:" << Cl_MessagesIn[0].id << std::endl;
        //Remove oldest in FIFO

        Cl_MessagesIn.erase(Cl_MessagesIn.begin());
        return true;
    }
}

uint16_t Client::GetClientStatus()
{
    return ClStatus;
}

/*
 * Message thread
 */
//void Client::Client_Serving(uint16_t KillTime)
void Client::Client_Serving()
{
    uint16_t KillTime = 60000;
    cMsg temp;
    uint32_t KillSwitch;

    KillSwitch = 0;
    std::cout << "Client thread ID=" << std::this_thread::get_id() << std::endl;
    while (Cl_isConnected)
    {
        Cl_ValRead = read(Cl_ServerSocket, Cl_buffer, 1024);
        if(Cl_ValRead < 0) {
            std::cout << "Error reading socket!" << std::endl;
        } else {
            //std::copy(&temp, &temp + 1, reinterpret_cast<cMsg*>(Cl_buffer));
            memcpy(&temp, Cl_buffer, cMsg_size);
            Cl_MessagesIn.push_back(temp);
            KillSwitch = 0;
        }

        if(!Cl_MessagesOut.empty()) {
            temp = Cl_MessagesOut[0];
            Cl_MessagesOut.erase(Cl_MessagesOut.begin());
            memcpy(Cl_buffer, (const unsigned char*)&temp, cMsg_size);
            send(Cl_ServerSocket, Cl_buffer, strlen(Cl_buffer), 0);
            KillSwitch = 0;
        }
        std::this_thread::sleep_for(1s);

        //We don't want this run eternity here, so what kind time is long enough?
        ++KillSwitch;
        if(KillSwitch > KillTime)
            break;
    }

}
