
-include boot/Makefile
-include kern/Makefile

INC=./

INCDIRS= -I$(INC)

CFLAGS = -g3 -mno-sse -fno-pie -static -nostdlib -nostdinc  $(INCDIRS) -fno-pie -fno-builtin -fno-inline -MD -std=gnu11 -fno-stack-protector \
             -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -nostdinc -fno-omit-frame-pointer

all: build qemu

build: obj obj/kernel.img

qemu: obj/kernel.img
	qemu-system-x86_64 -serial mon:stdio -d cpu_reset -D /dev/stdout -drive format=raw,file=obj/kernel.img  -m 1024

qemu-gdb:
	qemu-system-x86_64 -S -gdb tcp::1234 -serial mon:stdio -d cpu_reset -D /dev/stdout -drive format=raw,file=obj/kernel.img  -m 1024

qemu-arm: obj/boot.elf obj/kernel.img
	qemu-system-aarch64 -machine type=virt -cpu cortex-a53 -serial mon:stdio -d cpu_reset -D /dev/stdout -hda obj/kernel.img  -m 1024

obj/kernel.img: obj obj/boot.elf obj/kernel
	$(shell dd if=/dev/zero of=obj/kernel.img~ count=10000 2>/dev/null)
	$(shell dd if=obj/boot.elf of=obj/kernel.img~ conv=notrunc 2>/dev/null)
	$(shell dd if=obj/kernel of=obj/kernel.img~ conv=notrunc seek=3 2>/dev/null)
	$(shell mv obj/kernel.img~ obj/kernel.img)

obj:
	mkdir -p obj

clean:
	rm -rf obj/*
