

obj/kernel: obj/entrypgdir.o obj/kmain.o obj/display.o obj/entry.o obj/pmap.o
	$(LD) -N $^ -o $@ -T kern/kernel.ld

obj/entry.o: kern/entry.S
	$(CC) $(CFLAGS) -c $^ -o $@

obj/entrypgdir.o : kern/entrypgdir.c
	$(CC) $(CFLAGS) -c $^ -o $@

obj/kmain.o: kern/kmain.c
	$(CC) $(CFLAGS) -c $^ -o $@

obj/display.o: kern/display.c
	$(CC) $(CFLAGS) -c $^ -o $@

obj/pmap.o: kern/pmap.c
	$(CC) $(CFLAGS) -c $^ -o $@
