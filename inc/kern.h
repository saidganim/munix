#ifndef MX_KERN
#define MX_KERN

#define __init                    __attribute__ ((__section__(".init.text")))
#define unlikely(expr)            __builtin_expect((expr), 0)
#define likely(expr)              __builtin_expect((expr), 1)

#endif
