/*
 * comSend.h
 *
 *  Created on: 8 févr. 2012
 *      Author: root
 */

#ifndef COMSEND_H_
#define COMSEND_H_



mqd_t initComSend(int socket);
void closeComSend();

#define QUEUE_NAME  "/test_queue"
#define MAX_SIZE    1024
#define MSG_STOP    "exit"

#define CHECK(x) \
    do { \
        if (!(x)) { \
            fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
            perror(#x); \
            exit(-1); \
        } \
    } while (0) \



#endif /* COMSEND_H_ */
