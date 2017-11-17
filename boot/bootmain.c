#include <kern/display.h>

void bootmain(){
   __fb_print(0,'H');
  __fb_print(1,'E');
  __fb_print(2,'L');
  __fb_print(3,'L');
  __fb_print(4,'O');
}
