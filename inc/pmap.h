#ifndef MX_PMAP_H
#define MX_PMAP_H

#include <inc/mmu.h>
#include <inc/types.h>

#define roundup(addr, brdr) ({ \
  const typeof(brdr) __brdr = brdr;\
  ((addr + (__brdr - 1)) / __brdr * __brdr); \
})

#define rounddown(addr, brdr) ({\
  const typeof(brdr) _brdr = brdr;\
  (addr / _brdr * _brdr);\
})

extern uint32_t __ramsz__;
extern struct page_info* pglist_head;
extern struct page_info* __kernel_pages; // array of page_info structures

void kmem_init();

#define PG_BUSY (1 << 0)
#define PG_HUGE (1 << 1)

enum {
  ALLOC_HUGE = 1 << 0, // 4MB page
  ALLOC_ZERO = 1 << 1,
  ALLOC_KAS  = 1 << 2, // Allocate page from pages mapped by kernel directly
};

struct page_info {
  struct page_info* p_next;
  uint32_t p_counter;
  pflags_t p_flags;
};

struct page_info* page_alloc(pflags_t);
void page_free(struct page_info*);


#endif
