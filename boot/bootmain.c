#include <inc/types.h>
#include <inc/elf.h>
#include <inc/x86.h>

#define SECSIZE 512

typedef void (*entry_func)(void);

void memset(void* dest, char f, int n){
  for(;n >= 0; --n)
    *((char*)dest + n) = f;
};

void readsect(char*, uint32_t);
void readseg(char*, uint32_t, uint32_t);

void bootmain(){
  struct elf_hdr *elf;
  struct elf_proghdr *ph;
  physaddr_t pa;
  entry_func entry;
  elf = (struct elf_hdr*)0x00010000; // Kernel starts from 1MB
  readseg((char*)elf, 4096, 1);
  if(elf->e_magic != ELF_MAGIC)
    return; // Cannot bootstrap kernel;
  for(ph = (struct elf_proghdr*)((void*)elf + elf->e_phoff); ph < ((struct elf_proghdr*)((void*)elf + elf->e_phoff + elf->e_phnum)); ++ph){
    pa = ph->p_pa;
    pa -= ph->p_offset - (ph->p_offset / SECSIZE) * SECSIZE; // Rounddown pa to be aligned by SECSIZE
    readseg((char*)pa, ph->p_filesz, ph->p_offset / SECSIZE + 1);
    // if(ph->p_memsz > ph->p_filesz)
    //   memset((void*)pa + ph->p_filesz, 0x0, ph->p_memsz - ph->p_filesz);
  }
  entry = (entry_func)elf->e_entry;
  //entry += 1;
  entry();
}

// int disk_avail(){
//   return (x86_inb(0x1F7) & 0x08);
// }

void readseg(char *buf, uint32_t size, uint32_t offset){
  size = size / SECSIZE + 1;
  for(uint32_t block_i = 0; block_i < size; ++block_i)
    readsect(buf + block_i * SECSIZE, offset + block_i);
}

void readsect(char *buf, uint32_t offset){
  x86_outb(0x1F2, 0x1);
  x86_outb(0x1F3, (char)offset);
  x86_outb(0x1F4, (char)(offset >> 8));
  x86_outb(0x1F5, (char)(offset >> 16));
  x86_outb(0x1F6, (char)(0xE0 | ((offset >> 24) & 0xf) | (0 << 4))); // drive #0
  x86_outb(0x1F7, 0x20); // command to read
  while(!(x86_inb(0x1F7) & 0x08))/* Do nothing */;
  x86_insl(buf, SECSIZE / 4, 0x1F0);
};
