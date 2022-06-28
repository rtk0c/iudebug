#pragma once

#include "BaseDefs.h"
#include "LibElfCommon.h"

#include <stddef.h>

IU_EXTERN_C_BEGIN

//~ ELF64 specification data structures

// ELF header (ELF64 figure 2)
typedef struct {
    unsigned char ei_magic[4];
    unsigned char ei_class; // Iu_ElfObjectFileClass
    unsigned char ei_data; // Iu_ElfDataEncoding
    unsigned char ei_version;
    unsigned char ei_osabi; // Iu_ElfOsAbi
    unsigned char ei_abiversion;
    unsigned char ei_pad[7];
    
    Iu_ElfHalf et; // Object file type, Iu_ObjectFileType
    Iu_ElfHalf machine; // Machine type
    Iu_ElfWord version; // Object file version
    Iu_Elf64Addr entry; // Entry point address
    Iu_Elf64Offset  phoff; // Program header offset
    Iu_Elf64Offset  shoff; // Section header offset
    Iu_ElfWord flags; // Processor-specific flags
    Iu_ElfHalf ehsize; // ELF header size
    Iu_ElfHalf phentsize; // Size of program header entry
    Iu_ElfHalf phnum; // Number of program header entries
    Iu_ElfHalf shentsize; // Size of section header entry
    Iu_ElfHalf shnum; // Number of section header entries
    Iu_ElfHalf shstrndx; // Section name string table index
} Iu_Elf64Header;

// Section header (ELF64 figure 3)
typedef struct {
    Iu_ElfWord name; // Section name
    Iu_ElfWord type; // Section type
    Iu_ElfXword flags; // Section flags
    Iu_Elf64Addr addr; // Virtual address in memory
    Iu_Elf64Offset offset; // Offset in file
    Iu_ElfXword size; // Size of section
    Iu_ElfWord link; // Link to other section
    Iu_ElfWord info; // Misc information
    Iu_ElfXword addralign; // Addresss alignment
    Iu_ElfXword entsize; // Size of entries
} Iu_Elf64SectionHeader;

// Symbol table (ELF64 figure 4)
typedef struct {
    Iu_ElfWord name; // Symbol name (strtab offset)
    unsigned char info; // Type and binding attributes
    unsigned char other; // Reserved
    Iu_ElfHalf shnxd; // Section table index
    Iu_Elf64Addr value; // Symbol value (address)
    Iu_ElfXword size; // Size of object
} Iu_Elf64SymbolTable;

// Relocation entries (ELF64 figure 5)
typedef struct {
    Iu_Elf64Addr r_offset; // Address of reference
    Iu_ElfXword r_info; //  Symbol index and type of relocation
} Iu_Elf64Rel;
typedef struct {
    Iu_Elf64Addr r_offset; // Address of reference
    Iu_ElfXword r_info; // Symbol index and type of relocation
    Iu_ElfXword r_addend; // Constant part of expression
} Iu_Elf64Rela;

// Program header (ELF64 figure 6)
typedef struct {
    Iu_ElfWord type;
    Iu_ElfWord flags;
    Iu_Elf64Offset offset;
    Iu_Elf64Addr vaddr;
    Iu_Elf64Addr paddr;
    Iu_ElfXword filesz;
    Iu_ElfXword memsz;
    Iu_ElfXword aligne;
} Iu_Elf64ProgramHeader;

//~ API data structures

typedef struct {
    Iu_Elf64SectionHeader header;
    
    const char* name;
    size_t nameLen;
    
    const void* data;
    size_t dataLen;
} Iu_Elf64Section;

typedef struct {
} Iu_Elf64Segment;

//~ Main API struct

typedef struct {
    void* loader;
    Iu_Elf64Section* sections;
    Iu_Elf64Segment* segments;
    size_t segmentCount;
    size_t sectionCount;
    int loaderTag;
} Iu_Elf64;

void Iu_InitElf64(Iu_Elf64* data);

void Iu_CreateElf64FromMmapLoader(Iu_Elf64* data, int fd);
void Iu_DestroyElf64(Iu_Elf64* data);

IU_EXTERN_C_END
