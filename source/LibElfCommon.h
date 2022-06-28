#pragma once

#include "BaseDefs.h"

#include <stddef.h>
#include <stdint.h>

IU_EXTERN_C_BEGIN

// ELF32/ELF64 shared types
typedef uint16_t Iu_ElfHalf;
typedef uint32_t Iu_ElfWord;
typedef uint32_t Iu_ElfSword;

// ELF32 specific types
typedef uint32_t Iu_Elf32Addr;
typedef uint32_t Iu_Elf32Offset;

// ELF64 specific types
typedef uint64_t Iu_ElfXword;
typedef uint64_t Iu_ElfSxword;
typedef uint64_t Iu_Elf64Addr;
typedef uint64_t Iu_Elf64Offset;

// Object file classes (ELF64 table 3)
// Type: unsigned char
enum {
    Iu_ElfObjectFileClass_32,
    Iu_ElfObjectFileClass_64,
};

// Data encodings (ELF64 table 4)
// Type: unsigned char
enum {
    Iu_ElfDataEncoding_Lsb = 1,
    Iu_ElfDataEncoding_Msb = 2,
};

// Operating system and ABI identifiers (ELF64 table 5)
// Type: unsigned char
enum {
    Iu_ElfOsAbi_SysV,
    Iu_ElfOsAbi_HPUX,
    Iu_ElfOsAbi_Standalone,
};

// Object file types (ELF64 table 6)
// Type: Iu_ElfHalf
enum {
    Iu_ElfObjectFileType_None = 0, // No file type
    Iu_ElfObjectFileType_Relocatable = 1, // Relocatable object file
    Iu_ElfObjectFileType_Executable = 2, // Executable file
    Iu_ElfObjectFileType_Dynamic = 3, // Shared object file
    Iu_ElfObjectFileType_Core = 4, // Core file
    Iu_ElfObjectFileType_LoOS = 0xFE00, // Environment-specific use
    Iu_ElfObjectFileType_HiOS = 0xFEFF,
    Iu_ElfObjectFileType_LoProc = 0xFF00, // Processor-specific use
    Iu_ElfObjectFileType_HiProc = 0xFFFF,
};

// Special section indices (ELF32 figure 1-7, ELF64 table 7)
// Type: Iu_ElfHalf
enum {
    Iu_ElfSsiUndef = 0, // Undefined or meaningless
    
    Iu_ElfSsiLoProc = 0xff00, // Processor-specific use
    Iu_ElfSsiHiProc = 0xff1f,
    Iu_ElfSsiLoOS = 0xff20, // Environment-specific use
    Iu_ElfSsiHiOS = 0xff3f,
    
    Iu_ElfSsiAbs = 0xfff1, // Reference is an absolute value
    Iu_ElfSsiCommon = 0xfff2, // Symbol declared as a common block
};

// Section types (ELF64 table 8)
// Type: Iu_ElfWord
enum {
    Iu_ElfSectionType_Null = 0, // Marks an unseen section header
    Iu_ElfSectionType_Progbits = 1, // Contains information defined by the program
    Iu_ElfSectionType_SymbolTable = 2, // Contains a linker symbol table
    Iu_ElfSectionType_StringTable = 3, // Contains a string table
    Iu_ElfSectionType_Rela = 4, // Contains "Rela" type relocation entries
    Iu_ElfSectionType_Hash = 5, // Contains a symbol hash table
    Iu_ElfSectionType_Dynamic = 6, // Contains dynamic linking tables
    Iu_ElfSectionType_Note = 7, // Contains note information
    Iu_ElfSectionType_Nobits = 8, // Contains uninitialized space; does not occupy any space in the file
    Iu_ElfSectionType_Rel = 9, // Contains "Rel" type relocation entries
    Iu_ElfSectionType_Shlib = 10, // Reserved
    Iu_ElfSectionType_Dynsym = 11, // Contains a dynamic loader symbol table
    Iu_ElfSectionType_LoOS = 0x60000000, // Environment-specific use
    Iu_ElfSectionType_HiOS = 0x6FFFFFFF,
    Iu_ElfSectionType_LoProc = 0x70000000, // Processor-specific use
    Iu_ElfSectionType_HiProc = 0x7FFFFFFF,
};

// Section attributes (ELF64 table 9).
// Type: Iu_ElfWord on ELF32
//       Iu_ElfXword on ELF64
enum {
    Iu_ElfSectionAttribute_Write = 0x1, // Section contains writable data
    Iu_ElfSectionAttribute_Alloc = 0x2, // Section is allocated in memory image of program
    Iu_ElfSectionAttribute_ExecInstr = 0x4, // Section contains executable instructions
    Iu_ElfSectionAttribute_MaskOS = 0x0F000000, // Environment-specific use
    Iu_ElfSectionAttribute_MaskProc = 0xF0000000, // Processor-specific use
};

typedef enum {
    Iu_ElfLoaderError_Success,
    Iu_ElfLoaderError_SectionTypeMismatch,
} Iu_ElfLoaderError;

IU_EXTERN_C_END
