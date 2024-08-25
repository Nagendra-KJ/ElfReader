#include "readelf.h"
#include <stdio.h>

FILE *g_elf_file;
Elf64_Ehdr g_elf_header;

int main (int argc, char **argv)
{
    if (argc != 2)
    {
        eprintf(-ENOARG);
        return -ENOARG;
    }
    const char *elf_file_name = argv[1];
    g_elf_file = fopen(elf_file_name, "rb");
    if (!g_elf_file)
    {
        eprintf(-ENOFIL);
        return -ENOFIL;
    }

    return read_elf_header();
}

int read_elf_header()
{
    int retval;
    fread(&g_elf_header, sizeof(char), sizeof(g_elf_header), g_elf_file);
    retval = verify_e_ident(g_elf_header.e_ident);
    
    if (retval != EOK)
    {
        eprintf(retval);
        return retval;
    }
   
    printf("Type: %d\n", g_elf_header.e_type);
    printf("Machine: %x\n", g_elf_header.e_machine);
    
    if (g_elf_header.e_version != EI_VERSION)
        return -ENOVER;

    printf("Entry: %llx\n", g_elf_header.e_entry);
    printf("Program Header Offset: %lld\n", g_elf_header.e_phoff);
    printf("Section Header Offset: %lld\n", g_elf_header.e_shoff);
    printf("Flags: %x\n", g_elf_header.e_flags);
    printf("Elf Header Size: %d\n", g_elf_header.e_ehsize);
    printf("Program Header Entry Size: %d\n", g_elf_header.e_phentsize);
    printf("Program Header Entries Count: %d\n", g_elf_header.e_phnum);
    printf("Section Header Entry Size: %d\n", g_elf_header.e_shentsize);
    printf("Section Header Entry Count: %d\n", g_elf_header.e_shnum);
    printf("String Table Index: %d\n", g_elf_header.e_shstrndx);

    return retval;
}

int verify_e_ident(unsigned char *buffer)
{
    if (buffer[0] != ELFMAG0 || buffer[1] != ELFMAG1 || buffer[2] != ELFMAG2 || buffer[3] != ELFMAG3)
        return -ENOMAG;
    if (buffer[4] != CLASS64)
        return -ENOCLS;
    if (buffer[5] != DATALSB)
        return -ENODAT;
    if (buffer[6] != EI_VERSION)
        return -ENOVER;
    if (buffer[7] != EI_OSABI)
        return -ENOABI;
    if (buffer[8] != EI_ABIVERSION)
        return -ENOABV;

    for (int i = 9; i < EI_NIDENT; ++i)
        if (buffer[i] != EI_PAD)
            return -ENOPAD;
    return EOK;
}
