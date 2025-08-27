[BITS 32]
extern kernel_main

section .text
    global _start

_start:
    push ebx        ; parameters for kernel_main
    push eax        ; magic number (eax) and address (ebx)
    call setup_gdt
    call kernel_main
halt:
    hlt
    jmp halt



;--------------------
;    GDT Setup
;--------------------

setup_gdt:
    cli
    lgdt [GDT_Descriptor]
    jmp CODE_SEG:.flush_cs   ; Far jump to reload CS

.flush_cs:
    mov ax, DATA_SEG         ; Data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret




GDT_Start:
    null_descriptor:
        dd 0
        dd 0
    code_descriptor:
        dw 0xffff
        dw 0
        db 0
        db 0b10011010
        db 0b11001111
        db 0
    data_descriptor:
        dw 0xffff
        dw 0
        db 0
        db 0b10010010
        db 0b11001111
        db 0
    user_code_descriptor:
        dw 0xFFFF
        dw 0
        db 0
        db 0b11111010       
        db 0b11001111
        db 0
    user_data_descriptor:
        dw 0xFFFF
        dw 0
        db 0
        db 0b11110010     
        db 0b11001111
        db 0
GDT_End:

GDT_Descriptor:
    dw GDT_End - GDT_Start -1
    dd GDT_Start

CODE_SEG equ code_descriptor-GDT_Start
DATA_SEG equ data_descriptor-GDT_Start
