#ifndef IO_H
#define IO_H

#include <stdint.h>

// implemented in assembly

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);
void enable_interrupts();
void disable_interrupts();

#endif