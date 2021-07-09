#define _GNU_SOURCE
#include <sys/mman.h>
#include <sys/wait.h>
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>

int f(){

    printf("child thread exiting\n");
    return 0;
    
}

int main(){

    pid_t pid;
    void *start =(void *) 0x0000010000000000;
    size_t len = 0x0000000000200000;

    void *stack = mmap(start, len, PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE|MAP_FIXED|MAP_GROWSDOWN, 0, 0);
    if(stack == (void *)-1){
        perror("mmap error \n");
        exit(1);
    }
        
    pid = clone(&f, stack + len, SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, 0);
    if(pid == -1){
        perror("clone error\n");
        exit(1);
    }
            
    pid = waitpid(pid, 0, 0);
    if(pid == -1){
        perror("waitpid error\n");
        exit(1);
    }
        
    return 0;
}