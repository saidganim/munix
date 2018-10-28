#ifndef MX_UTILS
#define MX_UTILS

#include <inc/types.h>
// entry - *, list_head - **, member - pointer in structure
#define remove_entry(entry, list_head, member) {\
    __typeof__(entry) _entry = entry;\
    __typeof__(list_head) _head = list_head;\
    while(*_head){\
        if(*_head == _entry){\
        *_head = (*_head)->member;\
        } else {\
            _head = &(*_head)->member;\
        }\
    }\
}

#define MIN(_a, _b)                                                            \
({                                                                             \
    typeof(_a) __a = (_a);                                                     \
    typeof(_b) __b = (_b);                                                     \
    __a <= __b ? __a : __b;                                                    \
})
#define MAX(_a, _b)                                                            \
({                                                                             \
    typeof(_a) __a = (_a);                                                     \
    typeof(_b) __b = (_b);                                                     \
    __a >= __b ? __a : __b;                                                    \
})

#define roundup(addr, brdr) ({ \
  const typeof(brdr) __brdr = brdr;\
  ((addr + (__brdr - 1)) / __brdr * __brdr); \
})

#define rounddown(addr, brdr) ({\
  const typeof(brdr) _brdr = brdr;\
  (addr / _brdr * _brdr);\
})

void memset(void* buf, char b, size_t n);



#endif
