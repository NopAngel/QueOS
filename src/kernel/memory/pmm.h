#ifndef PMM_H
#define PMM_H

#include "../global.h"

void _sys_init_pmm(SystemMemoryInfo mem_info);
bool _sys_pmm_reserve_specific(void *ptr);
bool _sys_pmm_reserve_range(void *start, void *end);
void* _sys_pmm_alloc_page(int number);
void _sys_pmm_free(void *ptr);

bool check_bitmap(size_t page);
uint8_t* get_bitmap(); // Returns the bitmap for testing purposes
void _sys_pmm_get_range_states();

#endif 