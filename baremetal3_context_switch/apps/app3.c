unsigned long long text_section_top = 0x106000000;

void app3() {
  while (1) {
    char *str = "Hello from app3\r\n";

    // TODO: Say hello from here by issuing `puts` syscall
    unsigned long long syscall_ret;
    asm volatile (
    "movq %[id], %%rax\n"
    "movq %[arg1], %%rdi\n"
    "int $0x80\n"
    "movq %%rax, %[ret]\n"
    : [ret]"=r"(syscall_ret)
    : [id]"g"((unsigned long long)0x1),
    [arg1]"m"((unsigned long long)str)
    ); 

    volatile int i = 100000000;
    while (i--);
  }

  asm volatile ("jmp *%0" :: "m"(text_section_top));
}
