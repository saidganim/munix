// Everything about physical memory is here

#include <inc/pmap.h>
#include <inc/types.h>
#include <inc/x86.h>
#include <inc/mmu.h>

uint64_t __ramsz__;

void kmem_init(){
    __ramsz__ = *(__memory_size);
}
