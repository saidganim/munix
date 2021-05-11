/* Kernel starts executing here. Bootloader jumps to point below */


#include <inc/memlayout.h>
#define MULTIBOOT_MAGIC (0x1BAD002)
#define MULTIBOOT_FLAGS (0x0)
.align 4
.text
.code32
.global multiboot_hdr;

multiboot_hdr:
/* Header for multiboot;
http://www.gnu.org/software/grub/manual/multiboot/multiboot.html */
  .long MULTIBOOT_MAGIC
  .long MULTIBOOT_FLAGS
  .long (-MULTIBOOT_MAGIC - MULTIBOOT_FLAGS)

.global _start_kernel_
_start_kernel_ = kva2pa(entry)

.globl entry
entry:
movl %cr4, %eax
orl $(CR4_PSE), %eax
movl %eax, %cr4

movl $(kva2pa(entrypgdir)), %eax
movl %eax, %cr3

movl %cr0, %eax
orl $(CR0_PG | CR0_WP), %eax
movl %eax, %cr0

movl $(bootstack + KSTACKSIZE), %esp
movl $0, %ebp

call kmain

.comm bootstack, KSTACKSIZE
