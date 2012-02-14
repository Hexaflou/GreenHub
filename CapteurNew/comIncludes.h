#ifndef H_COMINCLUDES
#define H_COMINCLUDES
/***********************************************************************
 * 	Includes nécessaires pour tout les fichiers utilisant les sockets
 * 
 * ********************************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
#define MAX_MQ_SIZE    1024

#endif
