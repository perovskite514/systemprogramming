#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "parser/parse.h"

void myexec(process* pr, char *envp[]){

    pid_t pid = fork();
    if(pid < 0){
        perror("fork\n");
        exit(1);
    }
    else if(pid == 0){
        // child process
        int fd;
        int fd1;
        if(pr->input_redirection != NULL){
            fd = open(pr->input_redirection, O_RDONLY);
            dup2(fd, 0);
        }
        if(pr->output_redirection != NULL){
            if(pr->output_option == TRUNC){
                fd1 = open(pr->output_redirection, O_WRONLY | O_CREAT | O_TRUNC, 0666);
                dup2(fd1, 1);
            }
            else{
                fd1 = open(pr->output_redirection, O_WRONLY | O_CREAT | O_APPEND, 0666);
                dup2(fd1, 1);
            }
        }

        execve(pr->program_name, pr->argument_list, envp);
        perror("execve error\n");
        exit(1);
    }
    else{
        // parent process
        waitpid(pid, NULL, 0);
    }

    return;
}

