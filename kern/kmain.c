#include <kern/display.h>
#include <kern/cmos.h>
#include <inc/pmap.h>
#include <inc/mmu.h>
#include <inc/types.h>




void kinit(){
  // Initializing all kernel structures(memory, processes, smp)
  kmem_init(); // first to initialize

}

void kmain(){
  fb_clear();
  fb_print('M');
  fb_print('U');
  fb_print('N');
  fb_print('I');
  fb_print('X');
  fb_print(' ');
  fb_print('V');
  fb_print('0');
  fb_print('.');
  fb_print('1');
  fb_print(' ');
  fb_print('x');
  fb_print('8');
  fb_print('6');
  kinit();
  while(true){};
}
