// /* Detecting memory */


// .text
// .global bios_mem_size_15881
// .global bios_mem_size_15801


// /* DOESN'T WORK ON QEMU ( WHO KNOWS WHY )
// BIOS Function: INT 0x15, AX = 0xE801
// Typical Output:
// AX = CX = extended memory between 1M and 16M, in K (max 3C00h = 15MB)
// BX = DX = extended memory above 16M, in 64K blocks
// There are some BIOSes that always return with AX = BX = 0.
// Use the CX/DX pair in that case. Some other BIOSes will return CX = DX = 0.
// Linux initializes the CX/DX pair to 0 before the INT opcode, and then uses CX/DX,
// unless they are still 0 (when it will use AX/BX). In any case, it is best to do
// a sanity check on the values in the registers that you use before you trust
// the results.
// (GRUB just trusts AX/BX -- this is not good.)
// */
// bios_mem_size_15801:
//   xorw %cx, %cx
//   xorw %dx, %dx
//   xorl %ebx, %ebx
//   xorl %eax, %eax
//   movl $0xE801, %eax
//   int $0x15
//   jc .error
//   cmpb $0x86, %ah
//   je .error
//   cmpb $0x80, %ah
//   je .error
//   jcxz .axbx
//   movw %cx, %ax
//   movw %dx, %bx
//   jmp .axbx
// .axbx:
//   movl %eax, %ecx
//   movl %ebx, %eax
//   movl $64, %edx
//   mull %edx
//   addl %ecx, %eax
//   ret
// .error:
//   movl $0, %eax
//   ret
