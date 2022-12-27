#include "stdio.h"
#include "string.h"
#include "elf.h"

void info_phdr(Elf32_Phdr phdr)
{
  printf("  rva = 0x%x\n"
         "  fov = 0x%x\n"
         "  fsz = 0x%x\n"
         "  msz = 0x%x\n",
         phdr.p_vaddr, phdr.p_offset, phdr.p_filesz, phdr.p_memsz);
}

void info_ehdr(Elf32_Ehdr ehdr)
{
  printf("ehdr:\n"
         "  entry = 0x%x\n"
         "  phnum = 0x%x\n"
         "  phoff = 0x%x\n"
         "  phentsize = 0x%x\n",
         ehdr.e_entry, ehdr.e_phnum, ehdr.e_phoff, ehdr.e_phentsize);
}

int main(int argc, char **argv)
{
  if (argc != 2)
    {
      return 0;
    }
  Elf32_Ehdr ehdr;
  Elf32_Phdr phdr;

  int fd = open(argv[1], O_RDWR);
  read(fd, &ehdr, sizeof(ehdr));

  info_ehdr(ehdr);

  for (int i = 0; i < ehdr.e_phnum; i++)
    {
      int phdr_fov = ehdr.e_phoff + i * ehdr.e_phentsize;
      lseek(fd, phdr_fov, SEEK_SET);
      read(fd, &phdr, ehdr.e_phentsize);

      printf("num: %d\n", i);
      info_phdr(phdr);
    }
  return 0;
}