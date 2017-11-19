#ifndef MX_X86_H
#define MX_X86_H

static inline void x86_outb(int port, char data){
  __asm __volatile("outb %0, %w1" : : "a"(data), "d"(port));
}

#endif
