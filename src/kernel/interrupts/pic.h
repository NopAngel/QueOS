#ifndef PIC_H
#define PIC_H

#include <stdint.h>
#include "../io/io.h"

void _sys_init_PIC(uint8_t offsetPIC1, uint8_t offsetPIC2);
void enable_PIC();
void disable_PIC();
void mask_PIC(uint8_t irq);
void unmask_PIC(uint8_t irq);
// void send_EOI(uint8_t irq); // we set auto EOI in ICW4, so this is not needed
uint16_t read_IRR();
uint16_t read_ISR();

#endif 