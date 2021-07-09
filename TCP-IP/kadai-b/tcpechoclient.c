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

    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in claddr;
    unsigned short port = atoi(argv[2]);

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
    claddr.sin_family      = PF_INET;
    claddr.sin_addr.s_addr = inet_addr(argv[1]);
    claddr.sin_port        = htons(port);

    if(connect(sock, (struct sockaddr*) &claddr, sizeof(claddr)) < 0){
        perror("connect error.\n");
        close(sock);
        exit(1);
    }

    char buff[2048];
    int wco;
    int rco;

    while((rco = read(1, buff, 2048)) != 0){

        //bzero(buff, sizeof(buff));
        int rsize = 0;
        for(int i=0; i<2048; i++){
            if(buff[i] == '\n'){
                rsize = i + 1;
                break;
            }
        }
        //printf("%d\n", rsize);
        int send = write(sock, buff, 2048);
        if(send < 0){
            perror("sendto error.\n");
            close(sock);
            exit(1);
        }
        bzero(buff, sizeof(buff));
        //printf("%d\n", rsize);
        int recv = read(sock, buff, 2048);
        if(recv < 0){
            perror("recvfrom error\n");
            close(sock);
            exit(1);
        }
        //for(int i=0; i<10; i++){
            //printf("%c", buff[i]);
        //}
        if((wco = write(1, buff, rsize))!= rsize){
            perror("write error.\n");
            close(sock);
            exit(1);
        }
        bzero(buff, sizeof(buff));
    }

    close(sock);
                
    return 0;
}



