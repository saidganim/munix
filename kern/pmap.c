// Everything about physical memory is here
#include <inc/kern.h>
#include <inc/pmap.h>
#include <inc/types.h>
#include <inc/x86.h>
#include <inc/memlayout.h>
#include <inc/mmu.h>
#include <inc/utils.h>
#include <inc/errors.h>

uint32_t __ramsz__;
static uint32_t __max_kernmapped_addr = PGSIZE * 1024; // 4 MiB
static void* current;
struct page_info* __kernel_pages; // array of page_info structures
struct page_info* pglist_head; // free pages linked list head

/*
  Allocates @pg_num number of pages.
  This function is used during initialization ONLY.
*/
static void* boot_alloc(unsigned int pg_num){
  extern uint32_t tend; // end of kernel text section
  if(unlikely(!current))
    current = (void*)roundup(tend, PGSIZE);
  void* res = current;
  if(current + pg_num * PGSIZE >= (void*)__ramsz__)
    return NULL;
  current += pg_num * PGSIZE;
  return res;
}

static void __init pages_init(){
  uint32_t pg_num = __ramsz__ / PGSIZE;
  uint32_t i;
  uint32_t cfree;
  __kernel_pages = boot_alloc(pg_num);
  cfree = (uint32_t)boot_alloc(0) / PGSIZE;
  for(i = cfree; i < pg_num; ++i){
    __kernel_pages[i].p_next = &__kernel_pages[i];
    __kernel_pages[i].p_flags = 0x0;
  }
  __kernel_pages[pg_num - 1].p_next = 0x0;
  pglist_head = &__kernel_pages[cfree];
}


/*
  Initialize new kernel page table and load it into %cr3;
  This function should implement some KASLR in future
*/
void __init kpt_init(void){
  

}


void __init kmem_init(){

  /*
    This function has to initialize memory pages, real KPT;
    // TODO: for now only single core mode( will be fixed in future )
    // TODO: KASLR - very important (runtime ASLR)
    // TODO: ...
  */
    __ramsz__ = *(__memory_size); // no comments :)
    pages_init();

}


// Possible flags values : ALLOC_HUGE, ALLOC_ZERO, ALLOC_KAS
// ALLOC_HUGE - no comments;
// ALLOC_ZERO - no comments;
// ALLOC_KAS  - Allocate memory which is mapped by kernel already
// TODO: Need to test page_alloc() and page_free() funcs
struct page_info* page_alloc(pflags_t flags){
  size_t i;
  struct page_info *res = NULL, *cur = NULL;
  uint32_t max_addr = rounddown(__ramsz__, PGSIZE);
  unsigned short page_size = 1;
  if(flags & ALLOC_KAS)
    max_addr = rounddown(__max_kernmapped_addr, PGSIZE);

  if(flags & ALLOC_HUGE){
    // HUGE PAGE ALLOCATION
    page_size = HUGE_PG;
    size_t begpg = 0;
    for(i = 0; i <= (max_addr - PGSIZE * HUGE_PG)/ PGSIZE - HUGE_PG; ++i){
      if(!(__kernel_pages[i].p_flags & PG_BUSY)){
        if(i - begpg == (HUGE_PG - 1) ){
          res = &__kernel_pages[begpg];
          __kernel_pages[begpg].p_flags |= PG_HUGE;
          goto _release;
        }
        continue;
      } else{
        begpg = i + 1;
        continue;
      }
    }
  } else {
    cur = pglist_head;
    // NORMAL SIZE PAGE ALLOCATION
    while(cur){
      if((uint32_t)page2kva(cur) < max_addr){
        res = cur;
        cur->p_flags = PG_BUSY;
        goto _release;
      } else 
        cur = cur->p_next;
    }
  }
  
  _release:
  if(unlikely(!res))
    return res;
  if(flags & ALLOC_ZERO)
    memset(res, 0x0, page_size * PGSIZE);
  for(i = 0 ; i < page_size; ++i){
    remove_entry(&res[i], &pglist_head, p_next);
    res[i].p_flags |= PG_BUSY;
    res[i].p_counter = 1;
    res[i].p_next = 0x0;
  }
    
  return res;
};

void page_free(struct page_info* pg){
  size_t i, pg_num = 1;
  if(unlikely(pg->p_flags & PG_HUGE))
    pg_num = HUGE_PG;

  for( i = 0; i < pg_num; ++i){
    pg[i].p_flags = 0x0;
    pg[i].p_next = pglist_head;
    pg[i].p_counter = 0;
    pglist_head = pg + i;
  }
}

/*
* Walks two-level page table structure.
* If pte of va doesnt exist:
* - If create & CREATE_NORMAL - create page for given va
* - If create & CREATE_HUGE - create page for pde(va) entry in pgdir
*/
pte_t* pgdir_walk(pde_t* pgdir, void* va, int create){
  pde_t* pde;
  pte_t* res = NULL;
  pflags_t pflags = ALLOC_ZERO | ALLOC_KAS;
  struct page_info* pp = NULL;


  pde = pgdir + PDX(va);
  if(!(*pde & PTE_P)){
    // PDE is not presented. Creating new pde
    if(!create)
      goto _fail;
    if(create & CREATE_HUGE){
      pflags |= ALLOC_HUGE;
      *pde |= PTE_PS;
    }
    if(!(pp = page_alloc(pflags)))
      goto _fail;
    *pde |= (uint32_t)page2pa(pp);
    *pde |= PTE_P;
  }

  if(*pde && PTE_PS)
    res = pde;
  else
    res = PTEADDR(*pde) + PTX(va);

  return KADDR(res);

_fail:
  *pde = 0x0;
  return NULL;
};