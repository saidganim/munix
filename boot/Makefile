


obj/boot.elf: obj/boot.elf_
	$(shell objdump  -S obj/boot.elf_ > obj/boot.elf.asm)
	$(shell objcopy -S -O binary -j .text obj/boot.elf_ obj/boot.elf)
	$(shell boot/make_boot.py obj/boot.elf)

obj/boot.elf_: obj/bootasm.o obj/bootmain.o obj/display.o
	$(LD) -N $^ -o $@ -T boot/boot.ld

obj/display.o: kern/display.c
	$(CC) -c $(CFLAGS) $< -o $@

obj/bootasm.o: boot/bootasm.S
	$(CC) $(CFLAGS) -Os -c -o $@ $<

obj/bootmain.o: boot/bootmain.c
	$(CC) $(CFLAGS) -Os -c -o $@ $<
