#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>
#include <assert.h>
#include <sys/socket.h>


#include "../gCommunication/comIncludes.h"
#include "SimulationReceptorEnOcean.h"
#include "ComponentInterface.h"

/***************************PRIVATE DECLARATION***********************/
static void * comSimulationReceptorTask(void * attr);
static int dataSend(char * msg);
static mqd_t smq;
static mqd_t rmq;
static pthread_t comReceptorThread;
static SOCKET sock;
static SOCKET newSock;
static struct sockaddr_in socketAddr; /* Socket address */

#define QUEUE_NAME_RECEPTOR "/GH_comReceptorQ"

/************************PUBLIC***************************************/
/* Initialisation de la tache, retourne un pointeur sur la boite au lettre */
mqd_t comSimulationReceptorTaskInit()
{
  struct mq_attr attr;
  /* initialize the queue attributes */
  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = MAX_MQ_SIZE;
  attr.mq_curmsgs = 0;
  
  /* create the message queue */
  smq = mq_open(QUEUE_NAME_RECEPTOR, O_WRONLY | O_CREAT, 0644, &attr);
  assert((mqd_t)-1 != smq);
  
  /* lancement de la tache */
  pthread_create(&comReceptorThread, NULL, &comSimulationReceptorTask,NULL);
  
  return smq;
}

/* Destruction de la tache */
int comSimulationReceptorTaskClose()
{
	/* close task */
	int ret = pthread_cancel(comReceptorThread);
	assert((mqd_t)-1 != mq_close(rmq));
	/* cleanup */
	assert((mqd_t)-1 != mq_close(smq));
	assert((mqd_t)-1 != mq_unlink(QUEUE_NAME_RECEPTOR));
	close(sock);
	close(newSock);
	return ret;
}

/************************PRIVATE***************************************/
static void * comSimulationReceptorTask(void * attr)
{
	char buffer[MAX_MQ_SIZE + 1];
	ssize_t bytes_read;
	char *ip;				/* IP broadcast address */
	unsigned short port;	/* Server port */
	struct sockaddr_in sockClient;	/* structure destinée à recueillir les renseignements sur l'appelant */
	int sockClientLen;

	sockClientLen = sizeof(sockClient);
	ip = "127.0.0.1";		/* First arg:  broadcast IP address */ 
	port = 8888;			/* Second arg:  broadcast port */	

  /* Construct local address structure */
  memset(&socketAddr, 0, sizeof(socketAddr));		/* Zero out structure */
  socketAddr.sin_family = AF_INET;				/* Internet address family */
  socketAddr.sin_addr.s_addr = inet_addr(ip);		/* IP address */
  socketAddr.sin_port = htons(port);		/* Broadcast port */

  /* create the message queue */
  rmq = mq_open(QUEUE_NAME_RECEPTOR, O_RDONLY);
  assert((mqd_t)-1 != rmq);
  
  /* Create socket for sending/receiving datagrams */
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
	  perror("[SimulationReceptorEnOcean] Erreur dans la création du socket ");
	  comSimulationReceptorTaskClose();
	  exit(0);
  }

  /* Liaison du socket  */
	if (bind(sock,(struct sockaddr*)&socketAddr, sizeof(socketAddr) )  == SOCKET_ERROR)
	{
		perror("[SimulationReceptorEnOcean] Erreur dans la liaison du socket ");
		comSimulationReceptorTaskClose();
		exit(0);
	}

	/* Mise en écoute du socket */
	if (listen(sock,10) == SOCKET_ERROR) {
		perror("[SimulationReceptorEnOcean] Erreur dans l'écoute du socket ");
		comSimulationReceptorTaskClose();
		exit(0);
	}

	/* Acceptation une requête de connexion */
	if ((newSock = accept (sock, (struct sockaddr *) &sockClient, (socklen_t*) &sockClientLen)) == ERROR)
	{	
		perror("[SimulationReceptorEnOcean] Erreur dans l'acceptation de la requête de connexion.");
		comSimulationReceptorTaskClose();
		exit(0);
	}
  
	while(1)
	{
		/* receive the message */
		bytes_read = mq_receive(rmq, buffer, MAX_MQ_SIZE, NULL);
		assert(bytes_read >= 0);
		   
		buffer[bytes_read] = '\0';
		dataSend(buffer);
	}


	return (void *) NULL;
}

static int dataSend(char * msg)
{
  int msglen = strlen(msg);

  /* Broadcast msg in datagram to clients */
  if (send(newSock, msg, msglen, 0) == SOCKET_ERROR)
  {
    perror("[ComReceptorTask] Erreur dans l'envoi du  message ");
    return SOCKET_ERROR;
  }else
  {
	#if DEBUG > 0
		printf("[ComReceptorTask] Sent  : \n%s \n\n",msg);
	#endif
  }
  return 0;
}
