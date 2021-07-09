#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

volatile int count = 0;
void handler(int sig){
	count++;
  	return;
}

int main(){

	struct sigaction si;
	memset(&si, 0, sizeof(si));
	si.sa_handler = handler;
	si.sa_flags = SA_SIGINFO;

	if(sigaction(SIGINT, &si, NULL) < 0){
		perror("signal error\n");
		exit(1);
	}

	while (count < 10){}
	puts("exit");
	return 0;
}
