/*
 * Client
 * client.cpp
 * ver 0.20     //1st include to project
 * ver 0.25     //change from class to subroutine
 */

#include "client.h"

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
//ver 0.25
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

//Client::Client(/* args */)
void Client_Initialize()
{
    Cl_MessagesIn = {};
    Cl_MessagesOut = {};
    Cl_message_id = 0;
    Cl_ValRead = 0;
    ClStatus = 0;
    Cl_ServerSocket = 0;
    Cl_ServerPort = PORT;

    if ((Cl_ServerSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        ClStatus |= ERR_CREATE_SOCKET;
        #if (USE_DEBUG == 1)
        dbMessage("Can't create Server socket!", true);
        #endif
    }

    Cl_Server_addr.sin_family = AF_INET;
    Cl_Server_addr.sin_port = Cl_ServerPort;

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, AddressString, &Cl_Server_addr.sin_addr) <= 0) {
        ClStatus |= ERR_INVALID_ADDRESS;
        #if (USE_DEBUG == 1)
        dbMessage("Invalid address/ Address not supported", true);
        #endif
    }

    memset(Cl_buffer, 0, sizeof(Cl_buffer));
    Cl_isConnected = false;
}

//Client::~Client()
void Client_Close()
{
    Client_Disconnect();
    Cl_MessagesIn = {};
    Cl_MessagesOut = {};
}

//int Client::Connect()
int  Client_Connect()
{
    if (connect(Cl_ServerSocket, (struct sockaddr *)&Cl_Server_addr, sizeof(Cl_Server_addr)) < 0) {
        ClStatus |= ERR_CONNECTING;
        Cl_isConnected = false;
        return -1;
    } else {
        ClStatus &= ~(ERR_CONNECTING);
        Cl_isConnected = true;
        std::thread Cl_srv(Client_Serving, 60000);
        //srv = std::thread(&Client::Serving, this);
        return 0;
    }
}

//int Client::Disconnect()
int  Client_Disconnect()
{
    close(Cl_ServerSocket);
    Cl_isConnected = false;
    return 0;
}

//int Client::PutMessage(const str text, const uint16_t flags)
int Client_PutMessage(const str text, const uint16_t flags)
{
    cMsg temp_msg;
    temp_msg.cMessage = text;
    temp_msg.id = Cl_message_id++;
    temp_msg.status = flags;
    Cl_MessagesOut.push_back(temp_msg);
    return 0;
}

//bool Client::GetMessage(str& text)
bool Client_GetMessage(str& text)
{
    uint16_t tst;
    if(Cl_MessagesIn.empty()) {
        return false;
    } else {
        text = Cl_MessagesIn[0].cMessage;
        //Cl_MessagesIn[0].id;
        tst = Cl_MessagesIn[0].status;
        if((tst & AI_FLAG) > 0) {
            //Where AI want this message to sended?
            std::cout << "AI message: " << text << std::endl;
        }
        if((tst & AI_MOVE) > 0) {
            //Use this, if AI-moves are handled diffrrently from humans moves
            //Check move data and call 'Game::OnTileClicked(int x, int y)'
            std::cout << "AI move " << text << std::endl;
        }
        if((tst & CHAT_TEXT) > 0) {
            //Where we print chat-text?
            std::cout << "Chat [" << text << "]." << std::endl;
        }
        if((tst & HUMAN_MOVE) > 0) {
            //Use this, if humans moves are handled diffrrently from AI-moves
            //Check move data and call 'Game::OnTileClicked(int x, int y)'
            std::cout << "Human move " << text << std::endl;
        }
        if((tst & (AI_MOVE | HUMAN_MOVE)) > 0) {
            //Use this if human and AI move handling are not different
            //Check move data and call 'Game::OnTileClicked(int x, int y)'
            std::cout << "Move " << text << std::endl;
        }

        Cl_MessagesIn.erase(Cl_MessagesIn.begin());
        return true;
    }
}

//uint16_t Client::GetClientStatus()
uint16_t GetClientStatus()
{
    return ClStatus;
}

/*
 * Message thread
 */
//void Client::Serving()
void Client_Serving(uint16_t KillTime)
{
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
            std::copy(&temp, &temp + 1, reinterpret_cast<cMsg*>(Cl_buffer));
            Cl_MessagesIn.push_back(temp);
            KillSwitch = 0;
        }

        if(!Cl_MessagesOut.empty()) {
            temp = Cl_MessagesOut[0];
            Cl_MessagesOut.erase(Cl_MessagesOut.begin());
            memcpy(Cl_buffer, (const unsigned char*)&temp, sizeof(temp));
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
