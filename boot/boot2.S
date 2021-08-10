#include <inc/mmu.h>
.text
.code16
.globl boot2
boot2:
# at this moment, nothing is really given yet,
# we need to enable A20 gate
# we need to get memory map
# we need to jump to 32-bit
# setup init data structures, stack and jump to C
enable_a20:
   # A20 gate: Enable memory > 1MB addressable memory - Legacy shit, who ever used 1MB of RAM ? :)
   # https://wiki.osdev.org/A20_Line
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

# get memory map - http://www.ctyme.com/intr/rb-1741.htm
e820h_memory_map:
    movl $0x0, %ebx
    movl $0x500, %esi // we keep the memory map buffers here, should be safe
e820h_iter:
    movw $memory_map_addr_buf, %di // some BIOSes may require to have %di the same in the sequence of int calls
    movl $0xe820, %eax
    movl $0x534D4150, %edx
    movl $20, %ecx
    int $0x15
    jc e820_done //  see boot in linux kernel. sometimes CF is raised when mem map enum is done
    incl memory_map_cnt
    test %ebx, %ebx
    jz e820_done
    pushl %ecx //  for future
    pushl %ebx //  for future
    pushl %edi // will use this reg now
    addw %cx, %si
    subw $1, %si
    movl $memory_map_addr_buf, %edi
    addw %cx, %di
    subw $1, %di
mv_byte:
    movb (%edi), %bl
    movb %bl, (%si)
    decw %si
    decw %di
    decw %cx
    jnz mv_byte
mv_byte_done:
    popl %edi
    popl %ebx
    popl %ecx
    addl %ecx, %esi
    jmp e820h_iter
e820_done: # here memory_map_cnt has number of memory maps, memory_map_addr has the beginning of the buffer

// enter protmode
    movl %cr0, %eax
    orl $CR0_PE, %eax
    movl %eax, %cr0
    lgdt gdtreg
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ljmp $0x08, $protmode 
.code32
protmode:
# we are still in 32 bit mode
# in *those* times function arguments were passed via stack :)
pushl $boot_conf 
call bootmain


spin:
   hlt
   jmp spin


.align 8
gdt_table:
  SEG_NULL;
  // Code segment(SEG_W here means readable executable segment, read Intel manual)
  SEG(SEG_X | SEG_W, 0x0, 0xffffffff)
  // Data segment
  SEG(SEG_W, 0x0, 0xffffffff)

gdtreg:
  .word . - gdt_table - 1
  .long gdt_table

.align 8
memory_map_addr_buf:
.byte 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0
boot_conf:
memory_map_addr:
.quad 0x500 // we just hope that stack pointer will not fuck this up
memory_map_cnt:
.long 0x0
elfhdr_ptr:
.long 0x0

