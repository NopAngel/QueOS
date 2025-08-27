#include "irq.h"
#include "pic.h"
#include <stddef.h>
#include "../../utils/string.h"
#include "../../cliutils.h"

#define PIC_REMAP_OFFSET 0x20       // 0x20 = 32    pic 1
                                    // 0x28 = 40    pic 2


IRQHandler g_IRQHandlers[16];

static void irq_common_handler(ISR_StackFrame* regs){
    int irq_idx = regs->int_no - PIC_REMAP_OFFSET;

    if(g_IRQHandlers[irq_idx] != NULL){
        g_IRQHandlers[irq_idx](regs);
    } else {
        cli_error("Received unhandled IRQ: %d \n", irq_idx);
    }
}


static void IRQ_0_timer(ISR_StackFrame* regs){
    // printf(".");
}


static void default_IRQ_handlers() {
    set_IRQ_Handler(0, IRQ_0_timer);
}




/*
    ,--------------------,
    ; Exported functions ;
    '--------------------'
*/

void set_IRQ_Handler(int irq_index, IRQHandler handler){
    if(irq_index < 0 || irq_index >= 16){
        cli_error("Setting Invalid IRQ number: %d \n", irq_index);
        return;
    }
    g_IRQHandlers[irq_index] = handler;
}

void _sys_init_IRQ(){
    _sys_init_PIC(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8);
    for(int i = 0; i < 16; i++) {
        set_ISR_Handler(i + PIC_REMAP_OFFSET, irq_common_handler);
    }
    default_IRQ_handlers();
}