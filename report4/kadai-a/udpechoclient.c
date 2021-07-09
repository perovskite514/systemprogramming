#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 

int main(int argc, char* argv[]){

    if(argc != 3){
        perror("argument error.\n");
        exit(1);
    }

    int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    struct sockaddr_in svaddr; 
    struct sockaddr_in claddr; 
    unsigned short port = atoi(argv[2]);
    unsigned int clLen; 
    int rsize;

    if(sock < 0){
        perror("sock error.\n");
        exit(1);
    }
    
    if(port <= 0 || 65535 < port){
        perror("port number error.\n");
        close(sock);
        exit(1);
    }

    memset(&claddr, 0, sizeof(claddr));
    svaddr.sin_family      =  PF_INET;
    svaddr.sin_addr.s_addr = inet_addr(argv[1]);
    svaddr.sin_port        = htons(port);

    int rco;
    int wco;
    char sendbuff[1024];
    char recvbuff[1024];

    while((rco = read(1, sendbuff, 1024)) != 0){

        memset(recvbuff, 0, sizeof(recvbuff));
        if(rco == -1){
            perror("read error.\n");
            close(sock);
            exit(1);
        }

        if(sendto(sock, sendbuff, sizeof(sendbuff), 0, (struct sockaddr *)&svaddr, sizeof(svaddr)) < 0){
            perror("sendto error.\n");
            close(sock);
            exit(1);
        }

        rsize = recvfrom(sock, recvbuff, sizeof(recvbuff), 0, (struct sockaddr *)&claddr, &clLen);
        if(rsize < 0){
            perror("recvfrom error\n");
            close(sock);
            exit(1);
        }

        if((wco = write(1, recvbuff, rco))!= rco){
            perror("write error.\n");
            close(sock);
            exit(1);
        }
        memset(sendbuff, 0, sizeof(sendbuff));
    }

    close(sock);
                
    return 0;
}



