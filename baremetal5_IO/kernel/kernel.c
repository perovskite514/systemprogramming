#include "hardware.h"
#include "segmentation.h"
#include "sched.h"
#include "interrupt.h"
#include "memory.h"
#include "util.h"
#include "pm_timer.h"
#include "lapic_timer.h"
#include "virtio.h"
#pragma GCC diagnostic ignored "-Wunused-parameter" // For l6 (kernel_param_dummy)

void start(void *SystemTable __attribute__ ((unused)), struct HardwareInfo *_hardware_info, unsigned long long kernel_param_dummy) {
  // From here - Put this part at the top of start() function
  // Do not use _hardware_info directry since this data is located in UEFI-app space
  hardware_info = *_hardware_info;
  init_segmentation();
  init_virtual_memory();
  init_frame_buffer(&hardware_info.fb);
  init_intr();
  init_acpi_pm_timer(hardware_info.rsdp);
  init();
  while(1);
  // char *str = "perovskite\r\n";
  // unsigned long long syscall_ret;
  // asm volatile (
  // "movq %[id], %%rax\n"
  // "movq %[arg1], %%rdi\n"
  // "int $0x80\n"
  // : [ret]"=r"(syscall_ret)
  // : [id]"g"((unsigned long long)0x1),
  // [arg1]"m"((unsigned long long)str)
  // ); 
  // asm volatile (
  // "movq %[id], %%rax\n"
  // "movq %[arg1], %%rdi\n"
  // "int $0x80\n"
  // : [ret]"=r"(syscall_ret)
  // : [id]"g"((unsigned long long)0x1),
  // [arg1]"m"((unsigned long long)str)
  // );
  // unsigned long long int co = 0;
  // for(int i=0; i<1; i++){
  //   unsigned long long int time = timer();
  //   asm volatile (
  //   "movq %[id], %%rax\n"
  //   "movq %[arg1], %%rdi\n"
  //   "int $0x80\n"
  //   : [ret]"=r"(syscall_ret)
  //   : [id]"g"((unsigned long long)0x1),
  //   [arg1]"m"((unsigned long long)str)
  //   ); 
  //   unsigned long long int time2 = timer();
  //   unsigned long long int time3 = time2 - time;
  //   if(time > time2){
  //     time3 = (1 << 24) - time + time2;
  //   }
  //   co += time3;
  // }
  // co /= 100000;
  // puth(co, 8);
  // puts("\n");
  // co = 0;
  // for(int i=0; i<1; i++){
  //   unsigned long long int time = timer();
  //   puts("perovskite\n");
  //   unsigned long long int time2 = timer();
  //   unsigned long long int time3 = time2 - time;
  //   if(time > time2){
  //     time3 = (1 << 24) - time + time2;
  //   }
  //   co += time3;
  // }
  // co /= 100000;
  // puth(co, 8);
  // puts("\n");
  
  unsigned int a = measure_lapic_freq_khz();
  unsigned long long handler;
  asm volatile ("lea schedule(%%rip), %0" : "=r"(handler));
  lapic_periodic_exec(1000,  (void *) handler);
  init_tasks();
  while (1);

  //puth(kernel_param_dummy, 10);

  // To here - Put this part at the top of start() function

  // Delete me. I'm a sample code.
  


      // † AYAME †



  // To here - sample code

  // Do not delete it!
}
