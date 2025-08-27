#include "elf.h"
#include "../../cliutils.h"

#define PAGE_SIZE 4096

#define EI_MAG0 0
#define EI_MAG1 1
#define EI_MAG2 2
#define EI_MAG3 3
#define ELFMAG0 0x7F
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'

#define ET_EXEC 2
#define ET_DYN  3
#define EM_386  3
#define PT_LOAD 1
#define PF_X    0x1
#define PF_W    0x2
#define PF_R    0x4


typedef struct {
    unsigned char e_ident[16];
    uint16_t      e_type;
    uint16_t      e_machine;
    uint32_t      e_version;
    uint32_t      e_entry;
    uint32_t      e_phoff;
    uint32_t      e_shoff;
    uint32_t      e_flags;
    uint16_t      e_ehsize;
    uint16_t      e_phentsize;
    uint16_t      e_phnum;
    uint16_t      e_shentsize;
    uint16_t      e_shnum;
    uint16_t      e_shstrndx;
} Elf32_Header;


typedef struct {
    uint32_t p_type;
    uint32_t p_offset;
    uint32_t p_vaddr;
    uint32_t p_paddr;
    uint32_t p_filesz;
    uint32_t p_memsz;
    uint32_t p_flags;
    uint32_t p_align;
} Elf32_ProgramHeader;






static inline uint32_t page_align_down(uint32_t x){ return x & ~(PAGE_SIZE-1); }
static inline uint32_t page_align_up  (uint32_t x){ return (x + PAGE_SIZE-1) & ~(PAGE_SIZE-1); }

static void memset(void *ptr, int value, size_t num){
    unsigned char *p = ptr;
    while (num--) {
        *p++ = value;
    }
}

static void memcpy(void *dest, const void *src, size_t n){
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (n--) {
        *d++ = *s++;
    }
}

static void map_zero_pages_range(uint32_t va, uint32_t len){
    uint32_t start = page_align_down(va);
    uint32_t end   = page_align_up(va + len);

    _sys_pmm_reserve_range((void*)start, (void*)end);
    memset((void*)start, 0, end - start);
}

static void copy_from_blob(uint32_t dst_va, const uint8_t* blob, uint32_t blob_size, uint32_t off, uint32_t n)
{
    if (!n) return;
    if (off > blob_size || blob_size - off < n){
        cli_error("elf_loader: blob read OOB (off=%u, n=%u, size=%u)", off, n, blob_size);
        return;
    }
    memcpy((void*)dst_va, blob + off, n);
}






/*
    ,--------------------,
    ; Exported functions ;
    '--------------------'
*/



int _sys_load_module_app(uint32_t mod_start, uint32_t mod_end, const char* cmdline, void (**entry_out)(void)){
    cli_printf("starting\n");
    cli_printf("mod_start: %u, mod_end: %u, cmdline: %s\n", mod_start, mod_end, cmdline);
    if(!mod_start or (mod_end<=mod_start) or !entry_out)
        return -1;

    if (mod_end-mod_start < sizeof(Elf32_Header)) return -2;
    

    const uint8_t* blob = (const uint8_t*)(uintptr_t)mod_start; // identity map assumed
    uint32_t blob_size  = mod_end - mod_start;
    const Elf32_Header* eh = (const Elf32_Header*)blob;

    cli_printf("Loading ELF header...\n");
    // Basic ELF sanity
    if (!(eh->e_ident[EI_MAG0]==ELFMAG0 && eh->e_ident[EI_MAG1]==ELFMAG1 &&
          eh->e_ident[EI_MAG2]==ELFMAG2 && eh->e_ident[EI_MAG3]==ELFMAG3))
        return -3;

    if (!(eh->e_type==ET_EXEC || eh->e_type==ET_DYN)) return -4;  
    if (eh->e_machine != EM_386)                  return -5;
    if (!eh->e_phoff || !eh->e_phnum)             return -6;
    if (eh->e_phentsize != sizeof(Elf32_ProgramHeader))    return -7;

    cli_printf("Loading ELF segments...\n");
    // Load PT_LOAD segments
    for (uint16_t i=0; i<eh->e_phnum; i++) {
        const uint32_t off = eh->e_phoff + i * eh->e_phentsize;
        if (off + sizeof(Elf32_ProgramHeader) > blob_size) return -8;

        const Elf32_ProgramHeader* ph = (const Elf32_ProgramHeader*)(blob + off);
        if (ph->p_type != PT_LOAD) continue;

        uint32_t va      = ph->p_vaddr;     // target VA (as linked)
        uint32_t filesz  = ph->p_filesz;
        uint32_t memsz   = ph->p_memsz;
        uint32_t poff    = ph->p_offset;

        if (!memsz) continue;

        // Map and zero the target range
        map_zero_pages_range(va, memsz);

        if (filesz) {
            copy_from_blob(va, blob, blob_size, poff, filesz);
        }

        *entry_out = (void(*)(void))(uintptr_t)eh->e_entry;

        cli_printf("Loaded ELF entry point: %p\n", *entry_out);
        cli_printf("Module size: %u bytes, from %p to %p\n", memsz, (void*)va, (void*)(va + memsz));
    }

    return 0;
}