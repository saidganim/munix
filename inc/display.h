#ifndef MX_DISPLAY_H
#define MX_DISPLAY_H

#include <inc/types.h>

// Basic Display driver

#define FBUFFER_IO 0x000b8000 // IO area in memory which maps framebuffer
#define FBUFFER_COLS 80
#define FBUFFER_ROWS 25
#define FBUFFER_SIZE (FBUFFER_ROWS * FBUFFER_COLS)

// Colors
#define FB_BLACK 0x0
#define FB_BLUE 0x1
#define FB_GREEN 0x2
#define FB_CYAN 0x3
#define FB_RED 0x4
#define FB_MAGENTA 0x5
#define FB_BROWN 0x6
#define FB_LGREY 0x7
#define FB_DGREY 0x8
#define FB_LBLUE 0x9
#define FB_LGREEN 0xA
#define FB_LCYAN 0xB
#define FB_LRED 0xC
#define FB_LMAGENTA 0xD
#define FB_LBROWN 0xE
#define FB_WHITE 0xF

void __fb_print_clr(int, char, char, char); // print colored char at i-th position
void __fb_print(int, char); // print char at ith position (white on black);
void fb_print(char chr);
void fb_print_clr(char chr, char background, char foreground);
void fb_clear();



#endif
