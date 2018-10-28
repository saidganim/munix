#include <inc/types.h>
#include <inc/memlayout.h>
#include <inc/mmu.h>

__attribute__ ((aligned (PGSIZE)))
// 4 MiB is mapped by this table
pde_t entrypgdir[PDEN] = {
  [0] = 0x0 + PTE_P + PTE_W + PTE_PS,
  /* Don't need this range for now */
  [pde_index(KERNBASE)] = 0x0 + PTE_P + PTE_W + PTE_PS,
};
