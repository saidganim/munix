#ifndef MX_MEMLAYOUT
#define MX_MEMLAYOUT

#include <inc/mmu.h>

// FOR NOW only 1GB of RAM is supported and no KASLR

#define KERNBASE 0xc0000000
#define kva2pa(va) ((va) - KERNBASE)
#define pa2kva(pa) ((pa) + KERNBASE)
#define page2pa(p) ((void*)(PGSIZE * ((struct page_info*)p - __kernel_pages)))
#define page2kva(p) ((void*)(pa2kva(page2pa(p))))
#define KSTACKSIZE (8*PGSIZE)

#endif
