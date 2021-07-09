#include "interrupt.h"
#include "lapic_timer.h"
#include "util.h"

struct InterruptDescriptor {
  unsigned short offset_lo;
  unsigned short segment;
  unsigned short attribute;
  unsigned short offset_mid;
  unsigned int offset_hi;
  unsigned int reserved;
} __attribute__((packed));

struct InterruptDescriptor IDT[256];

void lapic_intr_handler();

void syscall_handler();

static void load_idt_to_idtr() {
  // Set idtr

  // Here (8-B)
  unsigned char data[10];
  unsigned short table_time = sizeof(IDT) - 1;
  unsigned long long int base_address = (unsigned long long int)IDT;
  data[0] = table_time & 0xff;
  data[1] = (table_time >> 8) & 0xff;
  for(int i=2; i<10; i++){
    data[i] = (base_address >> ((i - 2)*8)) & 0xff;
  }
  asm volatile ("lidt %0" :: "m"(data));
  //
}

static void register_intr_handler(unsigned char index, unsigned long long 
offset, unsigned short segment, unsigned short attribute){ 

  IDT[index].offset_lo = (unsigned short)(offset & 0xffff);
  IDT[index].offset_mid = (unsigned short)((offset >> 16) & 0xffff);
  IDT[index].offset_hi = (unsigned int)((offset >> 32) & 0xffffffff);

  IDT[index].segment = segment;
  IDT[index].attribute = attribute;

  return;
}

void init_intr() {
  // Get segment register value
  unsigned short cs;
  asm volatile ("mov %%cs, %0" : "=r"(cs));

  void* lapic_intr_handler_addr;
  asm volatile ("lea lapic_intr_handler(%%rip), %0" : "=r"(lapic_intr_handler_addr));

  void* syscall_handler_addr;
  asm volatile ("lea syscall_handler(%%rip), %[handler]" : [handler]"=r"(syscall_handler_addr));

  // Register Local APIC handler
  //
  unsigned char index = 32;
  unsigned short attribute = 0x8e00;
  register_intr_handler(index, (unsigned long long)lapic_intr_handler_addr, cs, attribute);
  //

  // Register Sycall handler
  //
  // Here (9-A)
  //

  // Tell CPU the location of IDT
  load_idt_to_idtr();

  // Set IF bit in RFLAGS register
  asm volatile ("sti");
}
