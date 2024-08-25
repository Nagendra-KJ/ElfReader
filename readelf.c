#include "readelf.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

FILE *g_elf_file;
Elf64_Ehdr g_elf_header;
Elf64_Shdr *g_elf_shdrs;
Elf64_Xword g_numsh;
Elf64_Word g_shstrndx;
char *g_strtab;

int main (int argc, char **argv)
{
    int retval;
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

    retval = read_elf_header();
    if (retval != EOK)
    {
        return retval;
    }

    print_star();
    printf("Header\n");
    print_elf_header();

    if (g_elf_header.e_shoff == 0 && g_elf_header.e_shnum == 0)
    {
        printf("This ELF file does not contain a sectional header\n");
        return EOK;
    }

    if (g_elf_header.e_shoff != 0 && g_elf_header.e_shnum == 0)
    {
        Elf64_Shdr first_shdr;
        retval = read_elf_shdrs(g_elf_header.e_shentsize, 1, g_elf_header.e_shoff, &first_shdr);
        if (retval != EOK)
        {
            return retval;
        }
        g_numsh = first_shdr.sh_size;
        g_shstrndx = first_shdr.sh_link;
    }
    else
    {
        g_numsh = g_elf_header.e_shnum;
        g_shstrndx = g_elf_header.e_shstrndx;
    }

    g_elf_shdrs = (Elf64_Shdr *) malloc(g_elf_header.e_shentsize * g_numsh);

    retval = read_elf_shdrs(g_elf_header.e_shentsize, g_numsh, g_elf_header.e_shoff, g_elf_shdrs);
    if (retval != EOK)
    {
        return retval;
    }

    Elf64_Shdr strtab_shdr = g_elf_shdrs[g_shstrndx];
    g_strtab = (char *) malloc(sizeof(char) * strtab_shdr.sh_size);
    read_elf_section(strtab_shdr, g_strtab);

    for (Elf64_Xword i = 0; i < g_numsh; ++i)
    {
        Elf64_Shdr shdr = g_elf_shdrs[i];
        char buffer[shdr.sh_size];
        read_elf_section(shdr, buffer);
        print_star();
        printf("Section %lld\n", i);
        print_section(shdr.sh_name, buffer, shdr.sh_size);
    }
    return EOK;
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

    if (g_elf_header.e_version != EI_VERSION)
        return -ENOVER;

    return EOK;
}


int read_elf_shdrs(Elf64_Half ent_size, Elf64_Half num_ents, Elf64_Off offset, void *buffer)
{
    fseek(g_elf_file, offset, SEEK_SET);

    if (!buffer)
    {
        eprintf(-ENOMEM);
        return -ENOMEM;
    }

    fread(buffer, sizeof(char), ent_size * num_ents, g_elf_file);
    return EOK;
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

void print_section_header(Elf64_Shdr shdr)
{
    printf("Section Name Index: %d\n", shdr.sh_name);
    printf("Section Type: %d\n", shdr.sh_type);
    printf("Section Flags: %lld\n", shdr.sh_flags);
    printf("Section Address: %llx\n", shdr.sh_addr);
    printf("Section Offset: %lld\n", shdr.sh_offset);
    printf("Section Size: %lld\n", shdr.sh_size);
    printf("Section Link: %d\n", shdr.sh_link);
    printf("Section Info: %d\n", shdr.sh_info);
    printf("Section Address Alignment: %lld\n", shdr.sh_addralign);
    printf("Section Entry Size: %lld\n", shdr.sh_entsize);
}

void print_elf_header()
{
    printf("Type: %d\n", g_elf_header.e_type);
    printf("Machine: %02x\n", g_elf_header.e_machine);
    printf("Version: %d\n", g_elf_header.e_version);
    printf("Entry: %llx\n", g_elf_header.e_entry);
    printf("Program Header Offset: %lld\n", g_elf_header.e_phoff);
    printf("Section Header Offset: %lld\n", g_elf_header.e_shoff);
    printf("Flags: %x\n", g_elf_header.e_flags);
    printf("Elf Header Size: %d\n", g_elf_header.e_ehsize);
    printf("Program Header Entry Size: %d\n", g_elf_header.e_phentsize);
    printf("Program Header Entry Count: %d\n", g_elf_header.e_phnum);
    printf("Section Header Entry Size: %d\n", g_elf_header.e_shentsize);
    printf("Section Header Entry Count: %d\n", g_elf_header.e_shnum);
    printf("String Table Index: %d\n", g_elf_header.e_shstrndx);
}

void print_section(Elf64_Word name, void *buffer, Elf64_Xword size)
{
    char *data = (char *)buffer;
    
    printf("Name: %s\n", &g_strtab[name]);
    printf("Size: %lld\n", size);
    printf("Data:\n");

    for (Elf64_Xword i = 0; i < size; ++i)
    {
        if (isprint(data[i]))
            printf("%c ", data[i]);
        else
            printf("%02x ", data[i]);
    }
    printf("\n");
    return;
}

int read_elf_section(Elf64_Shdr shdr, void *buffer)
{
    int retval = EOK;
    Elf64_Off offset = shdr.sh_offset;
    Elf64_Xword size = shdr.sh_size;

    fseek(g_elf_file, offset, SEEK_SET);
    retval = fread(buffer, sizeof(char), size, g_elf_file);
    
    return retval == size ? EOK:ENOERR;
}
