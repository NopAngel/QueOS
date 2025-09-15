[BITS 32]
extern kernel_main

section .text
    global _start
    global GDT_Start
    global GDT_End
    global GDT_Descriptor
    global KERNEL_STACK_TOP
    global saved_kernel_sp

_start:
    ; cli
    ; mov esp, KERNEL_STACK_TOP  
    ; [TO DO] [IMPORTANT] Set up a proper stack separate from TSS stack

    mov ebp, esp
    push ebx        ; parameters for kernel_main
    push eax        ; magic number (eax) and address (ebx)
    call setup_gdt
    call write_tss_descriptor
    call setup_tss
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






;--------------------
;   TSS Setup
;--------------------

setup_tss:
    ; clear the TSS
    mov edi, TSS32
    mov ecx, 104/4        ; size of 32-bit TSS / 4
    xor eax, eax
    rep stosd

    ; set ring0 stack
    mov dword [TSS32 + 4], KERNEL_STACK_TOP ; esp0
    mov word  [TSS32 + 8], DATA_SEG         ; ss0

    ; load TR
    mov ax, TSS_SEG
    ltr ax
    ret


; 
; "AI assisted code" for write_tss_descriptor
;

write_tss_descriptor:
    lea edi, [tss_descriptor_slot]

    ; eax <- base (address of TSS32)
    mov eax, TSS32

    ; ecx <- limit (0x67)
    mov ecx, 0x0067
    mov [edi], cx            ; word [edi] = limit_low

    ; write base_low (word)
    mov [edi+2], ax          ; word [edi+2] = base & 0xFFFF

    ; base_mid = (base >> 16) & 0xFF
    mov ebx, eax
    shr ebx, 16
    mov byte [edi+4], bl     ; base_mid

    ; access byte = 0x89 (present + available 32-bit TSS)
    mov byte [edi+5], 0x89

    ; granularity/limit_high: we want limit_high= (limit >> 16) & 0x0F ; flags=0 -> whole byte 0x00
    mov byte [edi+6], 0x00

    ; base_high = (base >> 24) & 0xFF
    mov ecx, eax
    shr ecx, 24
    mov byte [edi+7], cl     ; base_high

    ret




;--------------------
;   GDT and TSS Data
;--------------------

GDT_Start:
    null_descriptor:
        dd 0
        dd 0
    code_descriptor:        ; 0x08
        dw 0xffff
        dw 0
        db 0
        db 0b10011010
        db 0b11001111
        db 0
    data_descriptor:        ; 0x10
        dw 0xffff
        dw 0
        db 0
        db 0b10010010
        db 0b11001111
        db 0
    user_code_descriptor:   ; 0x18
        dw 0xFFFF
        dw 0
        db 0
        db 0b11111010       
        db 0b11001111
        db 0
    user_data_descriptor:   ; 0x20
        dw 0xFFFF
        dw 0
        db 0
        db 0b11110010     
        db 0b11001111
        db 0
    tss_descriptor_slot:           ; 0x28
        times 8 db 0   

GDT_End:

GDT_Descriptor:
    dw GDT_End - GDT_Start -1
    dd GDT_Start




CODE_SEG        equ code_descriptor      - GDT_Start
DATA_SEG        equ data_descriptor      - GDT_Start
USER_CODE_SEG   equ user_code_descriptor - GDT_Start + 3 ; RPL=3
USER_DATA_SEG   equ user_data_descriptor - GDT_Start + 3 ; RPL=3
TSS_SEG         equ tss_descriptor_slot - GDT_Start




;--------------------
;   TSS + Kernel Stack
;--------------------

section .data 

align 16
TSS32:
    times 104 db 0       ; space for the 32-bit TSS

align 16
KernelStack:
    times 4096 db 0
KERNEL_STACK_TOP:

saved_kernel_sp:
    dd 0
; KERNEL_STACK_TOP equ KernelStack + 4096