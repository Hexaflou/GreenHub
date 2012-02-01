#include "ComponentInterface.h"
#include "Test.h"
#include <stdio.h>
//#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>


int main(){
	//mqd_t msgQueueID;
	//struct mq_attr msgQueueAttributes;
	//msgQueueID = mq_open("/msgQueue", O_CREAT);

	//printf("Création boîte aux lettres \n");

	printf("Commencement fonction \n");
	initializeSensorList();
	ComponentInterface();
	return 0;
}
