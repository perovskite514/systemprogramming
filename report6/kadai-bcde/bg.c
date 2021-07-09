#include <stdio.h>
#include <stdlib.h>
#include "parser/parse.h"
#include "data_list.h"

extern suspend_process suspend;

wait_process bg(wait_process wait, int flag, int pid){

    if(suspend.count == 0) return wait;

    if(flag == 0){

        if(kill(suspend.suspend_process[0], 18) < 0){
            perror("kill error\n");
            exit(1);
        }
        wait.wait_process[wait.count] = suspend.suspend_process[0];
        wait.count++;
        for(int i=0; i<suspend.count; i++){
            suspend.suspend_process[i] = suspend.suspend_process[i+1];
        }
        suspend.count--;
    }
    else{
        int f = -1;
        for(int i=0; i<suspend.count; i++){
            if(suspend.suspend_process[i] == pid){
                f = i;
                break;
            }
        }
        if(f == -1){
            puts("not found");
            return wait;
        }
        else{
            if(kill(suspend.suspend_process[f], 18) < 0){
                perror("kill error\n");
                exit(1);
            }
            wait.wait_process[wait.count] = suspend.suspend_process[f];
            wait.count++;
            for(int i=f; i<suspend.count; i++){
                suspend.suspend_process[i] = suspend.suspend_process[i+1];
            }
            suspend.count--;
        }
    }

    return wait;
}