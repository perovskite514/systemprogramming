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

  for(int i=0; i<3; i++){
    task_cr3s[i] = (unsigned long long)PML4s[i];
  }
  for(int i=0; i<3; i++){ 
    unsigned long long int p4 = (unsigned long long int)PDPs[i];
    p4 = (p4 >> 12);
    PML4s[i][0].physical_address = p4;
    PML4s[i][0].present = 1;
    PML4s[i][0].writable = 1; 
    unsigned long long int p3 = (unsigned long long int)PDs[i];
    p3 = (p3 >> 12);
    PDPs[i][1].physical_address = p3;
    PDPs[i][1].present = 1;
    PDPs[i][1].writable = 1; 
    for(int j=0; j<8; j++){
      unsigned long long int p2 = (unsigned long long int)PTs[i][j];
      p2 = (p2 >> 12);
      PDs[i][j].physical_address = p2;
      PDs[i][j].present = 1;
      PDs[i][j].writable = 1; 
      for(int k=0; k<512; k++){
        unsigned long long int p1 = (unsigned long long int)(0x104000 + i * 0x1000 + j * 512 + k);
        PTs[i][j][k].physical_address = p1;
        PTs[i][j][k].present = 1;
        PTs[i][j][k].writable = 1; 
      }
    }
  }
  for(int i=0; i<3; i++){ 
    unsigned long long int p3 = (unsigned long long int)io_PD;
    p3 = (p3 >> 12);
    PDPs[i][3].physical_address = p3;
    PDPs[i][3].present = 1;
    PDPs[i][3].writable = 1; 
  }
  unsigned long long int p2 = (unsigned long long int)fb_PT;
  p2 = (p2 >> 12);
  io_PD[0].physical_address = p2;
  io_PD[0].present = 1;
  io_PD[0].writable = 1; 
  p2 = (unsigned long long int)lapic_PT;
  p2 = (p2 >> 12);
  io_PD[503].physical_address = p2;
  io_PD[503].present = 1;
  io_PD[503].writable = 1; 
  for(int k=0; k<512; k++){
    unsigned long long int p1 = (unsigned long long int)(0x0c0000 + k);
    fb_PT[k].physical_address = p1;
    fb_PT[k].present = 1;
    fb_PT[k].writable = 1; 
  }
  unsigned long long int p1 = (unsigned long long int)(0xfee00);
  lapic_PT[0].physical_address = p1;
  lapic_PT[0].present = 1;
  lapic_PT[0].writable = 1;
  for(int i=0; i<3; i++){ 
    unsigned long long int p3 = (unsigned long long int)kernel_PD;
    p3 = (p3 >> 12);
    PDPs[i][4].physical_address = p3;
    PDPs[i][4].present = 1;
    PDPs[i][4].writable = 1; 
  }
  for(int j=0; j<8; j++){
      unsigned long long int p2 = (unsigned long long int)kernel_PTs[j];
      p2 = (p2 >> 12);
      kernel_PD[j].physical_address = p2;
      kernel_PD[j].present = 1;
      kernel_PD[j].writable = 1; 
      for(int k=0; k<512; k++){
        unsigned long long int p1 = (unsigned long long int)(0x100000 + j * 512 + k);
        kernel_PTs[j][k].physical_address = p1;
        kernel_PTs[j][k].present = 1;
        kernel_PTs[j][k].writable = 1; 
      }
  }
}