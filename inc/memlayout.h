#ifndef MX_MEMLAYOUT
#define MX_MEMLAYOUT

#include <inc/mmu.h>

#define KERNBASE 0xc0000000
#define kva2pa(va) ((va) - KERNBASE)
#define pa2kva(pa) ((pa) + KERNBASE)
#define KSTACKSIZE (8*PGSIZE)

#endif
