#include "kernel/global.h"
#include "./kernel/drivers/display/display.h"
#include "kernel/drivers/keyboard/keyboard.h"
#include "kernel/memory/memory.h"
#include "kernel/memory/vmm.h"
#include "cliutils.h"
#include "kernel/loader/elf.h"
#include "kernel/usermode/appentry.h"


void print_meminfo();
void launchApp();
void dump_gdt();

struct screen scr;
struct __attribute__((packed)) gdtr {
    uint16_t limit;
    uint32_t base;
};

extern uint8_t GDT_Start[];
extern uint8_t GDT_End[];
extern struct gdtr GDT_Descriptor;

extern ScreenInfo _SYS_ScreenInfo;
extern ModuleList module_list;  
extern AppList app_list;



void virtual_putc(char c){
    if(c=='\b'){    // unsure to keep
        if(scr.x > 0) scr.x--;
        cli_printf(" ");
        return;
    }
    if(c=='\n'){
        scr.x = 0;
        scr.y++;
    }else{
        _sys_put_char(scr.x * FONT_WIDTH, scr.y * FONT_HEIGHT, hex_color(scr.fg_color), c);
        scr.x++;
    }
    if(scr.x >= scr.cols){
        scr.x = 0;
        scr.y++;
    }
}





// for now just return a string when a new line is found
void cli_scanf(char *buffer){
    char c = 0;
    int i = 0;
    while (c!='\n'){
        c = _sys_get_char();
        if(c){
            if(c=='\b' && i){
                scr.x-=1;
                i--;
                cli_clear_current_char();
                continue;
            }
            char print_buffer[2] = {c, '\0'};
            cli_colored_printf(get_color(COLOR_LIGHT_BLUE), print_buffer);

            buffer[i]=c;
            i++;
        }
    }
    buffer[i]='\0';
}








void init_cli_screen(){
    scr.cols = _SYS_ScreenInfo.width / FONT_WIDTH;
    scr.rows = _SYS_ScreenInfo.height / FONT_HEIGHT;
    scr.x = 0;
    scr.y = 0;
    scr.default_fg_color = create_color(200, 200, 200); 
    scr.default_bg_color = create_color(22, 22, 25);
    scr.fg_color = scr.default_fg_color;
    scr.bg_color = scr.default_bg_color;
    _sys_fill_rect(0, 0, _SYS_ScreenInfo.width, _SYS_ScreenInfo.height, hex_color(scr.bg_color));
}


void cli_main(){
    cli_printf(",-------------------------------,\n");
    cli_printf(";         Hello, QueOS!         ;\n");
    cli_printf("'-------------------------------'\n");
    cli_printf("This is a simple CLI.\n");
    cli_printf("Type 'help' for a list of commands.\n");

    // _sys_pmm_get_range_states();

    char buffer[256]="\0";
    cli_printf("\n");
    while (1) {
        cli_colored_printf(get_color(COLOR_GREEN), "$ ");
        cli_scanf(buffer);

        if(strcmp(buffer, "osinfo\n") == 0){
            cli_printf("BunnyOS v1.0\n");
            cli_printf("A simple operating system for x86 architecture.\n");
            cli_printf("\n");
        }
        else if(strcmp(buffer, "app1\n") == 0){
            cli_printf("Launching application...\n");
            launchApp();
        }else if(strcmp(buffer, "dumpgdt\n") == 0){
            dump_gdt();
            cli_printf("\n");
        }
        else if(strcmp(buffer, "meminfo\n")==0){
            print_meminfo();
            cli_printf("\n");
        }
        else if(strcmp(buffer, "pmminfo\n")==0){
            _sys_pmm_get_range_states();
            cli_printf("\n");
        }
        else if(strcmp(buffer, "pmmalloc\n")==0){
            _sys_pmm_alloc_page(5);
            _sys_pmm_reserve_range((void*)6000000, (void*)(6000000 + 5 * 1024));
            cli_printf("\n");
        }
        else if(strncmp(buffer, "test", 4) == 0){
            char *ptr = buffer + 4; 
            if(*ptr == '\n'){
                cli_printf("Test command received with no arguments.\n");
            }
            else if (strncmp(ptr+1, "interrupt", 9) == 0){
                cli_printf("Simulating interrupt...\n");
                asm volatile (
                    "int $0x35" 
                );

            }else if (strncmp(ptr+1, "vmm", 3) == 0){
                uint32_t *pd=_sys_create_pageDirectory();
                // char *randomAddr=(char*)0x12345000;
                _sys_load_page_directory((uint32_t)pd);
                // randomAddr=(char*)0x12345000;
                // call app
                while(1);
            }
            else{
                cli_printf("Unsupported test command: ");
                cli_printf(ptr);
            }
            cli_printf("\n");
        }
        else if(strcmp(buffer, "clear\n") == 0){
            scr.x = 0;
            scr.y = 0;
            _sys_fill_rect(0, 0, _SYS_ScreenInfo.width, _SYS_ScreenInfo.height, hex_color(scr.bg_color));
        }
        else if(strcmp(buffer, "help\n") == 0){
            cli_printf("Available commands:\n");
            cli_printf("osinfo - Show OS information\n");
            cli_printf("app1 - Launch the first loaded application\n");
            cli_printf("test - Run a test command (e.g., test interrupt)\n");
            cli_printf("clear - Clear the screen\n");
            cli_printf("meminfo - Show memory information\n");
            cli_printf("pmminfo - Show physical memory manager information\n");
            cli_printf("pmmalloc - Allocate memory from the physical memory manager\n");
            cli_printf("help - Show this help message\n");
            cli_printf("\n");
        }
        else{
            cli_printf("Unknown command: ");
            cli_printf(buffer);
            cli_printf("\n");
        }
        buffer[0] = '\0'; 
    }
}





void dump_gdt() {
    uint16_t limit = GDT_Descriptor.limit;
    uint32_t base  = GDT_Descriptor.base;

    cli_printf("GDT Base:  %x\n", base);
    cli_printf("GDT Limit: %x (%u bytes)\n\n", limit, (unsigned)limit + 1);

    int entries = (limit + 1) / 8;  // each entry = 8 bytes
    // for (int i = 0; i < entries; i++) {
    //     uint64_t desc = *(uint64_t *)(base + i * 8);
    //     cli_printf("GDT[%d] = %x\n", i, (unsigned long long)desc);
    // }
    for (int i = 0; i < entries; i++) {
        uint64_t desc = *(uint64_t *)(base + i * 8);

        // Extract fields
        uint16_t limit_low   = desc & 0xFFFF;
        uint16_t base_low    = (desc >> 16) & 0xFFFF;
        uint8_t  base_mid    = (desc >> 32) & 0xFF;
        uint8_t  access      = (desc >> 40) & 0xFF;
        uint8_t  gran_limit  = (desc >> 48) & 0xFF;
        uint8_t  base_high   = (desc >> 56) & 0xFF;

        uint32_t base_full   = base_low | (base_mid << 16) | (base_high << 24);
        uint32_t limit_full  = limit_low | ((gran_limit & 0x0F) << 16);
        uint8_t  flags       = (gran_limit >> 4) & 0x0F;

        // Print nicely
        cli_printf("GDT[%d]: raw=0x%016llx\n", i, (long long)desc);
        cli_printf("  Base=%x  Limit=%x\n", base_full, limit_full);
        cli_printf("  Access=%b  Flags=%x\n", access, flags);
        cli_printf("\n");
    }
}




void print_meminfo(){
    char buffer[256];
    SystemMemoryInfo *sysmem = _sys_get_memory_info();
    cli_printf("Total Memory: %d MB\n", byteToMB(sysmem->total_memory));

    cli_printf("Kernel Memory Start: %p\n", (uintptr_t)__kernel_start);
    cli_printf("Kernel Memory End  : %p\n", (uintptr_t)__kernel_end);


    cli_printf("Kernel Memory Size: ");
    int size = (uintptr_t)__kernel_end - (uintptr_t)__kernel_start;
    size = byteToKB(size);
    if(size < 1024) {
        cli_printf("%d kB\n", size);
    } else {
        cli_printf("%d MB\n", kBToMB(size));
    }


    cli_printf("Memory Regions:\n");
    for (int i = 0; i < sysmem->regionCount; i++) {

        uint64_t base = sysmem->regions[i].base;
        char type=0;

        if(base>=1024){
            type=1;
            base = byteToKB_64u(base);
            if(base>=1024){
                type=2;
                base = kBToMB(base);
            }
        }
        
        cli_printf(
            "  Region %d : Base = %lld %s, Type = %d", 
            i, 
            base,
            type == 0 ? "byte" : type == 1 ? "kB" : "MB",
            sysmem->regions[i].type
        );


        
        cli_printf(", Length = ");
        int len=byteToKB(sysmem->regions[i].length);
        if(len < 1024) {
            cli_printf("%d kB\n", len);
        } else {
            cli_printf("%d MB\n", kBToMB(len));
        }
    }
}







void launchApp(){   
    app_list.apps[app_list.count].module = &module_list.apps[0];
    // app_list.apps[app_list.count].vmm = _sys_vmm_create_context();
    app_list.apps[app_list.count].entry = NULL;
    
    int status = _sys_load_module_app(
        module_list.apps[0].base, 
        module_list.apps[0].base + module_list.apps[0].size,
        module_list.apps[0].name, 
        &(app_list.apps[app_list.count].entry)
    );

    
    if (app_list.apps[app_list.count].entry) {
        cli_printf("Launching application: %s\n", module_list.apps[0].name);
        cli_printf("======================\n\n");
        // app_list.apps[app_list.count].entry();
        // app_list.count++;
        start_user_program((uint32_t)app_list.apps[app_list.count].entry);
        // should never return here
        cli_printf("\n======================\n");
    } else {
        cli_printf("Failed to launch application: %s\n", module_list.apps[0].name);
        cli_printf("error : %d\n", status);
    }
    cli_printf("\n");
}