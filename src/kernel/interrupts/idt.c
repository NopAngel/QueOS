#include "idt.h"
#include <stdint.h>

/*
    IDT (Interrupt Descriptor Table)
    -------------------------------------------------------------
    *   g_IDT stores all IDT entries.
        each entry of g_IDT stores correspondint ISR handler (+other)

    *   _IDT_SetEntry is used in isr.c to set the IDT entry
*/



IDT_Entry g_IDT[256] __attribute__((aligned(8)));;

IDT_Register g_IDT_Register = {
    sizeof(g_IDT)-1,
    (uint32_t)g_IDT
};

void _IDT_SetEntry(int int_index, uint32_t handler_address, uint16_t seg_selector, uint8_t flags) {
    if (int_index < 0 || int_index >= 256) return;
    
    uint32_t mask = 0b00000000000000001111111111111111;

    g_IDT[int_index].baseLow = (uint16_t)(handler_address & mask);
    g_IDT[int_index].segSelector = seg_selector;
    g_IDT[int_index].reserved = 0; 
    g_IDT[int_index].flags = flags;
    g_IDT[int_index].baseHigh = (uint16_t)((handler_address >> 16) & mask);
}

void _sys_init_IDT() {
    _load_IDT(&g_IDT_Register);
}