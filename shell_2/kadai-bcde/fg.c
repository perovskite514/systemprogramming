#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "parser/parse.h"
#include "data_list.h"

wait_process fg(wait_process wait, int flag, int pid){

    if(wait.count == 0) return wait;

    if(flag == 0){

        pid = wait.wait_process[0];
        for(int i=0; i<wait.count; i++){
            wait.wait_process[i] = wait.wait_process[i+1];
        }
        wait.count--;
    }
    else{
        int f = -1;
        for(int i=0; i<wait.count; i++){
            if(wait.wait_process[i] == pid){
                f = i;
                break;
            }
        }
        if(f == -1){
            puts("not found");
            return wait;
        }
        else{
            for(int i=f; i<wait.count; i++){
                wait.wait_process[i] = wait.wait_process[i+1];
            }
            wait.count--;
        }
    }

    if(kill(pid, 18) < 0){
        perror("kill error\n");
        exit(1);
    }
        
    if(tcsetpgrp(STDIN_FILENO, getpgid(pid)) < 0){ 
        perror("tcsetpgrp error\n");
        exit(1);
    }
    
    if(waitpid(pid, NULL, 0) < 0){
        perror("wait error\n");
        exit(1);
    }

    if(tcsetpgrp(STDIN_FILENO, getpgrp()) < 0){ 
        perror("tcsetpgrp error\n");
        exit(1);
    }

    return wait;
}