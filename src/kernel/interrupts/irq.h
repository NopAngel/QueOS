#ifndef IRQ_H
#define IRQ_H

#include "../global.h"
#include "isr.h"

typedef void (*IRQHandler)(ISR_StackFrame* regs);

void _sys_init_IRQ();
void set_IRQ_Handler(int irq_index, IRQHandler handler);


#endif