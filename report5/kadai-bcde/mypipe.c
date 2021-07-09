#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "parser/parse.h"

void dopipe(process* pr, char *envp[], int *p, int count, int process_size){

    pid_t pid;
    int pfd[2] = {};

    if(count == process_size - 1){

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
    }
    else{

        if(pipe(pfd) < 0){
            perror("pipe error\n");
            exit(1);
        }
        pid = fork();
        if(pid < 0){
            perror("fork error\n");
            exit(1);
        }
        else if(pid == 0){
            close(pfd[1]);
            dup2(pfd[0], 0);
            close(pfd[0]);
            dopipe(pr->next, envp, p, count + 1, process_size);
        }
        else{
            close(pfd[0]);
            dup2(pfd[1], 1);
            close(pfd[1]);

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
    }
    
    return;
}

void mypipe(process* pr, char *envp[]){

    int count = 0;
    process * pr1 = pr;
    int process_size = 0;

    while(1){
        if(pr1 == NULL) break;
        process_size++;
        pr1 = pr1->next;
    }

    int *p;
    p = malloc(sizeof (int) * process_size);

    pid_t pid = fork();
    if(pid < 0){
        perror("fork error\n");
        exit(1);
    }
    else if(pid == 0){
        dopipe(pr, envp, p, count, process_size);
    }
    else{
        waitpid(pid, NULL, 0);
    }

    return;
}

