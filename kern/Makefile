
LDFLAGS := -nostdlib -n -static -fno-pie -N -Wl,-Tkern/kernel.ld

obj/kernel: obj/entrypgdir.o obj/kmain.o obj/display.o obj/entry.o obj/pmap.o obj/memory.o obj/cmos.o obj/utils.o
	$(CC) $(LDFLAGS) $(CFLAGS) $^ -o $@ 

obj/entry.o: kern/entry.S
	$(CC) -mcmodel=large  $(CFLAGS) -gdwarf-2 -fno-pie -c $^ -o $@

obj/memory.o: kern/memory.S
	$(CC) $(CFLAGS) -fno-pie -mcmodel=large -c $^ -o $@

obj/entrypgdir.o : kern/entrypgdir.c
	$(CC) $(CFLAGS) -fno-pie -mcmodel=large -c $^ -o $@

obj/kmain.o: kern/kmain.c
	$(CC) $(CFLAGS) -fno-pie -mcmodel=large -c $^ -o $@

obj/display.o: kern/display.c
	$(CC) $(CFLAGS) -fno-pie -mcmodel=large -c $^ -o $@

obj/pmap.o: kern/pmap.c
	$(CC) $(CFLAGS) -fno-pie -mcmodel=large -c $^ -o $@

obj/cmos.o: kern/cmos.c
	$(CC) $(CFLAGS) -fno-pie -mcmodel=large -c $^ -o $@

obj/utils.o: kern/utils.c
	$(CC) $(CFLAGS) -fno-pie -mcmodel=large -c $^ -o $@
