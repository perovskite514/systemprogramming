#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/epoll.h>  

int main(int argc, char* argv[]){

    if(argc != 2) {
        perror("argument error.\n");
        exit(1);
    }

    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in svaddr; 
    struct sockaddr_in claddr; 
    unsigned short port = atoi(argv[1]);
    unsigned int clLen;
    char echoBuffer[2048];
    int rsize;

    if(sock < 0){
        perror("sock error.\n");
        close(sock);
        exit(1);
    }
    
    if (port == 0) {
        perror("port number error.\n");
        close(sock);
        exit(1);
    }

    memset(&svaddr, 0, sizeof(svaddr));
    svaddr.sin_family      = PF_INET;
    svaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    svaddr.sin_port        = htons(port);

    if(bind(sock, (struct sockaddr *) &svaddr, sizeof(svaddr)) < 0){
        perror("bind error.\n");
        close(sock);
        exit(1);
    }

    if(listen(sock, 10) < 0){
        perror("listen error.\n");
        close(sock);
        exit(1);
    }

    int epfd;
    if((epfd = epoll_create1(0)) < 0){
        perror("epoll_create error.\n");
        close(sock);
        exit(1);
    }
    struct epoll_event ev;
    memset(&ev, 0, sizeof ev);
    ev.events = EPOLLIN;
    ev.data.fd = sock;
    if((epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev)) < 0){
        perror("epoll_ctl error.\n");
        close(sock);
        exit(1);
    }
    struct epoll_event events[10];
    while(1){
        int fd_count = epoll_wait(epfd, events, 10, -1);

        for(int i = 0; i < fd_count; i++){
            if(events[i].data.fd == sock){
                int cofd = accept(sock, (struct sockaddr *) &claddr, &clLen);
                if(cofd < 0){
                    perror("accept error.\n");
                    close(sock);
                    exit(1);
                }
                //setnonblocking(cofd);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = cofd;
                if(epoll_ctl(epfd, EPOLL_CTL_ADD, cofd, &ev) == -1){
                    perror("epoll_ctl error.\n");
                    exit(1);
                }
            }
            else if(events[i].events & EPOLLIN){

                int cofd = events[i].data.fd;
                memset(echoBuffer, 0, sizeof(echoBuffer));
                clLen = sizeof(claddr);
                rsize = recv(cofd, echoBuffer, sizeof(echoBuffer), 0);
                if(rsize < 0){
                    perror("recv error\n");
                    close(cofd);
                    close(sock);
                    exit(1);
                }

                else if(rsize == 0){
                    epoll_ctl(epfd, EPOLL_CTL_DEL, cofd, &ev);
                    close(cofd);
                    break;
                }
                if(send(cofd, echoBuffer, sizeof(echoBuffer), 0) < 0){
                    perror("send error.\n");
                    close(cofd);
                    close(sock);
                    exit(1);
                }

            }
        }
    }
        
    close(sock);
        
    return 0;
}



