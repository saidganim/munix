
# CFLAGS += -g3 -m32 -melf_i386
obj/boot.elf: obj/boot.elf_
	# $(shell objdump  -S obj/boot.elf_ > obj/boot.elf.asm)
	$(shell objcopy -S -O binary -j .boot obj/boot.elf_ obj/boot.elf)
	$(shell objcopy -S -O binary -j .text obj/boot.elf_ obj/boot2.elf)
	$(shell boot/make_boot.py obj/boot.elf obj/boot2.elf)

obj/boot.elf_: obj/bootasm.o obj/boot2.o obj/bootmain.o
	$(LD) -N -melf_i386 $^ -o $@ -T boot/boot.ld

obj/bootasm.o: boot/bootasm.S
	$(CC) -m32 $(CFLAGS) -Os -c -o $@ $^

obj/boot2.o: boot/boot2.S
	$(CC) -m32 $(CFLAGS) -Os -c -o $@ $^

obj/bootmain.o: boot/bootmain.c
	$(CC) -m32 -g3 -fno-pie $(CFLAGS) -Os -c -o $@ $^
