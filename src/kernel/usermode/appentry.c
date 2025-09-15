#include "appentry.h"
#include "../memory/vmm.h"

extern void enter_user_mode(uint32_t entry, uint32_t user_stack);
extern void cli_printf(const char *fmt, ...);

// void setup_user_program(uint32_t entry){
//     vmmContext *ctx = _sys_vmm_create_context();
// }


void start_user_program(uint32_t entry){
    // vmmContext *ctx = _sys_vmm_create_context();
    void* stack = _sys_pmm_alloc_page(16); // 64KB stack
    cli_printf("(Starting user program at 0x%x with stack at 0x%x)\n", entry, stack);
    enter_user_mode(entry, (uint32_t)stack);
}