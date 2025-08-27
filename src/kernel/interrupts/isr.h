#ifndef ISR_H
#define ISR_H

#include "../global.h"

typedef struct{
    uint32_t ds;
    uint32_t edi, esi, ebp, kernel_esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, user_esp, ss;
} __attribute__((packed)) ISR_StackFrame;

typedef void (*ISR_Handler)(ISR_StackFrame* regs);

void _sys_init_ISR();
void set_ISR_Handler(int int_no, ISR_Handler handler);

#endif