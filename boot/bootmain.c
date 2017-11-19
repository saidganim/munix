#include <kern/display.h>

void bootmain(){
  fb_print_clr('H', FB_BLACK, FB_GREEN);
  fb_print_clr('E', FB_BLACK, FB_GREEN);
  fb_print_clr('L', FB_BLACK, FB_GREEN);
  fb_print_clr('L', FB_BLACK, FB_GREEN);
  fb_print_clr('O', FB_BLACK, FB_GREEN);
}
