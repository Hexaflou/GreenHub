#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>
#include <assert.h>
#include "comIncludes.h"
#include "comSendTask.h"

#define QUEUE_NAME  "/GH_comSendQ"

/***************************PRIVATE DECLARATION***********************/
static void * comSendTask(void * attr);
static int dataSend(char * msg);
static mqd_t smq = -1;
static mqd_t rmq = -1;
static pthread_t comSendThread;
static SOCKET sock;

/************************PUBLIC***************************************/

/* Initialisation de la tache, retourne un pointeur sur la boite au lettre */
mqd_t comSendTaskInit(int socket) {
    struct mq_attr attr;
    sock = (SOCKET) socket;
    /* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MQ_SIZE;
    attr.mq_curmsgs = 0;

    /* create the message queue */
    smq = mq_open(QUEUE_NAME, O_WRONLY | O_CREAT, 0644, &attr);
    assert((mqd_t) - 1 != smq);

    /* lancement de la tache */
    pthread_create(&comSendThread, NULL, &comSendTask, NULL);

    return smq;
}

/* Destruction de la tache */
int comSendTaskClose() {
    /* close task */
    int ret = pthread_cancel(comSendThread);
    if (rmq != -1)
        assert((mqd_t) - 1 != mq_close(rmq));
    /* cleanup */
    if (smq != -1)
        assert((mqd_t) - 1 != mq_close(smq));
    assert((mqd_t) - 1 != mq_unlink(QUEUE_NAME));
    return ret;
}

/************************PRIVATE***************************************/
static void * comSendTask(void * attr) {
    char buffer[MAX_MQ_SIZE + 1];
    ssize_t bytes_read;

    /* create the message queue */
    rmq = mq_open(QUEUE_NAME, O_RDONLY);
    assert((mqd_t) - 1 != rmq);

    while (1) {
        /* receive the message */
        bytes_read = mq_receive(rmq, buffer, MAX_MQ_SIZE, NULL);
        assert(bytes_read >= 0);

        buffer[bytes_read] = '\0';
        dataSend(buffer);
    }

    return (void *) NULL;
}

static int dataSend(char * msg) {
    if (send(sock, msg, strlen(msg), 0) < 0) {
        perror("send()");
        return SOCKET_ERROR;
    } else {
        printf("send to server : \n%s \n\n", msg);
    }
    return 0;
}
