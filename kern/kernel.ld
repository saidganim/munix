OUTPUT_FORMAT(elf64-x86-64)

ENTRY(_start_kernel_)

SECTIONS
{
  . = 0x100000;
  .bootreg 0x100000 : {
    obj/entry.o (.text)
    obj/entry.o (.bss)
    obj/entry.o (.data)

    obj/entrypgdir.o (.text)
    obj/entrypgdir.o (.bss)
    obj/entrypgdir.o (.data)
  }
  bootreg_end = .;
  . += 0xFFFF800000000000;
  .text (0xFFFF800000000000 + bootreg_end) : AT(ADDR(.text) - 0xFFFF800000000000 ) { *(.text) }
  .data : AT(ADDR(.data) - 0xFFFF800000000000 ) { *(.data) }
  .rodata : AT(ADDR(.rodata) - 0xFFFF800000000000) { *(.rodata) }
  .bss : AT(ADDR(.bss) - 0xFFFF800000000000 ) { *(.bss) }
  tend = .;
}
