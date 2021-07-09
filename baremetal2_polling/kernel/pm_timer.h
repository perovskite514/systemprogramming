#pragma once
#include "hardware.h"
void init_acpi_pm_timer(struct RSDP *rsdp);
void pm_timer_wait_millisec(unsigned int msec);
unsigned int io_read32(unsigned long long addr);