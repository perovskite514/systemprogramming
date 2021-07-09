#include "memory.h"
#include "sched.h"

struct Entry {
  unsigned int present : 1;
  unsigned int writable : 1;
  unsigned int user_accessible : 1;
  unsigned int write_through_caching : 1;
  unsigned int disable_cache : 1;
  unsigned int accessed : 1;
  unsigned int dirty : 1;
  unsigned int huge_page : 1;
  unsigned int global : 1;
  unsigned int available1 : 3;
  unsigned long long physical_address : 40;
  unsigned int available2 : 11;
  unsigned int no_execute : 1;
} __attribute__((packed));

unsigned long long task_cr3s[3];
unsigned long long kernel_cr3;

struct Entry PML4s[3][512] __attribute__((aligned(4096)));
struct Entry PDPs[3][512] __attribute__((aligned(4096)));
struct Entry PDs[3][512] __attribute__((aligned(4096)));
struct Entry PTs[3][8][512] __attribute__((aligned(4096)));
struct Entry kernel_PD[512] __attribute__((aligned(4096)));
struct Entry kernel_PTs[8][512] __attribute__((aligned(4096)));
struct Entry io_PD[512] __attribute__((aligned(4096)));
struct Entry fb_PT[512] __attribute__((aligned(4096)));
struct Entry lapic_PT[512] __attribute__((aligned(4096)));

void init_virtual_memory() {
  // Save kernel cr3 register value
  asm volatile ("mov %%cr3, %0" : "=r"(kernel_cr3));

  // Page mapping

  unsigned long long int i;
  for(i=0; i<3; i++){
    task_cr3s[i] = (unsigned long long)PML4s[i];
  }
  int co = -1;
  for(i=0x040000000; i<=0x041000000; i+=4096){
    co++;
    int directory = (i >> 21) & 0x1ff;
    int directory_ptr = (i >> 30) & 0x1ff;
    int pml4 = (i >> 39) & 0x1ff;
    for(int j=0; j<3; j++){ 
      unsigned long long int p4 = (unsigned long long int)PDPs[j];
      p4 = (p4 >> 12);
      PML4s[j][pml4].physical_address = p4;
      PML4s[j][pml4].present = 1;
      PML4s[j][pml4].writable = 1; 
      unsigned long long int p3 = (unsigned long long int)PDs[j];
      p3 = (p3 >> 12);
      PDPs[j][directory_ptr].physical_address = p3;
      PDPs[j][directory_ptr].present = 1;
      PDPs[j][directory_ptr].writable = 1; 
      unsigned long long int p2 = (unsigned long long int)PTs[j][co/512];
      p2 = (p2 >> 12);
      PDs[j][directory].physical_address = p2;
      PDs[j][directory].present = 1;
      PDs[j][directory].writable = 1; 
      unsigned long long int p1 = (unsigned long long int)(0x104000000 + (j << 24) + co * 4096);
      p1 = (p1 >> 12);
      PTs[j][co/512][co%512].physical_address = p1;
      PTs[j][co/512][co%512].present = 1;
      PTs[j][co/512][co%512].writable = 1; 
    }
  }
  co = 0;
  for(i=0x0c0000000; i<=0x0c0200000; i+=4096){
    co++;
    int directory = (i >> 21) & 0x1ff;
    int directory_ptr = (i >> 30) & 0x1ff;
    int pml4 = (i >> 39) & 0x1ff;
    for(int j=0; j<3; j++){ 
      unsigned long long int p4 = (unsigned long long int)PDPs[j];
      p4 = (p4 >> 12);
      PML4s[j][pml4].physical_address = p4;
      PML4s[j][pml4].present = 1;
      PML4s[j][pml4].writable = 1; 
      unsigned long long int p3 = (unsigned long long int)io_PD;
      p3 = (p3 >> 12);
      PDPs[j][directory_ptr].physical_address = p3;
      PDPs[j][directory_ptr].present = 1;
      PDPs[j][directory_ptr].writable = 1; 
      unsigned long long int p2 = (unsigned long long int)fb_PT;
      p2 = (p2 >> 12);
      io_PD[directory].physical_address = p2;
      io_PD[directory].present = 1;
      io_PD[directory].writable = 1; 
      unsigned long long int p1 = (unsigned long long int)(i);
      p1 = (p1 >> 12);
      fb_PT[co%512].physical_address = p1;
      fb_PT[co%512].present = 1;
      fb_PT[co%512].writable = 1; 
    }
  }
  co = 0;
  for(i=0x0fee00000; i<=0x0fee00400; i+=4096){
    co++;
    int directory = (i >> 21) & 0x1ff;
    int directory_ptr = (i >> 30) & 0x1ff;
    int pml4 = (i >> 39) & 0x1ff;
    for(int j=0; j<3; j++){ 
      unsigned long long int p4 = (unsigned long long int)PDPs[j];
      p4 = (p4 >> 12);
      PML4s[j][pml4].physical_address = p4;
      PML4s[j][pml4].present = 1;
      PML4s[j][pml4].writable = 1; 
      unsigned long long int p3 = (unsigned long long int)io_PD;
      p3 = (p3 >> 12);
      PDPs[j][directory_ptr].physical_address = p3;
      PDPs[j][directory_ptr].present = 1;
      PDPs[j][directory_ptr].writable = 1; 
      unsigned long long int p2 = (unsigned long long int)lapic_PT;
      p2 = (p2 >> 12);
      io_PD[directory].physical_address = p2;
      io_PD[directory].present = 1;
      io_PD[directory].writable = 1;
      unsigned long long int p1 = (unsigned long long int)(i);
      p1 = (p1 >> 12);
      lapic_PT[co%512].physical_address = p1;
      lapic_PT[co%512].present = 1;
      lapic_PT[co%512].writable = 1; 
    }
  }
  co = 0;
  for(i=0x100000000; i<=0x101000000; i+=4096){
    co++;
    int directory = (i >> 21) & 0x1ff;
    int directory_ptr = (i >> 30) & 0x1ff;
    int pml4 = (i >> 39) & 0x1ff;
    for(int j=0; j<3; j++){ 
      unsigned long long int p4 = (unsigned long long int)PDPs[j];
      p4 = (p4 >> 12);
      PML4s[j][pml4].physical_address = p4;
      PML4s[j][pml4].present = 1;
      PML4s[j][pml4].writable = 1; 
      unsigned long long int p3 = (unsigned long long int)kernel_PD;
      p3 = (p3 >> 12);
      PDPs[j][directory_ptr].physical_address = p3;
      PDPs[j][directory_ptr].present = 1;
      PDPs[j][directory_ptr].writable = 1; 
      unsigned long long int p2 = (unsigned long long int)kernel_PTs[co/512];
      p2 = (p2 >> 12);
      kernel_PD[directory].physical_address = p2;
      kernel_PD[directory].present = 1;
      kernel_PD[directory].writable = 1; 
      unsigned long long int p1 = (unsigned long long int)(i);
      p1 = (p1 >> 12);
      kernel_PTs[co/512][co%512].physical_address = p1;
      kernel_PTs[co/512][co%512].present = 1;
      kernel_PTs[co/512][co%512].writable = 1; 
    }
  }
}
