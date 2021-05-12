#include <inc/types.h>
#include <inc/elf.h>
#include <inc/x86.h>

#define SECSIZE 512

struct boot_config_t{
  uint16_t mmap_addr;
  uint32_t mmap_cnt;
  uint32_t elfhdr_ptr;
};

// Everything is staticly inlined here because of we have to fit everything into 512 bytes :)

typedef void (*entry_func)(void);

static inline void memset(void* dest, char f, int n){
  for(;n >= 0; --n)
    *((char*)dest + n) = f;
};

static inline void readsect(uint32_t , uint32_t);
void readseg(uint32_t , uint32_t, uint32_t);

void bootmain(struct boot_config_t* boot_cfg){
  struct elf_hdr *elf;
  struct elf_proghdr *ph;
  uint32_t pa;
  entry_func entry;
  extern char boot2_end;
  extern char boot2_start;
  int offset = ((&boot2_end - &boot2_start) >> 9) + 1; // first sector is boot1, the second is variable size
  elf = (struct elf_hdr*)0x00010000; // we locate kernel elfhdr here

  readseg((uint32_t)elf, 4096, offset);
  if(elf->e_magic != ELF_MAGIC)
    return; // Cannot boot kernel;
  for(ph = (struct elf_proghdr*)((void*)elf + elf->e_phoff); ph < ((struct elf_proghdr*)((void*)elf + elf->e_phoff + elf->e_phnum*elf->e_phentsize)); ++ph){
    pa = ph->p_pa;
    // pa -= ph->p_offset - (ph->p_offset / SECSIZE) * SECSIZE; // Rounddown pa to be aligned by SECSIZE
    readseg((uint32_t)pa, ph->p_filesz, ph->p_offset / SECSIZE + offset);
    if(ph->p_memsz > ph->p_filesz)
      memset((void*)pa + ph->p_filesz, 0x0, ph->p_memsz - ph->p_filesz);
  }
  boot_cfg->elfhdr_ptr = (uint32_t)elf;
  entry = (entry_func)((uint32_t)elf->e_entry);
  // entry();
  __asm__ __volatile__("\tjmp *%1\n" : : "b"(boot_cfg), "r"(entry));
}

void readseg(uint32_t buf, uint32_t size, uint32_t offset){
  size = size / SECSIZE + 1;
  for(uint32_t block_i = 0; block_i < size; ++block_i)
    readsect(buf + block_i * SECSIZE, offset + block_i);
}

static inline void readsect(uint32_t buf, uint32_t offset){
  // https://wiki.osdev.org/ATA_PIO_Mode
  while((x86_inb(0x1F7) & 0xC0) != 0x40){__asm__ volatile ("pause");};
  x86_outb(0x1F2, 0x1); // one sector
  x86_outb(0x1F3, (char)offset);
  x86_outb(0x1F4, (char)(offset >> 8));
  x86_outb(0x1F5, (char)(offset >> 16));
  x86_outb(0x1F6, (char)(0xE0 | ((offset >> 24) & 0xf) | (0 << 4))); // drive #0
  x86_outb(0x1F7, 0x20); // command to read
  while((x86_inb(0x1F7) & 0xC0) != 0x40){__asm__ volatile ("pause");};
  x86_insl((void*)buf, SECSIZE / 4, 0x1F0);
};
