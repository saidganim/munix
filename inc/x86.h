#ifndef MX_X86_H
#define MX_X86_H

#include <inc/types.h>

static inline void x86_outb(int port, char data){
  __asm __volatile("outb %0, %w1" : : "a"(data), "d"(port));
}

static inline uint8_t x86_inb(uint16_t port){
  uint8_t data;
  __asm __volatile("inb %1, %b0" : "=a"(data) : "d"(port));
  return data;
}

static inline void x86_insl(void* addr, uint32_t count, uint32_t port){
  __asm __volatile("cld; rep insl;" : "=D"(addr) : "0"(addr), "c"(count), "d"(port): "memory", "cc");
}

#endif
