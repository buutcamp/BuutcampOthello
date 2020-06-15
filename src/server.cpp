/*
 * Server
 * server.cpp
 * ver 0.20
 */

#include "server.h"

//g++ -std=c++17 server.cpp -o server
//gcc server.c -o server 

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

int init_Server(void)
{
    return 0;
}

//#ifndef CLIENT_MODULE_IN_USE
//int main()
//{
//    return init_Server();
//}
//#endif
