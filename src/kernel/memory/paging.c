#include "paging.h"

// as we dont have any allocator, go for full
#define MAX_IDENTITY_MB 4096
#define TABLES_CAP ((MAX_IDENTITY_MB + 3) / 4)

__attribute__((aligned(4096))) static uint32_t kernel_page_directory[1024];
__attribute__((aligned(4096))) static uint32_t kernel_page_tables[TABLES_CAP][1024];

extern void load_page_directory(uint32_t phys_addr);
extern void enable_paging(void);

static inline void memset32(uint32_t *p, uint32_t v, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) p[i] = v;
}

static enum {
    PRESENT = 1,
    RW = 2
} PageFlags;



void _init_kernel_paging(){
    memset32(kernel_page_directory, 0, 1024);

    int tablesNeeded=TABLES_CAP;
    
    for(uint32_t t = 0; t < tablesNeeded; t++) {
        /*
        identity map
        0-11 (12 bits) -> 4096 = 4kb
        12-31 is just like normal number (except shifted left by 12) indicates only the page start address
        */
        for(uint32_t i=0; i<1024; i++){
            uint32_t phys = ((t * 1024u) + i) << 12;
            kernel_page_tables[t][i] = phys | PRESENT | RW;
        }
        uint32_t table_phys = (uint32_t) &kernel_page_tables[t][0];
        kernel_page_directory[t] = table_phys | PRESENT | RW;
    }

    load_page_directory((uint32_t)kernel_page_directory);
    enable_paging();
}