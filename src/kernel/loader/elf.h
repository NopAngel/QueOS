#ifndef ELF_H
#define ELF_H

#include "../memory/pmm.h"
#include "../global.h"


int _sys_load_module_app(uint32_t mod_start, uint32_t mod_end, const char* cmdline, void (**entry_out)(void));

#endif