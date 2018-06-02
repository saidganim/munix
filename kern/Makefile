

obj/kernel: obj/entrypgdir.o obj/kmain.o obj/display.o obj/entry.o obj/pmap.o obj/memory.o obj/cmos.o obj/utils.o
	$(LD) -N $^ -o $@ -T kern/kernel.ld

obj/entry.o: kern/entry.S
	$(CC) $(CFLAGS) -c $^ -o $@

obj/memory.o: kern/memory.S
	$(CC) $(CFLAGS) -c $^ -o $@

obj/entrypgdir.o : kern/entrypgdir.c
	$(CC) $(CFLAGS) -c $^ -o $@

obj/kmain.o: kern/kmain.c
	$(CC) $(CFLAGS) -c $^ -o $@

obj/display.o: kern/display.c
	$(CC) $(CFLAGS) -c $^ -o $@

obj/pmap.o: kern/pmap.c
	$(CC) $(CFLAGS) -c $^ -o $@

obj/cmos.o: kern/cmos.c
	$(CC) $(CFLAGS) -c $^ -o $@

obj/utils.o: kern/utils.c
	$(CC) $(CFLAGS) -c $^ -o $@
