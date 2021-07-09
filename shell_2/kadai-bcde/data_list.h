#include <signal.h>

typedef struct wait_process
{
    int wait_process[100];
    int change[100];
    int count;

} wait_process;

typedef struct suspend_process
{
    int suspend_process[100];
    int change[100];
    int count;

} suspend_process;

wait_process bg(wait_process, int flag, int pid);
wait_process fg(wait_process, int flag, int pid);
void handler_suspend(int sig);
void myexec(process *, struct sigaction, char **);
void mypipe(process *, struct sigaction, char **);
wait_process myexec2(process *, wait_process, struct sigaction, char **);
wait_process mypipe2(process *, wait_process, struct sigaction, char **);
