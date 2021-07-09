#include "hardware.h"
extern unsigned char font[128][8];
static struct FrameBuffer *FB;
static unsigned int fb_y;
static unsigned int fb_x;

void init_frame_buffer(struct FrameBuffer *fb){

    FB = fb;
    fb_y = 0;
    fb_x = 0;
    for (unsigned int i = 0; i < (FB->height); i++) {
    for (unsigned int j = 0; j < (FB->width); j++)  {
      struct Pixel *pixel = (FB->base) + (FB->width) * i + j;
      pixel->r = 111;//111
      pixel->g = 51;//51
      pixel->b = 129;//129
    }
  }
  return;
}

static void putc(char c){

    if(c == 10){
        //if(fb_y == (FB->height)){
          //  init_frame_buffer(FB);
        //}
        //else{
            fb_x = 0;
            fb_y += 8;
        //}
    }
    else{

        for (unsigned int y = 0; y < 8; y++){
        for (unsigned int x = 0; x < 8; x++){
            struct Pixel *pixel = (FB->base) + (fb_y + y) * (FB->width) + (fb_x + x);
            if(((font[(int)c][y] >> (7 - x)) & 1) == 1){
                pixel->r = 0;
                pixel->g = 0;
                pixel->b = 0;
            }
        }
        }
        fb_x += 8;

    }

    if(fb_x == FB->width){
        fb_x = 0;
        fb_y += 8;
    }
    if(fb_y >= FB->height){
        init_frame_buffer(FB);
    }
}

void puts(char *str){
    unsigned int i = 0;
    while(1){
        if(str[i] == 0){
            break;
        }
        else putc(str[i]);
        i++;
    }
}

void puth(unsigned long long value, unsigned char digits_len){
    
    unsigned int a = (int)digits_len;
    unsigned char c[a];
    for(unsigned i = 0; i < a; i++){
        c[a-1-i] = value % 16;
        value /= 16; 
    }
    for(unsigned int i = 0; i < a; i++){
        if(c[i] < 10) putc(c[i] + 48);
        else putc(c[i] + 87);
    }
}