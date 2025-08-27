#include "memory.h"
#include "../../cliutils.h"

#define MAX_MMAP_ENTRIES 128


typedef struct {
    uint64_t addr;
    uint64_t len;
    uint32_t type;
    uint32_t zero;
} __attribute__((packed)) multiboot_mmap_entry;

typedef struct {
    multiboot_tag tag;    
    uint32_t entry_size;   
    uint32_t entry_version;
    // followed by entries: multiboot_mmap_entry entries[];
} __attribute__((packed)) multiboot_tag_mmap;



static MemoryRegion g_memory_map[MAX_MMAP_ENTRIES];
static uint32_t g_memory_map_count = 0;
static SystemMemoryInfo sysmem;




/*
    ,--------------------,
    ; Exported functions ;
    '--------------------'
*/


void _sys_init_memory(multiboot_tag* tag) {

    while (tag->type != MULTIBOOT_TAG_TYPE_END) {
        if (tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
            
            multiboot_tag_mmap *mmap_tag = (multiboot_tag_mmap*)tag;
            
            sysmem.regionCount=0;

            /*  
                --------------------------------------
                warning : some ai generated code below 
                --------------------------------------
            */

            /* compute start and end of entries region */
            uint8_t *entries_start = (uint8_t*)mmap_tag + sizeof(multiboot_tag_mmap);
            uint8_t *entries_end   = (uint8_t*)mmap_tag + mmap_tag->tag.size;

            /* walk each entry using entry_size */
            uint8_t *p = entries_start;
            while (p + mmap_tag->entry_size <= entries_end && g_memory_map_count < MAX_MMAP_ENTRIES) {
                multiboot_mmap_entry *entry = (multiboot_mmap_entry*)(uintptr_t)p;

                /* copy into simpler structure */
                g_memory_map[g_memory_map_count].base = entry->addr;
                g_memory_map[g_memory_map_count].length = entry->len;
                g_memory_map[g_memory_map_count].type = entry->type;
                g_memory_map_count++;
                
                p += mmap_tag->entry_size;
                sysmem.regionCount++;
            }

            sysmem.regions = g_memory_map;
            
            sysmem.total_memory = 0;
            for (int i = 0; i < sysmem.regionCount; i++) {
                sysmem.total_memory += sysmem.regions[i].length;
            }

        }
        tag = (multiboot_tag*)((uint8_t*)tag + ((tag->size + 7) & ~7));
    }


    _init_kernel_paging();
    _sys_init_pmm(sysmem);
}



SystemMemoryInfo* _sys_get_memory_info() {
    return &sysmem;
}