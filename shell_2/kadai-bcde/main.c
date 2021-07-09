#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include "parser/parse.h"
#include "data_list.h"

suspend_process suspend;
struct sigaction si;
void print_job_list(job*);

int main(int argc, char *argv[], char *envp[]){

    //signalの設定
	memset(&si, 0, sizeof(si));
	si.sa_handler = SIG_IGN;
    si.sa_flags = 0;
    sigemptyset(&si.sa_mask);
    if(sigaction(SIGINT, &si, NULL) < 0){
        perror("sigaction error\n");
        exit(1);
    }
    if(sigaction(SIGTSTP, &si, NULL) < 0){
        perror("sigaction error\n");
        exit(1);
    }
    if(sigaction(SIGTTIN, &si, NULL) < 0){
        perror("sigaction error\n");
        exit(1);
    }
    if(sigaction(SIGTTOU, &si, NULL) < 0){
        perror("sigaction error\n");
        exit(1);
    }

    //バックグラウンドプロセスや一時停止中のプロセスのリストを作成
    char s[LINELEN];
    job *curr_job;
    process* pr; 
    wait_process wait;
    wait.count = 0;
    for(int i=0; i<100; i++){
        wait.wait_process[i] = 0;
        wait.change[i] = 0;
    }
    suspend.count = 0;
    for(int i=0; i<100; i++){
        suspend.suspend_process[i] = 0;
        suspend.change[i] = 0;
    }

    while(get_line(s, LINELEN)){

        if(!strcmp(s, "exit\n"))
            break;

        curr_job = parse_line(s);
        if(curr_job == NULL){
            free_job(curr_job);
            continue;
        }
        pr = curr_job->process_list;

        if(!strcmp(pr->argument_list[0], "bg")){
            if(pr ->argument_list[1] == NULL){
                wait = bg(wait, 0, -1);
            }
            else{
                wait = bg(wait, 1, atoi(pr ->argument_list[1]));
            }
        }
        else if(!strcmp(pr->argument_list[0], "fg")){
            if(pr ->argument_list[1] == NULL){
                wait = fg(wait, 0, -1);
            }
            else{
                wait = fg(wait, 1, atoi(pr ->argument_list[1]));
            }
        }
        else if(curr_job->mode == FOREGROUND){
            if(pr->next != NULL){
                mypipe(pr, si, envp);
            }
            else{
                myexec(pr, si, envp);
            }
        }
        else{
            if(pr->next != NULL){
                wait = mypipe2(pr, wait, si, envp);
            }
            else{
                wait = myexec2(pr, wait, si, envp);
            }
        }
        
        for(int i=0; i<wait.count; i++){
            if(waitpid(wait.wait_process[i], NULL, WNOHANG) < 0){
                perror("wait error\n");
                exit(1);
            }
            else if(waitpid(wait.wait_process[i], NULL, WNOHANG) != 0){
                wait.change[i] = -1;
            }
        }
        int tmp[wait.count];
        int inx = 0;
        for(int i=0; i<wait.count; i++){
            if(wait.change[i] == 0){
                tmp[inx] = wait.wait_process[i];
                inx++;
            }
            else wait.change[i] = 0;
        }

        for(int i=0; i<wait.count; i++){
            if(i < inx){
                wait.wait_process[i] = tmp[i];
            }
            else{
                wait.wait_process[i] = 0;
            }
        }

        wait.count = inx;
        free_job(curr_job);
    }

    return 0;
}
