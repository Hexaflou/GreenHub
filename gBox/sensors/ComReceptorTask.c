/* Inclusions externes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>
#include <assert.h>
#include <sys/socket.h>

/* Inclusions internes */
#include "ComReceptorTask.h"
#include "ComponentInterface.h"
#include "../gCommunication/comIncludes.h"

/* Définition de constante */
#define QUEUE_NAME  "/GH_comSendReceptorQ"

/***************************PRIVEE***********************/
static void * comSndReceptorTask(void * attr);
static void *ListenEnOcean(void *ptr);
static int dataSend(char * msg);

static mqd_t smq = -1;
static mqd_t rmq = -1;
static pthread_t comSndReceptorThread;
static pthread_t comRcvReceptorThread;
static SOCKET sock = 0;
static char * message = NULL;

/************************PUBLIC***************************************/
/* Initialisation de la tache, retourne un pointeur sur la boite au lettre */
mqd_t comReceptorTaskInit()
{
	struct mq_attr attr;
	/* initialize the queue attributes */
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = MAX_MQ_SIZE;
	attr.mq_curmsgs = 0;

	/* create the message queue */
	smq = mq_open(QUEUE_NAME, O_WRONLY | O_CREAT, 0644, &attr);
	assert((mqd_t)-1 != smq);
	
	/* lancement de la tache */
	pthread_create(&comRcvReceptorThread, NULL, &ListenEnOcean,NULL);
	pthread_create(&comSndReceptorThread, NULL, &comSndReceptorTask,NULL);
	
	return smq;
}

/* Destruction de la tache */
int comReceptorTaskClose()
{
	/* close task */
	int ret, ret2;
	ret = pthread_cancel(comSndReceptorThread);
	ret2 = pthread_cancel(comRcvReceptorThread);

	/* cleanup */
	if (smq != -1)
		assert((mqd_t)-1 != mq_close(smq));
	if (rmq != -1)
		assert((mqd_t)-1 != mq_close(rmq));
	assert((mqd_t)-1 != mq_unlink(QUEUE_NAME));

	if (sock != 0)
		close(sock);
	if (message != NULL)
		free(message);
	return (ret||ret2);
}

/************************PRIVATE***************************************/
/*
 * Fonction permettant l'écoute de périphériques EnOcean.
 * Elle reçoit des messages du récepteur EnOcean, enlève
 * l'en-tête et appelle le traitement de celui-ci.
 */
void *ListenEnOcean(void *message2)
{	
	char buffer[5];
	long n;
	int tailleTrame;
	struct sockaddr_in serverAddr;
	socklen_t serverAddrLen = sizeof(serverAddr);
	
	/* Protocole Internet */
	serverAddr.sin_family = AF_INET;
	/*serverAddr.sin_addr.s_addr = inet_addr("134.214.105.28");*/
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(8888);

	/* Creation d un socket TCP */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
	{
		perror("[ListenEnOcean] Erreur dans la creation du socket TCP ");
		return (int*)ERROR;
	}

#if DEBUG > 0
	printf("[ListenEnOcean] Connection avec le serveur...\n");
#endif

	if (connect(sock, (struct sockaddr*) &serverAddr, serverAddrLen) == -1)
	{
		perror("[ListenEnOcean] Erreur dans la connection du socket ");
		return (int*)SOCKET_ERROR;
	}

#if DEBUG > 0
	printf("[ListenEnOcean] Connection avec le serveur OK\n");
#endif

	while (1)
	{
#if DEBUG > 0
		printf("[ListenEnOcean] Attente d un debut de message...\n");
#endif

		while (strcmp(buffer, "A55A") != 0)
		{
			if ((n = recv(sock, buffer, sizeof(buffer - 1), 0)) < 0)
			{
				perror("[ListenEnOcean] Erreur dans la reception du message ");
				break;
			}
			buffer[4] = '\0';
		}

#if DEBUG > 0
		printf("[ListenEnOcean] Message recu.\n");
#endif

		if ((n = recv(sock, buffer, 2, 0)) < 0)
		{
			perror("[ListenEnOcean] Erreur dans la reception des donnees ");
			break;
		}
		buffer[2] = '\0';

#if DEBUG > 0
		printf("[ListenEnOcean] Taille du buffer : %i \n", xtoi(buffer));
#endif

		/* Convertit un tableau de caractere en hexadecimal en nombre decimal */
		tailleTrame = xtoi(buffer);

		/* Un octet correspond a deux caracteres */
		message = (char*) malloc(tailleTrame * sizeof(char) * 2 + 1);

		/* Reception du message sans le header */
		if ((n = recv(sock, message, tailleTrame * 2, 0)) < 0)
		{
			perror("[ListenEnOcean] Data Reception Error ");
			break;
		}
		message[tailleTrame * 2] = '\0';
		ManageMessage(message);
		free(message);
		message = NULL;
	}
	return 0;
}

/*
 * Tâche scrutant la boîte aux lettres pour envoyer les messages au récepteur EnOcean.
 */
static void * comSndReceptorTask(void * attr)
{
	    char buffer[MAX_MQ_SIZE + 1];
		ssize_t bytes_read;
				
	    /* create the message queue */
	    rmq = mq_open(QUEUE_NAME, O_RDONLY);
	    assert((mqd_t)-1 != rmq);

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

/*
 * Envoi le message en paramètre au récepteur EnOcean.
 */
static int dataSend(char * msg)
{
	int sendResult;
	while (sock == 0){
		printf("[datasend] Socket non initialisé...\n Nouvelle tentative dans quelques secondes.\n");
		sleep(5);
	}
	sendResult = send(sock, msg, strlen(msg), 0);
	if(sendResult < 0)
	{
		perror("[ComReceptorTask] Error in message post.");
		return SOCKET_ERROR;
	}
	else
	{
		#if DEBUG > 0
			printf("[ComReceptorTask] Sent to EnOcean receptor : \n%s \nNb of bytes sent : %i\n\n",msg,sendResult);
		#endif
	}
	return 0;
}
