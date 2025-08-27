#ifndef MEMORY_H
#define MEMORY_H

#include "../global.h"
#include "paging.h"
#include "pmm.h"

void _sys_init_memory(multiboot_tag* tag);
SystemMemoryInfo* _sys_get_memory_info();

void* _sys_malloc(size_t size);
void _sys_free(void* ptr);

#endif 