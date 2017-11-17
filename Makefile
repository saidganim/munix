
-include boot/Makefile
-include kern/Makefile

INC=./

INCDIRS= -I$(INC)

CFLAGS = -m32 -nostdlib -nostdinc  $(INCDIRS) -fno-builtin -fno-inline -MD -std=gnu11 -fno-stack-protector \
             -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -nostdinc -O1 -fno-omit-frame-pointer -g3

build: obj/kernel.img

qemu: obj/boot.elf obj/kernel.img
	qemu-system-i386 -serial mon:stdio -d cpu_reset -D /dev/stdout -hda obj/kernel.img  -m 256

obj/kernel.img: obj/boot.elf
	$(shell dd if=/dev/zero of=obj/kernel.img~ count=10000 2>/dev/null)
	$(shell dd if=obj/boot.elf of=obj/kernel.img~ conv=notrunc 2>/dev/null)
	$(shell mv obj/kernel.img~ obj/kernel.img)

clean:
	$(shell rm obj/*)
