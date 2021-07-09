#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "parser/parse.h"
#include "data_list.h"

extern suspend_process suspend;

void myexec(process* pr, struct sigaction si, char *envp[]){
    
    pid_t pid = fork();

    if(pid < 0){
        perror("fork error\n");
        exit(1);
    }
    else if(pid == 0){

        // child process
	    si.sa_handler = SIG_DFL;
        if(sigaction(SIGINT, &si, NULL) < 0){
            perror("sigaction1 error\n");
            exit(1);
        }
        
        if(sigaction(SIGTSTP, &si, NULL) < 0){
            perror("sigaction2 error\n");
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
        perror("execve error\n");
        exit(1);
    }
    else{
        // parent process
        if(setpgid(pid, pid) < 0){
            perror("setpgid error\n");
            exit(1);
        }
        
        if(tcsetpgrp(STDIN_FILENO, pid) < 0){ 
			perror("tcsetpgrp error\n");
			exit(1);
		}
        
        int status;
        if(waitpid(pid, &status, WUNTRACED) < 0){
            perror("wait error\n");
            exit(1);
        }
        if(WIFSTOPPED(status)){
            suspend.suspend_process[suspend.count] = pid;
            suspend.count++;
        }
        if(tcsetpgrp(STDIN_FILENO, getpgrp()) < 0){ 
			perror("tcsetpgrp error\n");
			exit(1);
		}
    }

    return;
}

