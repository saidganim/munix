#include <inc/mmu.h>

.global start
start:
  .code16
  cli
  cld

   xorw %ax, %ax
   movw %ax, %ds
   movw %ax, %es
   movw %ax, %ss

   # Enable memory > 1MB (Legacy shit)
 wait_1:
   inb $0x64, %al
   testb $0x2, %al
   jnz wait_1

   movb $0xd1, %al
   outb %al, $0x64

 wait_2:
   inb $0x64, %al
   testb $0x2, %al
   jnz wait_2

   movb $0xdf, %al
   outb %al, $0x60


  # Detecting size of RAM
  movl $0xE801, %eax
  int $0x15
  jc spin
  cmpb $0x86, %ah
  je spin
  cmpb $0x80, %ah
  je spin
  jcxz .axbx
  movl %ecx, %eax
  movl %edx, %ebx
.axbx:
  movl %eax, %ecx
  movl %ebx, %eax
  movl $64, %edx
  mull %edx
  addl %ecx, %eax
  movl %eax, (0x6c00)


  // Jumping to protected mode
.protmod:
  lgdt gdtreg;
  movl %cr0, %eax
  orl $CR0_PE, %eax
  movl %eax, %cr0

  ljmp $MX_CS, $start_32


  .code32
start_32:
  movl $MX_DS, %eax
  movl %eax, %ds
  movl %eax, %ss
  movl %eax, %es
  movl %eax, %fs
  movl %eax, %gs

  movl $start, %esp // Setting up stack pointer
  call bootmain

spin:
  hlt;

.align 4
gdt_table:
  SEG_NULL;
  // Code segment(SEG_W here means readable executable segment, read Intel manual)
  SEG(SEG_X | SEG_W, 0x0, 0xffffffff)
  // Data segment
  SEG(SEG_W, 0x0, 0xffffffff)

gdtreg:
  .word . - gdt_table - 1
  .long gdt_table
