#ifndef MX_MEMLAYOUT
#define MX_MEMLAYOUT


#include <inc/mmu.h>
// FOR NOW no KASLR

#define KERNBASE 0xFFFF800000000000ULL
#define kva2pa(va) ((va) - KERNBASE)
#define pa2kva(pa) ((pa) + KERNBASE)
#define page2pa(p) ((void*)(PGSIZE * ((struct page_info*)p - __kernel_pages)))
#define page2kva(p) ((void*)(pa2kva(page2pa(p))))
#define pa2page(pa) ((struct page_info*)(pa / PGSIZE + __kernel_pages))
#define KSTACKSIZE (8*PGSIZE)

#ifdef __ASSEMBLER__


#else


#include <inc/errors.h>
// This function are being used only to handle addresses mapped by kernel directly.
static inline void* KADDR(void* pa){
    extern uint64_t __max_kernmapped_addr;
    if(pa > (void*)__max_kernmapped_addr)
        panic("KADDR is called with wrong address");
    return pa2kva(pa);
}
static inline void* PADDR(void* va){
    void* res = kva2pa(va);
    extern uint64_t __max_kernmapped_addr;
    if(res > (void*)__max_kernmapped_addr)
         panic("PADDR is called with wrong address");
    return res;
}
#endif // __ASSEMBLER__


#endif // MX_MEMLAYOUT
