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

enum MultibootTags {
    MULTIBOOT2_MAGIC = 0x36d76289,
    
    MULTIBOOT_TAG_TYPE_END = 0,
    MULTIBOOT_TAG_TYPE_MODULE = 3,
    MULTIBOOT_TAG_TYPE_MMAP = 6,
    MULTIBOOT_TAG_TYPE_FRAMEBUFFER = 8,
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
} ModuleApp;

typedef struct{
    ModuleApp apps[16];
    int count;
} AppList;



#endif 