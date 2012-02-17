#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>
#include <assert.h>
#include <sys/socket.h>
#include "ComSndReceptorTask.h"
#include "ComponentInterface.h"

#define QUEUE_NAME  "/GH_comSendReceptorQ"

/***************************PRIVATE DECLARATION***********************/
static void * comSndReceptorTask(void * attr);
static int dataSend(char * msg);
static mqd_t smq;
static mqd_t rmq;
static pthread_t comSndReceptorThread;
static SOCKET sock;

/************************PUBLIC***************************************/
/* Initialisation de la tache, retourne un pointeur sur la boite au lettre */
mqd_t comSndReceptorTaskInit(int socket)
{
	struct mq_attr attr;
	sock = (SOCKET) socket;
	/* initialize the queue attributes */
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = MAX_MQ_SIZE;
	attr.mq_curmsgs = 0;

	/* create the message queue */
	smq = mq_open(QUEUE_NAME, O_WRONLY | O_CREAT, 0644, &attr);
	assert((mqd_t)-1 != smq);
	
	/* lancement de la tache */
	pthread_create(&comSndReceptorThread, NULL, &comSndReceptorTask,NULL);
	
	return smq;
}

/* Destruction de la tache */
int comSndReceptorTaskClose()
{
	/* close task */
	int ret = pthread_cancel(comSndReceptorThread);
	assert((mqd_t)-1 != mq_close(rmq));
	/* cleanup */
	assert((mqd_t)-1 != mq_close(smq));
	assert((mqd_t)-1 != mq_unlink(QUEUE_NAME));
	return ret;
}

/************************PRIVATE***************************************/
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

static int dataSend(char * msg)
{
	if(send(sock, msg, strlen(msg), 0) < 0)
	{
		perror("[ComSndReceptorTask] Error in message post.");
		return SOCKET_ERROR;
	}
	else
	{
		printf("[ComSndReceptorTask] Send to EnOcean receptor : \n%s \n\n",msg);
	}
	return 0;
}
