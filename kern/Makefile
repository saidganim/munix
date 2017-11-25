

obj/kernel: kern/kmain.o kern/display.o
	$(LD) -N $^ -o $@ -T kern/kernel.ld

kern/kmain.o: kern/kmain.c
	$(CC) $(CFLAGS) -c $^ -o $@

kern/display.o: kern/display.c
	$(CC) $(CFLAGS) -c $^ -o $@
