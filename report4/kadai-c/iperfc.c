#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <time.h>

int main(int argc, char* argv[]){

    if(argc != 3){
        perror("argument error.\n");
        exit(1);
    }

    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in claddr; 
    unsigned short port = atoi(argv[2]);

    if(sock < 0){
        perror("sock error.\n");
        close(sock);
        exit(1);
    }
    
    if(port <= 0 || 65535 < port){
        perror("port number error.\n");
        close(sock);
        exit(1);
    }

    memset(&claddr, 0, sizeof(claddr));
    claddr.sin_family      = AF_INET;
    claddr.sin_addr.s_addr = inet_addr(argv[1]);
    claddr.sin_port        = htons(port);

    if(connect(sock, (struct sockaddr*) &claddr, sizeof(claddr)) < 0){
        perror("connect error.\n");
        close(sock);
        exit(1);
    }

    char data[10000];
    struct timespec t1;
    struct timespec t2;
    struct timespec t3;
    struct timespec t4;
    data[0] = 'a';
    clock_gettime(CLOCK_REALTIME, &t1);
    if(write(sock, data, 1) < 0){
            perror("sendto error.\n");
            close(sock);
            exit(1);
    }

    if(read(sock, data, 1) < 0){
        perror("write error.\n");
        close(sock);
        exit(1);
    }
    clock_gettime(CLOCK_REALTIME, &t2);

    int msize = 1000;
    for(int i = 0; i < msize; i++){
        data[i] = 'a';
    }

    char buff[10000];

    clock_gettime(CLOCK_REALTIME, &t3);
    for(int i=0; i < 200000; i++){

        if(write(sock, data, 1000) < 0){
            perror("write error.\n");
            close(sock);
            exit(1);
        }

        if(read(sock, buff, 1000) < 0){
            perror("read error.\n");
            close(sock);
            exit(1);
        }

    }
    clock_gettime(CLOCK_REALTIME, &t4);

    int sp = 200 * 8 / (t4.tv_sec - t3.tv_sec - t2.tv_sec + t1.tv_sec);
    printf("%d %ld %d\n", 200000000, t4.tv_sec - t3.tv_sec - t2.tv_sec + t1.tv_sec, sp);
    
	return 0;
}