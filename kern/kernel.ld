OUTPUT_FORMAT("elf32-i386", "elf32-i386",
          "elf32-i386")
OUTPUT_ARCH(i386)

ENTRY(kmain)

SECTIONS
{
  . = 0x0100000;
  .text : AT(0x100000){ *(.text) }
  .data : { *(.data) }
  .bss : { *(.bss) }
}
