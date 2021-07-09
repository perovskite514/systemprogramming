#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "util.h"
#include "lapic_timer.h"
#define APP0_START 0x104000000
#define APP1_START 0x105000000
#define APP2_START 0x106000000
#define APP_SIZE 1024 * 1024 * 16 // 16MiB
#define TASK_NUM 3

struct Task {
 unsigned long long sp;
};
struct Task tasks[TASK_NUM];
unsigned int current_task = 0;
void init_tasks(){
  tasks[0].sp = APP0_START + APP_SIZE;
  tasks[1].sp = APP1_START + APP_SIZE;
  tasks[2].sp = APP2_START + APP_SIZE;

  unsigned long long reg64;
  unsigned long long sp = tasks[1].sp;
  unsigned long long temp = 0;
  unsigned long long sp_hold = sp;
  unsigned long long hoge = APP1_START;
  asm volatile(
    "mov %%rsp, %0\n"
    "mov %1, %%rsp\n" : "=r"(reg64) : "m"(sp));
  asm volatile("mov %%ss, %0\n" : "=r"(temp));
  asm volatile("push %0\n" :: "m"(temp));
  asm volatile("push %0\n" :: "m"(sp_hold));
  asm volatile("pushfq\n");
  asm volatile("mov %%cs, %0\n" : "=r"(temp));  
  asm volatile("push %0\n" :: "m"(temp));
  asm volatile("push %0\n" :: "m"(hoge));
  asm volatile ("mov %0, %%rsp" :: "m"(reg64));

  tasks[1].sp = tasks[1].sp - 96;

  sp = tasks[2].sp;
  sp_hold = sp;
  hoge = APP2_START;
  asm volatile(
    "mov %%rsp, %0\n"
    "mov %1, %%rsp\n" : "=r"(reg64) : "m"(sp));
  asm volatile("mov %%ss, %0\n" : "=r"(temp));
  asm volatile("push %0\n" :: "m"(temp));
  asm volatile("push %0\n" :: "m"(sp_hold));
  asm volatile("pushfq\n");
  asm volatile("mov %%cs, %0\n" : "=r"(temp));  
  asm volatile("push %0\n" :: "m"(temp));
  asm volatile("push %0\n" :: "m"(hoge));
  asm volatile ("mov %0, %%rsp" :: "m"(reg64));

  tasks[2].sp = tasks[2].sp - 96;
  sp = tasks[0].sp;

  hoge = APP0_START;
  asm volatile ("mov %0, %%rsp" :: "m"(sp));
  asm volatile ("jmp *%0" :: "m"(hoge));
}


void schedule(unsigned long long sp) {

  tasks[current_task].sp = sp;
  current_task++;
  current_task %= 3;
  set_eoi();

  unsigned long long sp2 = tasks[current_task].sp;
  asm volatile ("mov %0, %%rsp" :: "m"(sp2));
  asm volatile(
    "pop %rbp\n"
    "pop %rsi\n"
    "pop %rdi\n"
    "pop %rdx\n"
    "pop %rcx\n"
    "pop %rbx\n"
    "pop %rax\n"
    "iretq"
  );
  puts("schedule() called\r\n");
}
