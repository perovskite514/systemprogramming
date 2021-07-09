#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "parser/parse.h"
#include "data_list.h"

extern suspend_process suspend;

void mypipe(process* pr, struct sigaction si, char *envp[]){
    
    pid_t pid1 = fork();
    int pfd[2] = {};
    if(pipe(pfd) < 0){
		perror("pipe error\n");
		exit(1);
	}

    if(pid1 < 0){
        perror("fork error\n");
        exit(1);
    }
    else if(pid1 == 0){
        close(pfd[0]);
        dup2(pfd[1], 1);
        si.sa_handler = SIG_DFL;
        if(sigaction(SIGINT, &si, NULL) < 0){
            perror("sigaction error\n");
            exit(1);
        }

        if(sigaction(SIGTSTP, &si, NULL) < 0){
            perror("sigaction error\n");
            exit(1);
        }

        int fd;
        int fd1;
        if(pr->input_redirection != NULL){
            fd = open(pr->input_redirection, O_RDONLY);
            dup2(fd, 0);
        }
        if(pr->output_redirection != NULL){
            fd1 = open(pr->output_redirection, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            dup2(fd1, 1);
        }
        execve(pr->program_name, pr->argument_list, envp);
        perror("execve1 error\n");
        exit(1);
    }
    else{
        if(setpgid(pid1, pid1) < 0){
            perror("setpgid error\n");
            exit(1);
        }
        if(tcsetpgrp(STDIN_FILENO, pid1) < 0){ 
			perror("tcsetpgrp error\n");
			exit(1);
		}
    }

    pid_t pid2 = fork();
    if(pid2 < 0){
        perror("fork error\n");
        exit(1);
    }
    else if(pid2 == 0){
        close(pfd[1]);
        dup2(pfd[0], 0);
        si.sa_handler = SIG_DFL;
        if(sigaction(SIGINT, &si, NULL) < 0){
            perror("sigaction error\n");
            exit(1);
        }

        if(sigaction(SIGSTOP, &si, NULL) < 0){
            perror("sigaction error\n");
            exit(1);
        }

        int fd;
        int fd1;
        if((pr->next)->input_redirection != NULL){
            fd = open((pr->next)->input_redirection, O_RDONLY);
            dup2(fd, 0);
        }
        if((pr->next)->output_redirection != NULL){
            fd1 = open((pr->next)->output_redirection, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            dup2(fd1, 1);
        }
        execve((pr->next)->program_name, (pr->next)->argument_list, envp);
        perror("execve error\n");
        exit(1);
    }
    else{
        if(setpgid(pid2, pid1) < 0){
            perror("setpgid error\n");
            exit(1);
        }
    }

    close(pfd[0]);
    close(pfd[1]);
    int status1;
    int status2;
    if(waitpid(pid1, &status1, WUNTRACED) < 0){
        perror("wait error\n");
        exit(1);
    }
    if(WIFSTOPPED(status1)){
        suspend.suspend_process[suspend.count] = pid1;
        suspend.count++;
    }
    if(waitpid(pid2, &status2, WUNTRACED) < 0){
        perror("wait error\n");
        exit(1);
    }
    if(WIFSTOPPED(status2)){
        suspend.suspend_process[suspend.count] = pid2;
        suspend.count++;
    }
    
    if(tcsetpgrp(STDIN_FILENO, getpgrp()) < 0){ 
		perror("tcsetpgrp error\n");
		exit(1);
	}
    return;
}

