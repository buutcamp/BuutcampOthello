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

#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 

#define PORT 8080

int init_Server(void);


#endif     //end _SERVER_H_
