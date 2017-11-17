#include <kern/display.h>

static char *frame_buffer = (char*)FBUFFER_IO;

void __fb_print_clr(int index, char chr, uint16_t background, uint16_t foreground){
  //frame_buffer = (char*)FBUFFER_IO;
  frame_buffer[index * 2] = chr;
  frame_buffer[index * 2 + 1] = background << 4 | foreground;
}

void __fb_print(int index, char chr){
  //frame_buffer = (char*)FBUFFER_IO;
  frame_buffer[index * 2] = chr;
  frame_buffer[index * 2 + 1] = FB_BLACK << 4 | FB_WHITE;
}
