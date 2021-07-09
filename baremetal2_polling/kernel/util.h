#pragma once
#include "hardware.h"

void init_frame_buffer(struct FrameBuffer *fb);
void puts(char *str);
void puth(unsigned long long value, unsigned char digits_len);