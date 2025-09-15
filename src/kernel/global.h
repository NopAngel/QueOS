#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
#include "../utils/string.h"
#include "../utils/color.h"
#include "../utils/conversion.h"

#define or ||
#define and &&

extern char __kernel_start[];
extern char __kernel_end[];
extern struct _vmm_context;
typedef struct _vmm_context vmmContext;

extern uint32_t saved_kernel_sp;


enum MultibootTags {
    MULTIBOOT2_MAGIC = 0x36d76289,
    
    MULTIBOOT_TAG_TYPE_END = 0,
    MULTIBOOT_TAG_TYPE_MODULE = 3,
    MULTIBOOT_TAG_TYPE_MMAP = 6,
    MULTIBOOT_TAG_TYPE_FRAMEBUFFER = 8,
};

enum PageFlags {
    PAGE_PRESENT   = 0b00000001,
    PAGE_RW        = 0b00000010,
    PAGE_USER      = 0b00000100
};

enum Page{
    PAGE_SIZE      = 4096,
    PAGE_ENTRIES   = (PAGE_SIZE / 4)
};

typedef struct {
    uint32_t type;
    uint32_t size;
} __attribute__((packed)) multiboot_tag;


typedef struct {
    uint64_t base;
    uint64_t length;
    uint32_t type;
} MemoryRegion;

typedef struct{
    uint32_t total_memory; 
    MemoryRegion *regions;
    int regionCount;
} SystemMemoryInfo;




typedef struct{
    char *name;
    uint64_t base;
    uint64_t size;
} Module;

typedef struct{
    Module apps[16];
    int count;
} ModuleList;


typedef struct{
    Module *module;
    vmmContext *vmm;
    uint32_t user_stack;
    void (*entry)(void);
} AppContext;

typedef struct{
    AppContext apps[16];
    int count;
} AppList;





#endif 