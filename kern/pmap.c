// Everything about physical memory is here
#include <inc/kern.h>
#include <inc/pmap.h>
#include <inc/types.h>
#include <inc/x86.h>
#include <inc/memlayout.h>
#include <inc/mmu.h>
#include <inc/utils.h>

uint32_t __ramsz__;
uint32_t __max_kernmapped_addr;
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
// TODO: Need to test page_alloc() and page_free() funcs
void* page_alloc(pflags_t flags){
  size_t i, pg_num = 1;
  struct page_info *res = NULL, **resp = NULL;
  if(flags & ALLOC_HUGE){
    pg_num = HUGE_PG;
  } else {
     // Not huge alloc :
    if(flags && ALLOC_KAS){
      resp = &pglist_head;
_L1:
      if(*resp == NULL)
        goto error;

      if(page2pa(*resp) < (void*)__max_kernmapped_addr){
        res = *resp;
        *resp = (*resp)->p_next;
        goto release;
      }
      resp = &((*resp)->p_next);
      goto _L1;
    } else {
      res = pglist_head;
      pglist_head = pglist_head->p_next;
      res->p_flags = PG_BUSY;
      goto release;
    }
  }

  for(i = 0; i < __ramsz__ / PGSIZE; ++i){
    if(__kernel_pages[i].p_flags) // not free page
      res = &__kernel_pages[i];
    if((__kernel_pages + i) - res == HUGE_PG - 1)
      break;
  }

  if(page2pa(res) + HGPGSIZE > (void*)__ramsz__)
    goto error;

  if( (flags & ALLOC_KAS) && (page2pa(res) > (void*)__max_kernmapped_addr) ) // if didn't find huge free page, than there is no such one
    goto error;

  resp = &pglist_head;

  // Removing allocated pages from free pages list;
  while(*resp != NULL){
    if(page2pa(*resp) >= page2pa(res) && page2pa(*resp) < (page2pa(res) + HGPGSIZE)){
      (*resp)->p_flags = PG_HUGE || PG_BUSY;
      *resp = (*resp)->p_next;
      (*resp)->p_next = NULL;
    } else {
      resp = &(*resp)->p_next;
    }
  }
  goto release;

error:
  return NULL;

release:
  if(flags & ALLOC_ZERO)
    for(i = 0; i < pg_num; ++i)
      memset((void*)page2kva(res + i), 0x0, PGSIZE);
  return res;
};

void page_free(struct page_info* pg){
  size_t i, pg_num = 1;
  if(unlikely(pg->p_flags & PG_HUGE))
    pg_num = HUGE_PG;

  for( i = 0; i < pg_num; ++i){
    pg[i].p_flags = 0x0;
    pg[i].p_next = pglist_head;
    pglist_head = pg + i;
  }
}
