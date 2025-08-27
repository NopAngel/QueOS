[bits 32]

section .text

global inb
global outb

; I/O port operations
; --------------------------
; uint8_t inb(uint16_t port);
; void outb(uint16_t port, uint8_t value);

inb:
    mov dx, [esp + 4] 
    xor eax, eax
    in al, dx
    ret

outb:
    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret

    

; Interrupt control
; --------------------------

global enable_interrupts
global disable_interrupts

enable_interrupts:
    sti
    ret

disable_interrupts:
    cli
    ret