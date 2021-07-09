#include <string.h>
#include <stdio.h>
#include "parser/parse.h"

void print_job_list(job*);

int main(int argc, char *argv[], char *envp[]) {
    char s[LINELEN];
    job *curr_job;
    process* pr; 

    while(get_line(s, LINELEN)){
        if(!strcmp(s, "exit\n"))
            break;

        curr_job = parse_line(s);
        if(curr_job == NULL){
            free_job(curr_job);
            continue;
        }
        //print_job_list(curr_job);
        pr = curr_job->process_list;

        if(pr->next != NULL){
            mypipe(pr, envp);
        }
        else{
            myexec(pr, envp);
        }

        free_job(curr_job);
    }

    return 0;
}
