#include <inc/display.h>
#include <inc/pmap.h>
#include <inc/mmu.h>
#include <inc/types.h>
#include <inc/kern.h>
#include <inc/memlayout.h>


void prnt_greetings(){
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
  fb_print('_');
  fb_print('6');
  fb_print('4');
  fb_nextln();
}

void kinit(struct boot_config_t* boot_cfg){
  // Initializing all kernel structures(memory, processes, smp)
  fb_print('i');
  fb_print('n');
  fb_print('i');
  fb_print('t');
  fb_print(' ');
  fb_print('k');
  fb_print('e');
  fb_print('r');
  fb_print('n');
  fb_print('e');
  fb_print('l');
  fb_nextln();

  // we first relocate pointers of boot_cfg
  boot_cfg->mmap_addr += KERNBASE;
  boot_cfg->elfhdr_ptr += KERNBASE;
  kmem_init(boot_cfg); // first to initialize

}

void kmain(struct boot_config_t* boot_cfg){
  prnt_greetings();
  kinit(boot_cfg);
  while(true){};
}
