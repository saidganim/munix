
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
   # Enable memory > 1MB addressable memory - Legacy shit, who ever used 1MB of RAM ? :)
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
spin:
   hlt
   jmp spin


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
