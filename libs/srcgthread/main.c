#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include "gThread.h"

#define RETURN_SUCCESS 0
#define RETURN_FAILURE 1

struct ctx_s ctx_ping;
struct ctx_s ctx_pong;
struct ctx_s ctx_paf;

void f_ping(void *arg);
void f_pong(void *arg);
void f_paf(void *arg);

int main(int argc, char *argv[])
{
	init_ctx(&ctx_ping, 16384, f_ping, NULL);
	init_ctx(&ctx_pong, 16384, f_pong, NULL);
	init_ctx(&ctx_paf, 16384, f_paf, NULL);
	switch_to_ctx(&ctx_ping);
	exit(EXIT_SUCCESS);
}

void f_ping(void *args)
{
	while(1) {
		printf("A\n") ;
		switch_to_ctx(&ctx_pong);
		printf("B\n") ;
		switch_to_ctx(&ctx_pong);
		printf("C\n") ;
		switch_to_ctx(&ctx_pong);
	}
}

void f_pong(void *args)
{
	while(1) {
		printf("1\n") ;
		switch_to_ctx(&ctx_paf);
		printf("2\n") ;
		switch_to_ctx(&ctx_paf);
	}
}

void f_paf(void *args)
{
	while(1) {
		printf("*\n") ;
		switch_to_ctx(&ctx_ping);
	}
}
