#ifndef IDT_H
#define IDT_H

#include <stdint.h>


typedef struct {
    uint16_t baseLow;
    uint16_t segSelector;
    uint8_t reserved;
    uint8_t flags;
    uint16_t baseHigh;
} __attribute__((packed)) IDT_Entry;


typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) IDT_Register;


typedef enum {
    // ignoring 16bit flags
    IDT_FLAG_GATE_TASK          =       0b0101,
    IDT_FLAG_GATE_INTERRUPT     =       0b1110,   
    IDT_FLAG_GATE_TRAP          =       0b1111,

    // 5th is reserved, must be 0
    IDT_FLAG_RING_0             =   0b00000000,
    IDT_FLAG_RING_1             =   0b00100000,
    IDT_FLAG_RING_2             =   0b01000000,
    IDT_FLAG_RING_3             =   0b01100000,

    //always 1
    IDT_FLAG_PRESENT            =   0b10000000
} IDT_Flags;



void _sys_init_IDT();  
void _IDT_SetEntry(int int_index, uint32_t handler_address, uint16_t seg_selector, uint8_t flags) ;

extern void _load_IDT(IDT_Register* idt_register);     // implemented in idt.asm

#endif