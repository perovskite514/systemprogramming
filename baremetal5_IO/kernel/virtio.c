#include "hardware.h"
#include "util.h"
unsigned int io_read32(unsigned long long addr);
unsigned short io_read8(unsigned long long addr);
void io_write32(unsigned short a, unsigned long long addr);

void init(){

    for(int i=0; i<255; i++){
        int bus = i << 16;
        for(int j=0; j<32; j++){
            int device = j << 11;
            unsigned int addr = (1 << 31);
            addr += bus + device;
            io_write32(0xcf8, addr);
            unsigned int data = io_read32(0xcfc);
            int vid = data & 0xffff;
            int did = data >> 16;
            if(vid == 0x1af4 && did == 0x1000){
                addr += 16;
                io_write32(0xcf8, addr);
                unsigned int bar0 = io_read32(0xcfc);
                bar0 = bar0 & 0xfffffffc;
                bar0 += 20;
                unsigned long long addr2 = 0;
                for(int k=0; k<6; k++){
                    addr2 = (addr2 << 8);
                    unsigned short hoge = io_read8(bar0);
                    hoge = hoge & 0x00ff;
                    addr2 += hoge;
                    bar0++;
                }
                puts("MAC ADDRESS\n");
                puth(addr2, 12);
            }
        }
    }

    return;
}
