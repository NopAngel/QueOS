#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../../global.h" 
#include "../../io/io.h"
#include "../../interrupts/irq.h"
#include "../../interrupts/isr.h"

void _sys_init_keyboard();
char _sys_get_char();

#endif
