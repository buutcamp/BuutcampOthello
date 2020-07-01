/*
 * Client
 * client.cpp
 * ver 0.20
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

Client::Client(/* args */)
{
    MessagesIn = {};
    MessagesOut = {};
    message_id = 0;
    ValRead = 0;
    ClStatus = 0;
    ServerSocket = 0;
    ServerPort = PORT;

    if ((ServerSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        ClStatus |= ERR_CREATE_SOCKET;
        #if (USE_DEBUG == 1)
        dbMessage("Can't create Server socket!", true);
        #endif
    }

    Server_addr.sin_family = AF_INET;
    Server_addr.sin_port = ServerPort;

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, AddressString, &Server_addr.sin_addr) <= 0) {
        ClStatus |= ERR_INVALID_ADDRESS;
        #if (USE_DEBUG == 1)
        dbMessage("Invalid address/ Address not supported", true);
        #endif
    }

    memset(buffer, 0, sizeof(buffer));
    isConnected = false;
}

Client::~Client()
{
    Disconnect();
    MessagesIn = {};
    MessagesOut = {};
}

int Client::Connect()
{
    if (connect(ServerSocket, (struct sockaddr *)&Server_addr, sizeof(Server_addr)) < 0) {
        ClStatus |= ERR_CONNECTING;
        isConnected = false;
        return -1;
    } else {
        ClStatus &= ~(ERR_CONNECTING);
        isConnected = true;
        srv = std::thread(&Client::Serving, this);
        return 0;
    }
}

int Client::Disconnect()
{
    close(ServerSocket);
    isConnected = false;
    return 0;
}

int Client::PutMessage(const str text, const uint16_t flags)
{
    cMsg temp_msg;
    temp_msg.cMessage = text;
    temp_msg.id = message_id++;
    temp_msg.status = flags;
    MessagesOut.push_back(temp_msg);
    return 0;
}

bool Client::GetMessage(str& text)
{
    uint16_t tst;
    if(MessagesIn.empty()) {
        return false;
    } else {
        text = MessagesIn[0].cMessage;
        //MessagesIn[0].id;
        tst = MessagesIn[0].status;
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

        MessagesIn.erase(MessagesIn.begin());
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
void Client::Serving()
{
    cMsg temp;
    uint32_t KillSwitch;

    KillSwitch = 0;
    std::cout << "Client thread ID=" << std::this_thread::get_id() << std::endl;
    while (isConnected)
    {
        ValRead = read(ServerSocket, buffer, 1024);
        if(ValRead < 0) {
            std::cout << "Error reading socket!" << std::endl;
        } else {
            std::copy(&temp, &temp + 1, reinterpret_cast<cMsg*>(buffer));
            MessagesIn.push_back(temp);
            KillSwitch = 0;
        }

        if(!MessagesOut.empty()) {
            temp = MessagesOut[0];
            MessagesOut.erase(MessagesOut.begin());
            memcpy(buffer, (const unsigned char*)&temp, sizeof(temp));
            send(ServerSocket, buffer, strlen(buffer), 0);
            KillSwitch = 0;
        }
        std::this_thread::sleep_for(1s);

        //We don't want this run eternity here, so what kind time is long enough?
        ++KillSwitch;
        if(KillSwitch > 60000)
            break;
    }

}
