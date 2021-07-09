#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[]){

    if(argc < 2){
        perror("argv error\n");
        exit(1);
    }

    pid_t pid = fork();
    if(pid < 0){
        perror("fork error\n");
        exit(1);
    }
    else if(pid == 0){
        // child process
        execve(argv[1], argv + 1, envp);
        perror("execve error\n");
        exit(1);
    }
    else{
        // parent process
        waitpid(pid, NULL, 0);
    }

    return 0;
}

