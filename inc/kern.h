#ifndef MX_KERN
#define MX_KERN

#include <inc/types.h>

#define __init                    __attribute__ ((__section__(".init.text")))
#define unlikely(expr)            __builtin_expect((expr), 0)
#define likely(expr)              __builtin_expect((expr), 1)

struct boot_config_t{
  uint64_t mmap_addr;
  uint32_t mmap_cnt;
  uint32_t elfhdr_ptr;
};


#endif
