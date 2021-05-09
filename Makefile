
-include boot/Makefile
-include kern/Makefile

INC=./

INCDIRS= -I$(INC)

CFLAGS = -m32 -nostdlib -nostdinc  $(INCDIRS) -fno-builtin -fno-inline -MD -std=gnu11 -fno-stack-protector \
             -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -nostdinc -O3 -fno-omit-frame-pointer

all: build qemu

build: obj/kernel.img

qemu: obj/kernel.img
	qemu-system-i386 -serial mon:stdio -d cpu_reset -D /dev/stdout -hda obj/kernel.img  -m 1024

qemu-gdb:
	qemu-system-i386 -S -gdb tcp::1234 -serial mon:stdio -d cpu_reset -D /dev/stdout -hda obj/kernel.img  -m 1024

qemu-arm: obj/boot.elf obj/kernel.img
	qemu-system-aarch64 -machine type=virt -cpu cortex-a53 -serial mon:stdio -d cpu_reset -D /dev/stdout -hda obj/kernel.img  -m 1024

obj/kernel.img: obj/boot.elf obj/kernel
	$(shell dd if=/dev/zero of=obj/kernel.img~ count=10000 2>/dev/null)
	$(shell dd if=obj/boot.elf of=obj/kernel.img~ conv=notrunc 2>/dev/null)
	$(shell cat obj/kernel >> obj/kernel.img~ 2>/dev/null)
	$(shell mv obj/kernel.img~ obj/kernel.img)

clean:
	rm -rf obj/*
