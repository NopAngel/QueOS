#include "global.h"
#include "io/io.h"
#include "interrupts/idt.h"
#include "interrupts/isr.h"
#include "interrupts/irq.h"
#include "memory/memory.h"
#include "drivers/display/display.h"
#include "drivers/keyboard/keyboard.h"
#include "../cliutils.h"


extern ScreenInfo _SYS_ScreenInfo;
extern void cli_main();
extern void init_cli_screen();






typedef struct{
    multiboot_tag tag;
    uint32_t mod_start;
    uint32_t mod_end;
    char cmdline[0];    // zero terminated string
}multiboot_tag_module;









AppList app_list;


void kernel_main(uint32_t magic, uint32_t addr) {
    if (magic != MULTIBOOT2_MAGIC) return;

    multiboot_tag* tag = (multiboot_tag*)(addr + 8); // skip total_size and reserved
    
    _sys_init_display(tag);
    init_cli_screen();
    
    _sys_init_ISR();
    _sys_init_IDT();
    _sys_init_IRQ();
    
    _sys_init_memory(tag);
    
    _sys_init_keyboard();
    
    enable_interrupts();


    app_list.count = 0;
    // cli_printf("Loading modules...\n");

    while (tag->type != MULTIBOOT_TAG_TYPE_END) {
        if (tag->type == MULTIBOOT_TAG_TYPE_MODULE) {
            // cli_printf("got a module\n");
            multiboot_tag_module* module = (multiboot_tag_module*)tag;
            
            cli_printf("Loaded module: %s, from %d kb to %d kb\n", &(module->cmdline[0]), byteToKB(module->mod_start), byteToKB(module->mod_end));
            _sys_pmm_reserve_range((void*)module->mod_start, (void*)module->mod_end);

            app_list.apps[app_list.count].base = module->mod_start;
            app_list.apps[app_list.count].size = module->mod_end - module->mod_start;
            app_list.apps[app_list.count].name = &(module->cmdline[0]);
            // cli_printf("%s\n", app_list.apps[app_list.count].name);
            app_list.count++;
        }
        tag = (multiboot_tag*)((uint8_t*)tag + ((tag->size + 7) & ~7));
    }



    // finally go to cli stuff
    cli_main();

    while (1);
}
