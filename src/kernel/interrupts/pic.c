#include "pic.h"

#define PIC1_COMMAND_PORT 0x20
#define PIC1_DATA_PORT    0x21
#define PIC2_COMMAND_PORT 0xA0
#define PIC2_DATA_PORT    0xA1



/*
    Initialization Control Word 1 (ICW1) 
    ---------------------------------------------
    0   IC4     if set, ICW4 is needed
    1   SGNL    if set, only one PIC is present, if unset, two PICs are present 
                and ICW3 must be sent
    2   ADI     0 (ignored in x86)
    3   LTIM    if set, level triggered mode, if unset, edge triggered mode
    4   INIT    1 (for ICW-1)
    5-7         0 (ignored in x86)


    Initialization Control Word 4 (ICW4)
    ---------------------------------------------
    0     x86 mode      if set, 8086/8088 mode (x86)
    1     Auto EOI      if set, automatic End of Interrupt
    2-3                 (ignored)
    4     Fully nested  if set, fully nested mode (not used in x86)
    5-7                 (ignored)
*/



enum {
    PIC_ICW1_ICW4_NEEDED = 0b00000001,
    PIC_ICW1_SINGLE      = 0b00000010,   // wont use, cuz we have two PICs
    PIC_ICW1_LEVEL       = 0b00001000,   // wont use, cuz we use edge triggered mode
    PIC_ICW1_INIT        = 0b00010000,

} PIC_ICW1;

enum {
    PIC_ICW4_8086_MODE = 0b00000001,
    PIC_ICW4_AUTO_EOI  = 0b00000010,
    PIC_ICW4_NESTED    = 0b00010000, // wont use, cuz we dont use fully nested mode
} PIC_ICW4;



void _sys_init_PIC(uint8_t offsetPIC1, uint8_t offsetPIC2){
    // ICW-1
    outb(PIC1_COMMAND_PORT,     PIC_ICW1_INIT | PIC_ICW1_ICW4_NEEDED);
    outb(PIC2_COMMAND_PORT,     PIC_ICW1_INIT | PIC_ICW1_ICW4_NEEDED);

    // ICW-2
    outb(PIC1_DATA_PORT,        offsetPIC1);
    outb(PIC2_DATA_PORT,        offsetPIC2);

    // ICW-3
    outb(PIC1_DATA_PORT,        4);  // 0000 0100
    outb(PIC2_DATA_PORT,        2);

    // ICW-4
    outb(PIC1_DATA_PORT,        PIC_ICW4_8086_MODE | PIC_ICW4_AUTO_EOI);
    outb(PIC2_DATA_PORT,        PIC_ICW4_8086_MODE | PIC_ICW4_AUTO_EOI);

    // clear the masks
    outb(PIC1_DATA_PORT,        0);  
    outb(PIC2_DATA_PORT,        0);  
}








/*
    Operational Control Word 1 (OCW1)
    ---------------------------------------------
    A0 = 1, and D7 to D0 are the mask bits M7 to M0
*/


void mask_PIC(uint8_t irq){
    uint8_t port;
    if(irq < 8){
        port = PIC1_DATA_PORT;
    } else {
        irq -= 8;
        port = PIC2_DATA_PORT;
    }
    uint8_t mask = inb(port);
    outb(port, mask | (1 << irq));
}

void unmask_PIC(uint8_t irq){
    uint8_t port;
    if(irq < 8){
        port = PIC1_DATA_PORT;
    } else {
        irq -= 8;
        port = PIC2_DATA_PORT;
    }
    uint8_t mask = inb(port);
    outb(port, mask & ~(1 << irq));
}


void disable_PIC(){
    outb(PIC1_DATA_PORT, 0xFF);
    outb(PIC2_DATA_PORT, 0xFF);
}

void enable_PIC(){
    outb(PIC1_DATA_PORT, 0x00);
    outb(PIC2_DATA_PORT, 0x00);
}





/*
    Operational Control Word 2 (OCW2)
    ---------------------------------------------
    A0 = 0 , D4 = 0 and D3 = 0
    Lower 3 bits (D2 to D0) are the IRQ number
    upper 3 bits (D7 to D5) to specify operation mode

    we do not need to use OCW2 for sending EOI if we enabled Auto EOI in ICW4 ???
*/

void send_EOI(uint8_t irq){
    if(irq < 8){
        outb(PIC1_COMMAND_PORT, 0x20); 
    } else {
        outb(PIC2_COMMAND_PORT, 0x20);  
        outb(PIC1_COMMAND_PORT, 0x20); 
    }
}



/*
    Operational Control Word 3 (OCW3)
    ---------------------------------------------
    A0 = 0, D4 = 0, D3 = 1
    D0-D1   read  IRR (all requests) or ISR (in service register, that are being serviced)
    remaining bits are not applicable for us
*/

enum {
    PIC_OCW3_READ_IRR = 0x0A, 
    PIC_OCW3_READ_ISR = 0x0B
} PIC_OCW3;

uint16_t read_IRR(){
    outb(PIC1_COMMAND_PORT, PIC_OCW3_READ_IRR);
    outb(PIC2_COMMAND_PORT, PIC_OCW3_READ_IRR);
    return inb(PIC1_COMMAND_PORT) | (inb(PIC2_COMMAND_PORT) << 8);
}

uint16_t read_ISR(){
    outb(PIC1_COMMAND_PORT, PIC_OCW3_READ_ISR);
    outb(PIC2_COMMAND_PORT, PIC_OCW3_READ_ISR);
    return inb(PIC1_COMMAND_PORT) | (inb(PIC2_COMMAND_PORT) << 8);
}