// Everything about physical memory is here
#include <inc/kern.h>
#include <inc/pmap.h>
#include <inc/types.h>
#include <inc/x86.h>
#include <inc/memlayout.h>
#include <inc/mmu.h>
#include <inc/utils.h>
#include <inc/errors.h>

uint64_t __ramsz__;
uint64_t __max_kernmapped_addr = PGSIZE * 1024; // 4 MiB
static void* current;
struct page_info* __kernel_pages; // array of page_info structures
struct page_info* pglist_head; // free pages linked list head



void fix_memory_map(struct boot_config_t* boot_cfg){
  struct e820_mmap_ent_t (*mmap_arr)[] = (struct e820_mmap_ent_t (*)[])boot_cfg->mmap_addr;
  uint32_t i = 0, j = 0;
  uint32_t ss_gap = 0;
  struct e820_mmap_ent_t tmp_ent ={0};
  // we first sort
  // dummy shell-sort here
  for(ss_gap = boot_cfg->mmap_cnt/2; ss_gap != 0; ss_gap /= 2){
      for(i = ss_gap; i < boot_cfg->mmap_cnt; ++i){
        tmp_ent = (*mmap_arr)[i];
        for(j = i; (j >= ss_gap) && ((*mmap_arr)[j-ss_gap].mm_base_addr > tmp_ent.mm_base_addr); j -= ss_gap){
          (*mmap_arr)[j] = (*mmap_arr)[j-ss_gap];
        }
      }

  }

  // second we merge crossing mem regions
  for(i = 1; i < boot_cfg->mmap_cnt; ++i){
    if((*mmap_arr)[i].mm_base_addr < ((*mmap_arr)[i-1].mm_base_addr + (*mmap_arr)[i-1].mm_size) &&
        (*mmap_arr)[i].mm_type == (*mmap_arr)[i-1].mm_type
    ){
        // merge entries
        (*mmap_arr)[i-1].mm_size = MAX((*mmap_arr)[i-1].mm_base_addr + (*mmap_arr)[i-1].mm_size, (*mmap_arr)[i].mm_base_addr + (*mmap_arr)[i].mm_size) - (*mmap_arr)[i-1].mm_base_addr;
        for(j = i; j < boot_cfg->mmap_cnt - 1; ++j) (*mmap_arr)[j] = (*mmap_arr)[j+1];
        --boot_cfg->mmap_cnt;
    }
  }
  __ramsz__ = (*mmap_arr)[boot_cfg->mmap_cnt - 1].mm_base_addr + (*mmap_arr)[boot_cfg->mmap_cnt - 1].mm_size;

}


/*
  Allocates @pg_num number of pages.
  This function is used during initialization ONLY.
*/
static void* boot_alloc(unsigned int pg_num){
  extern char tend; // end of kernel image in the memory
  if(unlikely(!current))
    current = (void*)roundup((uint64_t)&tend, PGSIZE);
  void* res = current;
  if(current + pg_num * PGSIZE >= (void*)__ramsz__)
    return NULL;
  current += pg_num * PGSIZE;
  return res;
}

static void __init pages_init(){
  uint64_t pg_num = __ramsz__ / PGSIZE;
  uint64_t i;
  uint64_t cfree;
  __kernel_pages = boot_alloc(pg_num);
  cfree = (uint64_t)boot_alloc(0) / PGSIZE;
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


void __init kmem_init(struct boot_config_t* boot_cfg){

  /*
    This function has to initialize memory pages, real KPT;
    // TODO: for now only single core mode( will be fixed in future )
    // TODO: KASLR - very important (runtime ASLR)
    // TODO: ...
  */

    fix_memory_map(boot_cfg);
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
  uint64_t max_addr = rounddown(__ramsz__, PGSIZE);
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
      if((uint64_t)page2kva(cur) < max_addr){
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
    *pde |= (uint64_t)page2pa(pp);
    *pde |= PTE_P;
  }

  if(*pde && PTE_PS)
    res = pde;
  else
    res = (pte_t*)PTEADDR(*pde) + PTX(va);

  return KADDR(res);

_fail:
  *pde = 0x0;
  return NULL;
};

int page_insert(pde_t* pgdir,void* va, struct page_info* pp, int perm){
  pte_t* pte;
  int createf = CREATE_NORMAL;
  size_t i,page_to_remove = 1;
  pte = pgdir_walk(pgdir, va, 0);
  
  if(perm & PTE_PS){
    createf = CREATE_HUGE;
    page_to_remove = HUGE_PG;
  }

  if(pte)
    for(i = 0; i < page_to_remove; ++i)
      page_remove(pgdir, va + PGSIZE * i); // might return -EFAULT, but it doesn't matter.

  pte = pgdir_walk(pgdir, va, createf);
  if(!pte)
    return -ENOMEM;

  *pte = (uint64_t)page2pa(pp);
  *pte |= perm;
  return 0;
};

int page_remove(pde_t* pgdir, void* va){
  struct page_info *pp = NULL;
  pte_t* pte = pgdir_walk(pgdir, va, 0);
  if(!pte)
    return -EFAULT;
  pp = pa2page(PTEADDR(*pte));
  page_decref(pp);
  *pte = 0;
  return 0;
};

void page_decref(struct page_info* pp){
  if(!pp->p_counter)
    panic("page_decref on page with counter == 0");
  --pp->p_counter;
  if(!pp->p_counter)
    page_free(pp);
}

struct page_info* page_lookup(pde_t* pgdir, void* va, pte_t** pte_store){
  pte_t *pte = pgdir_walk(pgdir, va, 0);
  if(!pte || !(*pte & PTE_P))
    return NULL;
  if(pte_store)
    *pte_store = pte;
  return pa2page(PTEADDR(*pte));
};


/***************************************************************
 * Checking functions.
 * These functions are taken from JOS framework , which is base of current kernel.
 ***************************************************************/

/*
 * Check that the pages on the page_free_list are reasonable.
 */
// static void check_page_free_list(bool only_low_memory)
// {
//     struct page_info *pp;
//     unsigned pdx_limit = only_low_memory ? 1 : NPDENTRIES;
//     int nfree_basemem = 0, nfree_extmem = 0;
//     char *first_free_page;

//     if (!page_free_list)
//         panic("'page_free_list' is a null pointer!");

//     if (only_low_memory) {
//         /* Move pages with lower addresses first in the free list, since
//          * entry_pgdir does not map all pages. */
//         struct page_info *pp1, *pp2;
//         struct page_info **tp[2] = { &pp1, &pp2 };
//         for (pp = page_free_list; pp; pp = pp->pp_link) {
//             int pagetype = PDX(page2pa(pp)) >= pdx_limit;
//             *tp[pagetype] = pp;
//             tp[pagetype] = &pp->pp_link;
//         }
//         *tp[1] = 0;
//         *tp[0] = pp2;
//         page_free_list = pp1;
//     }

//     /* if there's a page that shouldn't be on the free list,
//      * try to make sure it eventually causes trouble. */
//     for (pp = page_free_list; pp; pp = pp->pp_link)
//         if (PDX(page2pa(pp)) < pdx_limit)
//             memset(page2kva(pp), 0x97, 128);

//     first_free_page = (char *) boot_alloc(0);
//     for (pp = page_free_list; pp; pp = pp->pp_link) {
//         /* check that we didn't corrupt the free list itself */
//         assert(pp >= pages);
//         assert(pp < pages + npages);
//         assert(((char *) pp - (char *) pages) % sizeof(*pp) == 0);

//         /* check a few pages that shouldn't be on the free list */
//         assert(page2pa(pp) != 0);
//         assert(page2pa(pp) != IOPHYSMEM);
//         assert(page2pa(pp) != EXTPHYSMEM - PGSIZE);
//         assert(page2pa(pp) != EXTPHYSMEM);
//         assert(page2pa(pp) < EXTPHYSMEM || (char *) page2kva(pp) >= first_free_page);

//         if (page2pa(pp) < EXTPHYSMEM)
//             ++nfree_basemem;
//         else
//             ++nfree_extmem;
//     }

//     assert(nfree_basemem > 0);
//     assert(nfree_extmem > 0);
// }

// /*
//  * Check the physical page allocator (page_alloc(), page_free(),
//  * and page_init()).
//  */
// static void check_page_alloc(void)
// {
//     struct page_info *pp, *pp0, *pp1, *pp2;
//     struct page_info *php0, *php1, *php2;
//     int nfree, total_free;
//     struct page_info *fl;
//     char *c;
//     int i;

//     if (!pages)
//         panic("'pages' is a null pointer!");

//     /* check number of free pages */
//     for (pp = page_free_list, nfree = 0; pp; pp = pp->pp_link)
//         ++nfree;
//     total_free = nfree;

//     /* should be able to allocate three pages */
//     pp0 = pp1 = pp2 = 0;
//     assert((pp0 = page_alloc(0)));
//     assert((pp1 = page_alloc(0)));
//     assert((pp2 = page_alloc(0)));

//     assert(pp0);
//     assert(pp1 && pp1 != pp0);
//     assert(pp2 && pp2 != pp1 && pp2 != pp0);
//     assert(page2pa(pp0) < npages*PGSIZE);
//     assert(page2pa(pp1) < npages*PGSIZE);
//     assert(page2pa(pp2) < npages*PGSIZE);

//     /* temporarily steal the rest of the free pages.
//      *
//      * Lab 1 Bonus:
//      * For the bonus, if you go for a different design for the page allocator,
//      * then do update here suitably to simulate a no-free-memory situation */
//     fl = page_free_list;
//     page_free_list = 0;

//     /* should be no free memory */
//     assert(!page_alloc(0));

//     /* free and re-allocate? */
//     page_free(pp0);
//     page_free(pp1);
//     page_free(pp2);
//     pp0 = pp1 = pp2 = 0;
//     assert((pp0 = page_alloc(0)));
//     assert((pp1 = page_alloc(0)));
//     assert((pp2 = page_alloc(0)));
//     assert(pp0);
//     assert(pp1 && pp1 != pp0);
//     assert(pp2 && pp2 != pp1 && pp2 != pp0);
//     assert(!page_alloc(0));

//     /* test flags */
//     memset(page2kva(pp0), 1, PGSIZE);
//     page_free(pp0);
//     assert((pp = page_alloc(ALLOC_ZERO)));
//     assert(pp && pp0 == pp);
//     c = page2kva(pp);
//     for (i = 0; i < PGSIZE; i++)
//         assert(c[i] == 0);

//     /* give free list back */
//     page_free_list = fl;

//     /* free the pages we took */
//     page_free(pp0);
//     page_free(pp1);
//     page_free(pp2);

//     /* number of free pages should be the same */
//     for (pp = page_free_list; pp; pp = pp->pp_link)
//         --nfree;
//     assert(nfree == 0);

//     cprintf("[4K] check_page_alloc() succeeded!\n");

//     /* test allocation of huge page */
//     pp0 = pp1 = php0 = 0;
//     assert((pp0 = page_alloc(0)));
//     assert((php0 = page_alloc(ALLOC_HUGE)));
//     assert((pp1 = page_alloc(0)));
//     assert(pp0);
//     assert(php0 && php0 != pp0);
//     assert(pp1 && pp1 != php0 && pp1 != pp0);
//     assert(0 == (page2pa(php0) % (1024*PGSIZE)));
//     if (page2pa(pp1) > page2pa(php0)) {
//         assert(page2pa(pp1) - page2pa(php0) >= 1024*PGSIZE);
//     }

//     /* free and reallocate 2 huge pages */
//     page_free(php0);
//     page_free(pp0);
//     page_free(pp1);
//     php0 = php1 = pp0 = pp1 = 0;
//     assert((php0 = page_alloc(ALLOC_HUGE)));
//     assert((php1 = page_alloc(ALLOC_HUGE)));

//     /* Is the inter-huge-page difference right? */
//     if (page2pa(php1) > page2pa(php0)) {
//         assert(page2pa(php1) - page2pa(php0) >= 1024*PGSIZE);
//     } else {
//         assert(page2pa(php0) - page2pa(php1) >= 1024*PGSIZE);
//     }

//     /* free the huge pages we took */
//     page_free(php0);
//     page_free(php1);

//     /* number of free pages should be the same */
//     nfree = total_free;
//     for (pp = page_free_list; pp; pp = pp->pp_link)
//         --nfree;
//     assert(nfree == 0);

//     cprintf("[4M] check_page_alloc() succeeded!\n");
// }