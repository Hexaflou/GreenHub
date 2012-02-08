#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>



#define MAX_CLIENT 4
#define DEBUG

int PORT = 5000;
char *IP = "192.168.0.12";

enum {
	U_INT_SIZE = sizeof(unsigned int)
};

int list[MAX_CLIENT];
fd_set fdset;
int highfd = MAX_CLIENT;

void nonblock(int sockfd)
{
	int opts;
	opts = fcntl(sockfd, F_GETFL);
	if(opts < 0)
	{
		perror("fcntl(F_GETFL)\n");
		exit(1);
	}
	opts = (opts | O_NONBLOCK);
	if(fcntl(sockfd, F_SETFL, opts) < 0)
	{
		perror("fcntl(F_SETFL)\n");
		exit(1);
	}
}


void cli_con()
{
	struct sockaddr_in srv;
	struct hostent *h_name;
	int clifd;
	int n;

	bzero(&srv, sizeof(srv));
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	inet_pton(AF_INET, IP, &srv.sin_addr);

	if( (clifd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("ERROR clifd\n");
		exit(1);
	}

	if(connect(clifd, (struct sockaddr *) &srv, sizeof(srv)) < 0)
	{
		printf("ERROR connect\n");
		exit(1);
	}

	nonblock(clifd);

	for(n = 0; (n < MAX_CLIENT) && (clifd != -1); n++)
	{
		if(list[n] == 0) {
#ifdef DEBUG
			printf("connected %d\n", clifd);
#endif
			list[n] = clifd;
			clifd = -1;
		}
	}

	if(clifd != -1) {
		printf("list FULL");
	}
}

void set_list() {
	int n;
	FD_ZERO(&fdset);

	for(n = 0; n < MAX_CLIENT; n++)
	{
		if(list[n] != 0) {
			FD_SET(list[n], &fdset);
			if(list[n] > highfd)
				highfd = list[n];
		}
	}
}

void recv_data(int num)
{
	int n;
	char buffer[1024];
	n = recv(list[num], buffer, 1023, 0);
	if(n > 0)
	{
#ifdef DEBUG
		//printf("%d bytes from %d\n", n, list[num]);
		printf("%s\r\n", buffer);
#endif
	}
	else if(n < 0)
	{
		if(errno != EAGAIN)
			printf("client %d disconnected\n", list[num]);
	}
	else if(n == 0)
	{
		printf("client %d disconnected\n", list[num]);
	}
}


void send_data(int num)
{
	int n;
	char buffer[1024];
	//sprintf(buffer, "TEST list[%d]=%d \r\n", num, list[num]);
	sprintf(buffer, "On y va Agent Jackson");
	if((n = send(list[num], buffer, strlen(buffer), 0)) > 0)
	{
#ifdef DEBUG
		//printf("%d bytes sent from %d\n", n, list[num]);
#endif
	}
}

void scan_clients()
{
#ifdef DEBUG
	printf("scan_clients\n");
#endif
	int num;

	for(num = 0; num < MAX_CLIENT; num++) {
		if(FD_ISSET(list[num], &fdset))
			recv_data(num);
	}

	for(num = 0; num < MAX_CLIENT; num++)
		send_data(num);
}

int main()
{
	int readsocks;
	int i=0;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 10;
	char buffer[1024];

	while(i < MAX_CLIENT)
	{
		cli_con();
		i++;
	}
	i=0;
	while(i < MAX_CLIENT)
	{
		printf("list[%d] = %d\n",i,list[i]);
		i++;
	}
	i = 0;
	while(1)
	{
		set_list();
		readsocks = select(highfd+1, &fdset, NULL, NULL, &tv);
		if(readsocks < 0) {
			perror("select\n");
			exit(1);
		}

		/*printf("else scan_clients\n");*/
		scan_clients();

			sprintf(buffer, "\r\n Fin de la vague Client \r\n");
			send(highfd, buffer, strlen(buffer), 0);
#ifdef DEBUG
		sleep(4);
#endif
	}
	return 0;
}
