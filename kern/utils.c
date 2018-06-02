#include <inc/utils.h>
#include <inc/types.h>


void memset(void* buf, char b, size_t n){
  size_t i = 0;
  for(; i < n; ++i)
    ((char*)buf)[i] = b;
}
