#ifndef MX_MEMLAYOUT
#define MX_MEMLAYOUT

#include <inc/mmu.h>
#include <inc/errors.h>

// FOR NOW only 1GB of RAM is supported and no KASLR

#define KERNBASE 0xc0000000
#define kva2pa(va) ((va) - KERNBASE)
#define pa2kva(pa) ((pa) + KERNBASE)
#define page2pa(p) ((void*)(PGSIZE * ((struct page_info*)p - __kernel_pages)))
#define page2kva(p) ((void*)(pa2kva(page2pa(p))))
#define KSTACKSIZE (8*PGSIZE)


static inline void* KADDR(void* pa){
    void* res = pa2kva(pa);
    extern uint32_t __ramsz__;
    if(res > (void*)__ramsz__)
        panic("KADDR is called with wrong address");
    return res;
}

#endif
