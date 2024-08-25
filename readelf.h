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

enum RETURN {EOK, ENOFIL, ENOARG, ENOMAG, ENOCLS, ENODAT, ENOVER, ENOABI, ENOABV, ENOPAD};

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

int verify_e_ident(unsigned char *buffer);
int read_elf_header();
