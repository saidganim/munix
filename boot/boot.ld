OUTPUT_FORMAT("elf32-i386", "elf32-i386",
          "elf32-i386")
OUTPUT_ARCH(i386)

ENTRY(start)

SECTIONS
{
  . = 0x7C00;
  .boot 0x7c00 : AT(ADDR(.boot)) {
      obj/bootasm.o (.text)
      obj/bootasm.o (.rodata)
      obj/bootasm.o (.data)
      obj/bootasm.o (.bss)
  }
  . = 0x7E00;
  boot2_start = .;
  .text : {
      *(.text)
  }
  . = ALIGN(512);
  boot2_end = .;
}
