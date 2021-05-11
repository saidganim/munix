OUTPUT_FORMAT(elf64-x86-64)

ENTRY(_start_kernel_)

SECTIONS
{
  . = 0xc0100000;
  .text 0xc0100000 : AT(0x0100000){ *(.text) }
  .data : { *(.data) }
  .bss : { *(.bss) }
  PROVIDE(tend = .);
}
