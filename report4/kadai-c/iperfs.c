#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 

int main(int argc, char* argv[]){

    if(argc != 2) {
        perror("argument error\n");
        exit(1);
    }

    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in svaddr; 
    struct sockaddr_in claddr; 
    unsigned short port = atoi(argv[1]);
    unsigned int clLen; 
    char echoBuffer[10000];

    if(sock < 0){
        perror("sock error\n");
        close(sock);
        exit(1);
    }
    
    if(port == 0){
        perror("port number error\n");
        close(sock);
        exit(1);
    }

    memset(&svaddr, 0, sizeof(svaddr));
    svaddr.sin_family      = PF_INET;
    svaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    svaddr.sin_port        = htons(port);

    if(bind(sock, (struct sockaddr *) &svaddr, sizeof(svaddr)) < 0){
        perror("bind error\n");
        close(sock);
        exit(1);
    }

    if(listen(sock, 10) < 0){
        perror("listen error\n");
        close(sock);
        exit(1);
    }

    int cofd = accept(sock, (struct sockaddr *) &claddr, &clLen);
    if(cofd < 0){
        perror("accept error\n");
        close(sock);
        exit(1);
    }
    
    while(1){

        int rsize = read(cofd, echoBuffer, 1000);
        if(rsize < 0){
            perror("recv error\n");
            close(sock);
            close(cofd);
            exit(1);
        }
        
        int wsize = write(cofd, echoBuffer, 1000);
        if(wsize < 0){
            perror("send error\n");
            close(sock);
            close(cofd);
            exit(1);
        }
        
    }

    close(sock);   
    return 0;
}



