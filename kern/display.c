#include <kern/display.h>
#include <inc/x86.h>

static uint16_t* const frame_buffer = (uint16_t*)FBUFFER_IO;
static uint32_t fb_cursor = FBUFFER_IO;

void __move_cursor(uint16_t position){
  x86_outb(0x3D4, 14);
  x86_outb(0x3D5, position >> 8);
  x86_outb(0x3D4, 15);
  x86_outb(0x3D5, position & 0xff);
}

void __fb_print_clr(int index, char chr, char background, char foreground){
  if(index >= FBUFFER_SIZE) // Out of frame buffer
    return;
  frame_buffer[index] = chr;
  frame_buffer[index] |= (background << 4 | foreground) << 8;
}

void __fb_print(int index, char chr){
  if(index >= FBUFFER_SIZE) // Out of frame buffer
    return;
  frame_buffer[index] = chr;
  frame_buffer[index] |= (FB_BLACK << 4 | FB_WHITE) << 8;
};

void fb_print(char chr){
  __fb_print(fb_cursor++, chr);
  __move_cursor(fb_cursor);
}

void fb_print_clr(char chr, char background, char foreground){
  __fb_print_clr(fb_cursor++, chr, background, foreground);
  __move_cursor(fb_cursor);
}
