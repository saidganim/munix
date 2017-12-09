// Everything about physical memory is here

#include <kern/pmap.h>
#include <inc/types.h>
#include <inc/x86.h>

/* DOESN'T WORK ON QEMU ( WHO KNOWS WHY )
BIOS Function: INT 0x15, AX = 0xE801
Typical Output:
AX = CX = extended memory between 1M and 16M, in K (max 3C00h = 15MB)
BX = DX = extended memory above 16M, in 64K blocks
There are some BIOSes that always return with AX = BX = 0.
Use the CX/DX pair in that case. Some other BIOSes will return CX = DX = 0.
Linux initializes the CX/DX pair to 0 before the INT opcode, and then uses CX/DX,
unless they are still 0 (when it will use AX/BX). In any case, it is best to do
a sanity check on the values in the registers that you use before you trust
the results.
(GRUB just trusts AX/BX -- this is not good.)
*/
uint32_t bios_mem_size_extended(){
    // returns RAM size in KB
    char ax = 0, bx = 0;
    goto unsupported_error; // REMOVE WHEN FIX PROBLEM WITH INT 0x15 on QEMU
    asm goto("\tmovw $0xE801, %%ax\n"
                     "\txorw %%cx, %%cx\n"
                     "\txorw %%dx, %%dx\n"
                     "\tint $0x15\n"
                     "\tjc %l0\n"
                     "\tcmpb $0x86, %%ah\n" // unsupported function
                     "\tje %l0\n"
                     "\tcmpb $0x80, %%ah\n" // invalid command
                     "\tje %l0\n"
                     "\tjcxz .axbx\n"
                     "\tmovw %%cx, %%ax\n"
                     "\tmovw %%dx, %%bx\n"
                     "\t.axbx:\n"
                     :
                     :
                     : "ax", "bx" , "cc", "memory"
                     : unsupported_error);

  return (ax) + (bx * 64);
unsupported_error:
  return 0;
}
