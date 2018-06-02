#ifndef MX_MMU_H
#define MX_MMU_H


// Application segment types
#define SEG_X 0x8 // Executable segment
#define SEG_W 0x2 // Writable segment
#define SEG_EXP_DWN 0x4

#define __memory_size ((unsigned int*)0x6c00) // For keeping RAM SIZE. TODO: dirty way, explore Linux kernel
// GDT Descriptors
#define MX_CS 0x8 // Kernel code segment
#define MX_DS 0x10 // Kernel data segment

/* Control Register flags */
#define CR0_PE      0x00000001  /* Protection Enable */
#define CR0_MP      0x00000002  /* Monitor coProcessor */
#define CR0_EM      0x00000004  /* Emulation */
#define CR0_TS      0x00000008  /* Task Switched */
#define CR0_ET      0x00000010  /* Extension Type */
#define CR0_NE      0x00000020  /* Numeric Errror */
#define CR0_WP      0x00010000  /* Write Protect */
#define CR0_AM      0x00040000  /* Alignment Mask */
#define CR0_NW      0x20000000  /* Not Writethrough */
#define CR0_CD      0x40000000  /* Cache Disable */
#define CR0_PG      0x80000000  /* Paging */

#define CR4_PCE     0x00000100  /* Performance counter enable */
#define CR4_MCE     0x00000040  /* Machine Check Enable */
#define CR4_PSE     0x00000010  /* Page Size Extensions */
#define CR4_DE      0x00000008  /* Debugging Extensions */
#define CR4_TSD     0x00000004  /* Time Stamp Disable */
#define CR4_PVI     0x00000002  /* Protected-Mode Virtual Interrupts */
#define CR4_VME     0x00000001  /* V86 Mode Extensions */

#define PTE_P           0x001   // Present
#define PTE_W           0x002   // Writeable
#define PTE_U           0x004   // User
#define PTE_PWT         0x008   // Write-Through
#define PTE_PCD         0x010   // Cache-Disable
#define PTE_A           0x020   // Accessed
#define PTE_D           0x040   // Dirty
#define PTE_PS          0x080   // Page Size
#define PTE_MBZ         0x180   // Bits must be zero
#define PGSIZE 4096
#define HGPGSIZE (PGSIZE * 1024)
#define HUGE_PG 1024

#define PDEN 1024
#define PTEN 1024

#define PDESHIFT 22
#define PTESHIFT 12
#define PGSHIFT 12

#define pde_index(va) ((va >> PDESHIFT) & 0x3FF)
#define pte_index(va) ((va >> PTESHIFT) & 0x3FF)

#ifdef __ASSEMBLER__

#define SEG_NULL \
    .quad 0; \

#define SEG(type, base, lim) \
  .word (((lim) >> 12) & 0xffff), ((base) & 0xffff); \
  .byte (((base) >> 16) & 0xff), ((type) | 0x90), \
    (0xC0 | (((lim) >> 28) & 0xf) ), (((base) >> 24) & 0xff);


#else


// Descriptor of segment
/*
 64.............................................................32
 +----------------------------------------------------------------
 |BASE(24-31)|G|B|O|AVL|LIMIT(16-19) |  P| DPL|S|TYPE|BASE(16-23)|
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 +---------------------------------------------------------------+
 |              BASE(0-15)           |        LIMIT(0-15)        |
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 31..............................................................0
*/
struct segment_descriptor{
  unsigned int sd_limit_1:16;
  unsigned int sd_base_1:16;
  unsigned int sd_base_2:8;
  unsigned int sd_type:4;
  unsigned int sd_s:1;
  unsigned int sd_dpl:2;
  unsigned int sd_present:1;
  unsigned int sd_limit_2:4;
  unsigned int sd_avl:1;
  unsigned int sd_o:1;
  unsigned int sd_b:1;
  unsigned int sd_g:1;
  unsigned int sd_base_3:8;
}__attribute__((packed));

#define SEG_NULL (struct segment_descriptor){0,0,0,0,0,0,0,0,0,0,0,0,0}

/* Normal segment */
#define SEG(type, base, lim, dpl) (struct segment_descriptor)          \
{ ((lim) >> 12) & 0xffff, (base) & 0xffff, ((base) >> 16) & 0xff,   \
    type, 1, dpl, 1, (unsigned) (lim) >> 28, 0, 0, 1, 1,        \
    (unsigned) (base) >> 24 }

#endif // __ASSEMBLER__
#endif // MX_MMU_H
