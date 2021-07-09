#include "lapic_timer.h"
#include "pm_timer.h"
#include "hardware.h"
#include "sched.h"
#include "util.h"

#define COUNT_MAX 0xffffffff

volatile unsigned int *lvt_timer = (unsigned int *)0xfee00320;
volatile unsigned int *initial_count = (unsigned int *)0xfee00380;
volatile unsigned int *current_count = (unsigned int *)0xfee00390;
volatile unsigned int *divide_config = (unsigned int *)0xfee003e0;

unsigned int lapic_timer_freq_khz = 0;

volatile unsigned int *lapic_eoi = (unsigned int *)0xfee000b0;


void (*reserved_callback)(unsigned long long);

unsigned int measure_lapic_freq_khz() {
  // Initialize LAPIC One-Shot timer
  *divide_config = ((*divide_config) & 0xfffffff0) | 0xb;
  *initial_count = (unsigned int) 0xffffffff;
  pm_timer_wait_millisec(1000);
  lapic_timer_freq_khz = (*initial_count - *current_count) / 1000;
  // Calculate LAPIC Freq.

  return lapic_timer_freq_khz;

}

void lapic_periodic_exec(unsigned int msec, void *callback) {

  if (!lapic_timer_freq_khz) {
    // Comment out me, if needed.
    puts("Call measure_lapic_freq_khz() beforehand\r\n");
    return;
  }

  reserved_callback = callback;
  // Set LAPIC Periodic Timer
  * lapic_eoi = 0x0;
  *lvt_timer = ((*lvt_timer) & 0x00001100) | 0x00020020; 
  *divide_config = ((*divide_config) & 0xfffffff0) | 0xb;
  *initial_count = lapic_timer_freq_khz * msec;
  //

  //
}

void lapic_intr_handler_internal(unsigned long long sp) {
  // Set End of Interrupt
  reserved_callback(sp);
}

void set_eoi(){
  * lapic_eoi = 0x0;
  return;
}