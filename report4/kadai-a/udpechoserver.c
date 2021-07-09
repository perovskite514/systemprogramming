#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 

int main(int argc, char* argv[]){

    if(argc != 2){
        perror("argument error.\n");
        exit(1);
    }

    int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    struct sockaddr_in svaddr; 
    struct sockaddr_in claddr; 
    unsigned short port = atoi(argv[1]);
    unsigned int clLen; 
    char echoBuffer[1024];
    int rsize;

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
    svaddr.sin_family      = PF_INET;
    svaddr.sin_addr.s_addr = INADDR_ANY;
    svaddr.sin_port        = htons(port);

    if(bind(sock, (struct sockaddr *) &svaddr, sizeof(svaddr)) < 0){
        perror("bind error.\n");
        close(sock);
        exit(1);
    }

    while(1){
        memset(echoBuffer, 0, sizeof(echoBuffer));
        clLen = sizeof(claddr);

        rsize = recvfrom(sock, echoBuffer, sizeof(echoBuffer), 0, (struct sockaddr *)&claddr, &clLen);
        if(rsize < 0){
            perror("recvfrom error\n");
            close(sock);
            exit(1);
        }
        else if(rsize == 0) break;
        if(sendto(sock, echoBuffer, sizeof(echoBuffer), 0, (struct sockaddr *)&claddr, sizeof(claddr)) < 0){
            perror("sendto error.\n");
            close(sock);
            exit(1);
        }
    }

    close(sock);
        
    return 0;
}



