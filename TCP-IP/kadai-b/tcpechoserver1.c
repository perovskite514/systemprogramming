#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <pthread.h>

void *server(void *arg){

    int fd = *((int *)arg);
    struct sockaddr_in claddr; 
    unsigned int clLen; 
    char echoBuffer[2048];
    int clsock;

    if((clsock = accept(fd, (struct sockaddr *) &claddr, &clLen)) < 0){
            perror("accept error.\n");
            close(fd);
            exit(1);
    }

    while(1){
        int rsize = read(clsock, echoBuffer, sizeof(echoBuffer));
        printf("%d\n", rsize);
        if(rsize < 0){
            perror("recv error\n");
            close(fd);
            close(clsock);
            exit(1);
        }
        //for(int i=0; i<10; i++){
           // printf("%c", echoBuffer[i]);
        //}
        int wsize = write(clsock, echoBuffer, sizeof(echoBuffer));
        if(wsize < 0){
            perror("send error.\n");
            close(fd);
            close(clsock);
            exit(1);
        }
        //bzero(echoBuffer, 2048);
    }

    close(clsock);
    close(fd);
    return 0;
}

int main(int argc, char* argv[]){

    if(argc != 2){
        perror("argument error.\n");
        exit(1);
    }

    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in svaddr;  
    unsigned short port = atoi(argv[1]);
    
    if(sock < 0){
        perror("sock error.\n");
        exit(1);
    }
    
    if(port == 0){
        perror("port number error.\n");
        close(sock);
        exit(1);
    }

    memset(&svaddr, 0, sizeof(svaddr));
    svaddr.sin_family      = AF_INET;
    svaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    svaddr.sin_port        = htons(port);

    if(bind(sock, (struct sockaddr *) &svaddr, sizeof(svaddr)) < 0){
        perror("bind error.\n");
        close(sock);
        exit(1);
    }

    if(listen(sock, 10) < 0){
        perror("listen error.\n");
        exit(1);
    }

    pthread_t thread[10];

    while(1){

        for(int i = 0; i < 5; i++){
            if(pthread_create(&thread[i], NULL, server, &sock) != 0){
                perror("thread_create error.\n");
                close(sock);
                exit(1);
            }
        }
        
        for(int i = 0; i < 5; i++){
            if(pthread_join(thread[i], NULL) != 0){
                perror("pthread_join error.\n");
                close(sock);
                exit(1);
            }
        }
    }

    close(sock);        
    return 0;
}



