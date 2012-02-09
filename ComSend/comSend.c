#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <mqueue.h>
#include <fcntl.h>
#include "comSend.h"



pthread_t tid;



void thread_send_func()
{
	mqd_t mq;
	    char buffer[MAX_SIZE];

	    /* open the mail queue */
	    mq = mq_open(QUEUE_NAME, O_WRONLY);
	    CHECK((mqd_t)-1 != mq);


	    printf("Send to server (enter \"exit\" to stop it):\n");

	    do {
	        printf("> ");
	        fflush(stdout);

	        memset(buffer, 0, MAX_SIZE);
	        fgets(buffer, MAX_SIZE, stdin);

	        /* send the message */
	        CHECK(0 <= mq_send(mq, buffer, MAX_SIZE, 0));

	    } while (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)));

	    /* cleanup */
	    CHECK((mqd_t)-1 != mq_close(mq));

	    return 0;

}

mqd_t initComSend(int socket)
{
	//pthread_create(&thread.tid, NULL, &thread_send_func,NULL);
}

void closeComSend()
{
	//mq_close(mq);
	//must_stop=0;
}

	int main()
	{
	    mqd_t mq;
	    struct mq_attr attr;
	    char buffer[MAX_SIZE + 1];
	    int must_stop = 0;

	    /* initialize the queue attributes */
	    attr.mq_flags = 0;
	    attr.mq_maxmsg = 10;
	    attr.mq_msgsize = MAX_SIZE;
	    attr.mq_curmsgs = 0;

	    /* create the message queue */
	    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
	    CHECK((mqd_t)-1 != mq);

	    pthread_create(&tid, NULL, &thread_send_func,NULL);

	    do {
	        ssize_t bytes_read;

	        /* receive the message */
	        bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
	        CHECK(bytes_read >= 0);

	        buffer[bytes_read] = '\0';
	        if (! strncmp(buffer, MSG_STOP, strlen(MSG_STOP)))
	        {
	            must_stop = 1;
	        }
	        else
	        {
	            printf("Received: %s\n", buffer);
	        }
	    } while (!must_stop);

	    /* cleanup */
	    CHECK((mqd_t)-1 != mq_close(mq));
	    CHECK((mqd_t)-1 != mq_unlink(QUEUE_NAME));

	    return 0;
	}






