/*
  Code working with CMOS
  To read a byte from CMOS, do an OUT 70H,addr followed by IN 71H.  To
  write a byte to CMOS, do an OUT 70H,addr followed by OUT 71H,value.
*/
/* Documentation at http://webpages.charter.net/danrollins/techhelp/0060.HTM */

#include <kern/cmos.h>
#include <inc/x86.h>

char cmos_read(char reg){
  x86_outb(CMOS_PORT1, reg);
  return x86_inb(CMOS_PORT2);
}

void cmos_write(char addr, char val){
  x86_outb(CMOS_PORT1, addr);
  x86_outb(CMOS_PORT2, val);
}
