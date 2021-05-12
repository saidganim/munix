#include <inc/types.h>
#include <inc/memlayout.h>
#include <inc/mmu.h>


// may be this isn't wise for the size of final elf,
// but the size of the binary in memory is still the same
// this file will be linked in the lower address region (phys addr)


/*
            uuuuuuuuuuuuuuuuuuuu
          u" uuuuuuuuuuuuuuuuuu "u
        u" u$$$$$$$$$$$$$$$$$$$$u "u
      u" u$$$$$$$$$$$$$$$$$$$$$$$$u "u
    u" u$$$$$$$$$$$$$$$$$$$$$$$$$$$$u "u
  u" u$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$u "u
u" u$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$u "u
$ $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ $
$ $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ $
$ $$$" ... "$...  ...$" ... "$$$  ... "$$$ $
$ $$$u `"$$$$$$$  $$$  $$$$$  $$  $$$  $$$ $
$ $$$$$$uu "$$$$  $$$  $$$$$  $$  """ u$$$ $
$ $$$""$$$  $$$$  $$$u "$$$" u$$  $$$$$$$$ $
$ $$$$....,$$$$$..$$$$$....,$$$$..$$$$$$$$ $
$ $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ $
"u "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" u"
  "u "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" u"
    "u "$$$$$$$$$$$$$$$$$$$$$$$$$$$$" u"
      "u "$$$$$$$$$$$$$$$$$$$$$$$$" u"
        "u "$$$$$$$$$$$$$$$$$$$$" u"
          "u """""""""""""""""" u"
            """"""""""""""""""""
*/

// DON'T:: use this structures from the C code 


__attribute__ ((aligned (PGSIZE)))
// 4 MiB is mapped by this directory
pde_t entrypgdir[PDEN] = {
  [0] = 0x0 + PTE_P + PTE_W + PTE_PS,
  [1] = 0x200000 + PTE_P + PTE_W + PTE_PS,
  /* Don't need this range for now */
};

__attribute__ ((aligned (PGSIZE)))
pdt_t page_pdt[PDTEN] = {
  [0] = (uint64_t)entrypgdir + PTE_P + PTE_W,
  /* Don't need this range for now */
  // [pdte_index(KERNBASE)] = kva2pa((uint64_t)entrypgdir) + PTE_P + PTE_W,
};

__attribute__ ((aligned (PGSIZE)))
pml4_t page_pml4[PML4EN] = {
  [0] = (uint64_t)page_pdt + PTE_P + PTE_W ,
  /* Don't need this range for now */
  [pml4e_index(KERNBASE)] = (uint64_t)page_pdt + PTE_P + PTE_W,
};

