#include "hardware.h"
#include "util.h"

const unsigned int freq_hz = 3579545;
unsigned short pm_timer_blk;
char pm_timer_is_32;
unsigned int io_read32(unsigned long long addr);

void init_acpi_pm_timer(struct RSDP *rsdp) {

    unsigned long long xsdt_address = rsdp->xsdt_address;
    struct XSDT *xsdt = (struct XSDT *) xsdt_address;
    struct SDTH sdth = xsdt -> sdth;
    int length = (sdth.length - 36) / 8;
    xsdt++;
    for(int i = 0; i < length; i++){
        
        unsigned long long hoge = (unsigned long long)((char *)xsdt + (char)(8 * i));
        struct SDTH * sdth1 = (struct SDTH *) * (unsigned long long *) hoge; 
        if(sdth1->signature[0] == 'F' && sdth1->signature[1] == 'A' && sdth1->signature[2] == 'C' && sdth1->signature[3] == 'P'){
            struct FADT * fadt = (struct FADT *) * (unsigned long long *) hoge;
            pm_timer_blk = fadt -> PM_TMR_BLK;
            pm_timer_is_32 = ((fadt -> flags) >> 8) & 1;
            break;   
        }
    }
    // No FACP found
    return;
}

void pm_timer_wait_millisec(unsigned int msec){

    unsigned int clock = io_read32(pm_timer_blk);
    if(pm_timer_is_32){
        while(1){
            unsigned int clock1 = io_read32(pm_timer_blk);
            if(clock1 <= clock){
                if((unsigned long long int)(1000 * (4294967295 - clock + clock1)) >= (unsigned long long int)(freq_hz * msec)){
                    return;
                } 
            }   
            else{
                if((unsigned long long int)(1000 * (clock1 - clock)) >= (unsigned long long int)(freq_hz * msec)){
                    return;
                }
            }
        }
    }
    else{
        clock = 0xffffff & clock;
        while(1){
            unsigned long long int clock1 = io_read32(pm_timer_blk);
            clock1 = 0xffffff & clock1;
            if(clock1 <= clock){
                if((unsigned long long int)(1000 * (16777215 - clock + clock1)) >= (unsigned long long int)(freq_hz * msec)){
                    return;
                } 
            }   
            else{
                if((unsigned long long int)(1000 * (clock1 - clock)) >= (unsigned long long int)(freq_hz * msec)){
                    return;
                }
            }
        }
    }
}

 
    