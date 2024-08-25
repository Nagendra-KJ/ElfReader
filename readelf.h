#include <stdint.h>
#include "utils.h"

#define EI_NIDENT 16

typedef uint64_t Elf64_Addr;
typedef uint64_t Elf64_Off;
typedef uint16_t Elf64_Half;
typedef uint32_t Elf64_Word;
typedef int32_t Elf64_Sword;
typedef uint64_t Elf64_Xword;
typedef int64_t Elf64_Sxword;

#define ELFMAG0 0x7f
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'

#define EI_VERSION 1
#define EI_OSABI 0
#define EI_ABIVERSION 0
#define EI_PAD 0

enum ELFCLASS {CLASSNONE, CLASS32, CLASS64};
enum EIDATA {DATANONE, DATALSB, DATAMSB};

enum RETURN {EOK, ENOERR, ENOFIL, ENOARG, ENOMEM, ENOMAG, ENOCLS, ENODAT, ENOVER, ENOABI, ENOABV, ENOPAD};

enum SHT {SHT_NULL, SHT_STRTAB=3};

typedef struct {
    unsigned char e_ident[EI_NIDENT];
    Elf64_Half e_type;
    Elf64_Half e_machine;
    Elf64_Word e_version;
    Elf64_Addr e_entry;
    Elf64_Off e_phoff;
    Elf64_Off e_shoff;
    Elf64_Word e_flags;
    Elf64_Half e_ehsize;
    Elf64_Half e_phentsize;
    Elf64_Half e_phnum;
    Elf64_Half e_shentsize;
    Elf64_Half e_shnum;
    Elf64_Half e_shstrndx;
} Elf64_Ehdr;

typedef struct {
    Elf64_Word sh_name;
    Elf64_Word sh_type;
    Elf64_Xword sh_flags;
    Elf64_Addr sh_addr;
    Elf64_Off sh_offset;
    Elf64_Xword sh_size;
    Elf64_Word sh_link;
    Elf64_Word sh_info;
    Elf64_Xword sh_addralign;
    Elf64_Xword sh_entsize;
} Elf64_Shdr;

int verify_e_ident(unsigned char *buffer);
int read_elf_header();
int read_elf_shdrs(Elf64_Half ent_size, Elf64_Half num_ents, Elf64_Off offset, void *buffer);
void print_section(Elf64_Word name, void *buffer, Elf64_Xword size);
void print_section_header(Elf64_Shdr shdr);
void print_elf_header();
int read_elf_section(Elf64_Shdr shdr, void *buffer);
