/*
 * Client
 * client.cpp
 * ver 0.20
 */

#include "client.h"

//g++ -std=c++17 client.cpp -o client
//gcc client.c -o client

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

int init_Client(void)
{
    return 0;
}

//#ifndef CLIENT_MODULE_IN_USE
//int main()
//{
//    return init_Client();
//}
//#endif
