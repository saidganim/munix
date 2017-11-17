#ifndef MX_TYPES_H
#define MX_TYPES_H

#ifndef NULL
#define NULL ((void*) 0)
#endif

typedef _Bool bool;
enum { false, true };

typedef __signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

typedef uint32_t physaddr_t;
typedef uint32_t size_t;
typedef int32_t ssize_t;
typedef uint32_t off_t;

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


#endif
