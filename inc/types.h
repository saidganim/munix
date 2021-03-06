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

// typedef uint32_t physaddr_t;
typedef uint64_t size_t;
typedef int64_t ssize_t;
typedef uint64_t off_t;

typedef uint64_t pml4_t;
typedef uint64_t pdt_t;
typedef uint64_t pde_t;
typedef uint64_t pte_t;

typedef char pflags_t;

typedef uint64_t size_t;
typedef int64_t ssize_t;

#endif
