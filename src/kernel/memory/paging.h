#ifndef PAGING_H
#define PAGING_H

#include "../global.h"
#include "pmm.h"

void _init_kernel_paging();

uint32_t* _sys_create_pageDirectory();
void _sys_map_page(uint32_t pd_phys, uint32_t vaddr, uint32_t phys, uint32_t flags);
void* _sys_get_page_mapping(uint32_t pd_phys, uint32_t vaddr);
void _sys_load_page_directory(uint32_t pd_phys);

#endif