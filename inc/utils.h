#ifndef MX_UTILS
#define MX_UTILS

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




#include <inc/types.h>

void memset(void* buf, char b, size_t n);



#endif
