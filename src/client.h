/*
 * Client
 * client.h
 * ver 0.20
 */
/*
https://www.geeksforgeeks.org/socket-programming-cc/
https://www.bogotobogo.com/cplusplus/sockets_server_client.php
https://www.youtube.com/watch?v=0Zr_0Jy8mWE     TCP
https://www.youtube.com/watch?v=uIanSvWou1M     UDP

*/
#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 

#define PORT 8080 

int init_Client(void);

#endif     //end _CLIENT_H_
