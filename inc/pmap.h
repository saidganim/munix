#ifndef MX_PMAP_H
#define MX_PMAP_H

#include <inc/mmu.h>
#include <inc/types.h>

#define PG_BUSY (1 << 0)
#define PG_HUGE (1 << 1)

struct page_info {
  struct page_info* p_next;
  uint32_t p_counter;
  pflags_t p_flags;
};

enum {
  ALLOC_HUGE = 1 << 0, // 4MB page
  ALLOC_ZERO = 1 << 1,
  ALLOC_KAS  = 1 << 2, // Allocate page from pages mapped by kernel directly
};

enum {
  CREATE_NORMAL = 1 << 1,
  CREATE_HUGE = 1 << 2,
};

struct page_info* page_alloc(pflags_t);
void page_free(struct page_info*);
pte_t* pgdir_walk(pde_t*, void*, int);
struct page_info* page_lookup(pde_t*, void*, pte_t**);
int page_insert(pde_t*,void*, struct page_info*, int);
int page_remove(pde_t*, void*);

extern struct page_info* pglist_head;
extern struct page_info* __kernel_pages; // array of page_info structures

void kmem_init();


#endif
