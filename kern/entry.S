/* Kernel starts executing here. Bootloader jumps to point below */


#include <inc/memlayout.h>
#define MULTIBOOT_MAGIC (0x1BAD002)
#define MULTIBOOT_FLAGS (0x0)
.align 4
.text
.code32
// .global multiboot_hdr;

// multiboot_hdr:
/* Header for multiboot;
http://www.gnu.org/software/grub/manual/multiboot/multiboot.html */
  // .long MULTIBOOT_MAGIC
  // .long MULTIBOOT_FLAGS
  // .long (-MULTIBOOT_MAGIC - MULTIBOOT_FLAGS)

.global _start_kernel_
_start_kernel_ = kva2pa(entry)

.globl entry
entry:
# setting up new stack
movl $bootstacktop, %esp
movl $0, %ebp

# saving boot_cfg ptr (64-bit version of the ptr)
pushl $0x0
pushl %ebx


// Enable page size extension and page address extension
movl %cr4, %eax
orl $(CR4_PSE), %eax
orl $(CR4_PAE), %eax
movl %eax, %cr4

mov $page_pml4, %eax
movl %eax, %cr3
movl $MSR_EFER, %ecx
rdmsr
orl $MSR_EFER_LME, %eax
wrmsr

movl %cr0, %eax
orl $(CR0_PG | CR0_WP), %eax
movl %eax, %cr0

lgdt gdt64_reg

movw $0x10, %ax # data segment selector
movw %ax, %ds
movw %ax, %es
movw %ax, %ss

ljmp $0x08, $entry64

.code64
entry64:
// Finally. We are in 64bit mode... I am crying :)
// now lets change the stack pointer and boot_cfg ptr param
movabs $KERNBASE, %rax
addq %rax, %rsp
popq %rdi
addq %rax, %rdi // finally, function args go through regs like normal people do

movabs $kmain, %rax
call *%rax

// We never reach here
spin:
hlt
jmp spin

.align 4
gdt64_table:
  SEG_NULL;
  // Code segment(SEG_W here means readable executable segment, read Intel manual)
  SEG64(SEG_X | SEG_W, 0x0, 0xffffffff)
  // Data segment
  SEG64(SEG_W, 0x0, 0xffffffff)
  // User segments
  // Code:
  SEG64USER(SEG_X | SEG_W, 0x0, 0xffffffff)
  // Data:
  SEG64USER(SEG_W, 0x0, 0xffffffff)

gdt64_reg:
.word 0x37 
.quad gdt64_table

.section .bss
.balign 0x1000
.comm bootstack, KSTACKSIZE
bootstacktop:
.byte 0x0