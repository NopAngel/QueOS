#include "paging.h"

// as we dont have any allocator, go for full
#define MAX_IDENTITY_MB 4096
#define TABLES_CAP ((MAX_IDENTITY_MB + 3) / 4)

#define PD_INDEX(vaddr) (((vaddr) >> 22) & 0x3FF)
#define PT_INDEX(vaddr) (((vaddr) >> 12) & 0x3FF)





__attribute__((aligned(4096))) static uint32_t kernel_page_directory[1024];
__attribute__((aligned(4096))) static uint32_t kernel_page_tables[TABLES_CAP][1024];

extern void load_page_directory(uint32_t phys_addr);
extern void enable_paging(void);

static inline void memset32(uint32_t *p, uint32_t v, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) p[i] = v;
}






/*
    ,--------------------,
    ; Exported functions ;
    '--------------------'
*/



void _init_kernel_paging(){
    memset32(kernel_page_directory, 0, 1024);

    for(uint32_t t = 0; t < TABLES_CAP; t++) {
        /*
        identity map
        0-11 (12 bits) -> 4096 = 4kb
        12-31 is just like normal number (except shifted left by 12) indicates only the page start address
        */
        for(uint32_t i=0; i<1024; i++){
            uint32_t phys = ((t * 1024u) + i) << 12;
            kernel_page_tables[t][i] = phys | PAGE_PRESENT | PAGE_RW | PAGE_USER;
        }
        uint32_t table_phys = (uint32_t) &kernel_page_tables[t][0];
        kernel_page_directory[t] = table_phys | PAGE_PRESENT | PAGE_RW | PAGE_USER;
    }

    load_page_directory((uint32_t)kernel_page_directory);
    enable_paging();
}





uint32_t* _sys_create_pageDirectory() {
    uint32_t* page_dir = (uint32_t*) _sys_pmm_alloc_page(1);
    for (int i = 0; i < PAGE_ENTRIES; i++) {
        page_dir[i] = 0;
    }

    for (int i = 0; i < PAGE_ENTRIES; i++) {
        uint32_t* page_table = (uint32_t*) _sys_pmm_alloc_page(1);

        for (int j = 0; j < PAGE_ENTRIES; j++) {
            page_table[j] = 0;  // all bits 0, also means 'not present'
        }
        // page table not present, but directory entry is
        page_dir[i] = ((uint32_t) page_table) | PAGE_RW | PAGE_USER | PAGE_PRESENT;
    }

    return page_dir;
}




void _sys_map_page(uint32_t pd_phys, uint32_t vaddr, uint32_t phys, uint32_t flags) {
    uint32_t pd_index = PD_INDEX(vaddr);
    uint32_t pt_index = PT_INDEX(vaddr);

    uint32_t* page_dir = (uint32_t*) pd_phys;
    uint32_t* page_table = (uint32_t*)(page_dir[pd_index] & ~0xFFF);

    page_dir[pd_index] |= PAGE_PRESENT | PAGE_RW;
    if (flags & PAGE_USER) {
        page_dir[pd_index] |= PAGE_USER;
    }
    
    page_table[pt_index] = (phys & ~0xFFF) | (flags | PAGE_PRESENT);
    
    // on real hardware, apparently we need to do this (reason not 100% clear)
    asm volatile("invlpg (%0)" :: "r"(vaddr) : "memory");
}







void* _sys_get_page_mapping(uint32_t pd_phys, uint32_t vaddr){
    uint32_t pd_index = PD_INDEX(vaddr);
    uint32_t pt_index = PT_INDEX(vaddr);

    uint32_t* page_dir = (uint32_t*) pd_phys;
    
    if (page_dir[pd_index] == 0) {
        return NULL;
    }

    uint32_t* page_table = (uint32_t*) (page_dir[pd_index] & ~0xFFF);
    return (void*) (page_table[pt_index] & ~0xFFF);
}


void _sys_load_page_directory(uint32_t pd_phys) {
    load_page_directory(pd_phys);
    // enable_paging();
}