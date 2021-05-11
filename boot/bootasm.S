#include <inc/mmu.h>

.code16
.text
.global start
start:
  cli
  cld
   xorw %ax, %ax
   movw %ax, %ds
   movw %ax, %es
   movw %ax, %ss
   # setting the stack pointer
   movw $start, %sp
   # Number of the drive we booted from is stored in %dl, we need it later
   movb %dl, boot_drive_number
  

  movw $0x7E00, %di
  movw $boot2_end, %si
  subw $boot2_start, %si
  shrw $9, %si
  call disk_read
  call boot2
spin:
  hlt;
  jmp spin;

# %si - number of blocks; %di - address of buffer
disk_read:
    movb boot_drive_number, %dl
    movw $int13_disk_addr_packet, %bp
    movw %si, 2(%bp) # number of blocks
    movw %di, 4(%bp) # address of the "buffer"
    movw $int13_disk_addr_packet, %si
    movb $0x42, %ah
    int $0x13 # http://www.ctyme.com/intr/rb-0708.htm - EXTENDED READ
    jc disk_error
    ret

disk_error:
  jmp spin

int13_disk_addr_packet:
.byte 0x10
.byte 0
.word 0x1
.long 0x0
.quad 0x1

.globl boot_drive_number
boot_drive_number:
.byte 0