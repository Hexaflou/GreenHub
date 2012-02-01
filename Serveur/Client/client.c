// compilation :   gcc -Wall -o client.o  -c client.c
//         gcc -o client client.o

#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>


#define SERVEURNAME "127.0.0.1" // adresse IP de mon serveur

int to_server_socket = -1;

int main ( void )
{

char *server_name = SERVEURNAME;
struct sockaddr_in serverSockAddr;
struct hostent *serverHostEnt;
long hostAddr;
long status;
char buffer[512];
unsigned int nbytes = 100;
char *msg;

bzero(&serverSockAddr,sizeof(serverSockAddr));
hostAddr = inet_addr(SERVEURNAME);
if ( (long)hostAddr != (long)-1)
  bcopy(&hostAddr,&serverSockAddr.sin_addr,sizeof(hostAddr));
else
{
  serverHostEnt = gethostbyname(SERVEURNAME);
  if (serverHostEnt == NULL)
  {
    printf("gethost rate\n");
    exit(0);
  }
  bcopy(serverHostEnt->h_addr,&serverSockAddr.sin_addr,serverHostEnt->h_length);
}
serverSockAddr.sin_port = htons(30000);
serverSockAddr.sin_family = AF_INET;

/* creation de la socket */
if ( (to_server_socket = socket(AF_INET,SOCK_STREAM,0)) < 0)
{
  printf("creation socket client ratee\n");
  exit(0);
}
/* requete de connexion */
if(connect( to_server_socket,
            (struct sockaddr *)&serverSockAddr,
            sizeof(serverSockAddr)) < 0 )
{
  printf("demande de connection ratee\n");
  exit(0);
}

/* envoie de donne et reception */
msg = (char *) malloc (nbytes + 1);
printf("Entrer le message à envoyer\n");
getline (&msg, &nbytes, stdin);
write(to_server_socket,msg,strlen(msg));
read(to_server_socket,buffer,512);
printf("%s\n",buffer);
/* fermeture de la connection */
shutdown(to_server_socket,2);
close(to_server_socket);

return 0;
}
