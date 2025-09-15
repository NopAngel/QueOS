/*
    Bitmap Allocation
*/

#include "pmm.h"
#include "../../cliutils.h"

#define PAGE_SIZE 4096 // 4KB 
#define BITS_PER_PAGE 1
#define BITMAP_SIZE ((4*((1024*1024*1024U)/PAGE_SIZE)*BITS_PER_PAGE)/8)


static uint8_t bitmap[BITMAP_SIZE];

static void init_bitmap() {
    //initially mark every page as used
    for (size_t i = 0; i < BITMAP_SIZE; i++) {
        bitmap[i] = 255;
    }
}

static void update_bitmap(size_t page, bool used) {
    size_t byte_index = page / 8;
    size_t bit_index = page % 8;

    if (used) {
        bitmap[byte_index] |= (1 << bit_index);
    } else {
        bitmap[byte_index] &= ~(1 << bit_index);
    }
}




/*
    ,--------------------,
    ; Exported functions ;
    '--------------------'
*/

bool check_bitmap(size_t page) {
    size_t byte_index = page / 8;
    size_t bit_index = page % 8;

    return (bitmap[byte_index] & (1 << bit_index)) != 0;
}

uint8_t* get_bitmap() {
    return bitmap;
}   

void _sys_init_pmm(SystemMemoryInfo mem_info) {
    init_bitmap();

    uint64_t kernel_end = (uintptr_t)__kernel_end;
    uint64_t startAddr = (kernel_end + PAGE_SIZE - 1) & ~((uint64_t)PAGE_SIZE - 1);

    int size = 0;
    for(int i = 0; i < mem_info.regionCount; i++) {
        if(mem_info.regions[i].type == 1) {
            int mx = mem_info.regions[i].length;
            if(mx > size) {
                size = mx;
            }
        }
    }

    uint64_t endAddr = (startAddr + size) & ~((uint64_t)PAGE_SIZE - 1);

    for (uint64_t addr = startAddr; addr < endAddr; addr += PAGE_SIZE) {
        size_t page = addr / PAGE_SIZE;
        update_bitmap(page, false); // Mark as free
        // cli_printf("Freed page: %u status :%d\n", page, check_bitmap(page));
    }
}


bool _sys_pmm_reserve_specific(void *ptr) {
    size_t page = (uintptr_t)ptr / PAGE_SIZE;
    if (check_bitmap(page)) return false; // Already reserved
    cli_printf("Reserved page: %u\n", page);
    update_bitmap(page, true);
    return true;
}

bool _sys_pmm_reserve_range(void *start, void *end) {
    size_t start_page = (uintptr_t)start / PAGE_SIZE;
    size_t end_page = (uintptr_t)end / PAGE_SIZE;

    for (size_t page = start_page; page <= end_page; page++) {
        if (check_bitmap(page)) return false; // Already reserved
    }
    
    cli_printf("Reserving pages from %u to %u\n", start_page, end_page);
    for (size_t page = start_page; page <= end_page; page++) {
        update_bitmap(page, true);
        cli_printf("Reserved page: %u\n", page);
    }

    // [ Possible Bug : the end address might need another page ? ]

    return true;
}

void* _sys_pmm_alloc_page(int number){
    if(number <= 0){
        return NULL;
    }

    int cnt = number;
    
    int startPage=-1;
    for(int page=0; page<BITMAP_SIZE*8; page++) {
        if(check_bitmap(page)==0){
            if (startPage == -1) {
                startPage = page;
            }
        }else {
            startPage=-1;
        }
        if(startPage != -1 && page-startPage+1>=cnt){
            for(int i=startPage;i<startPage+cnt;i++){
                update_bitmap(i,true);
                // cli_printf("allocated : %d, page : %d\n", (i * PAGE_SIZE), i);
            }
            return (void*)(uintptr_t)(startPage * PAGE_SIZE);
        }
    }

    return NULL;
}

void _sys_pmm_get_range_states(){
    int last=-1;
    for(uint64_t i=0;i<BITMAP_SIZE*8;){
        int current = check_bitmap(i);
        if(current != last){
            last = current;
            cli_printf("starting from address %u KB", byteToKB_64u(i * PAGE_SIZE));
            uint64_t end = i;
            while(check_bitmap(i) == last && i < BITMAP_SIZE*8){
                end = i;
                i++;
            }
            cli_printf(" to %u KB, state = %s\n", byteToKB_64u(end * PAGE_SIZE), current? "used" : "free");
        }
    }
}











