[bits 32]

global _load_IDT    

;
; void _load_IDT(IDT_Register* idt_register);
; ---------------------------------------------
;
; the pointer param is passed on the stack (cdecl)
; [ebp+0] → old ebp (saved) [push ebp]
; [ebp+4] → return address
; [ebp+8] → first argument
; [ebp+12] → second argument (if any) (not used here)
;
; "lidt" loads the idt in the "IDTR" register.
;


_load_IDT:
    push ebp
    mov ebp, esp
               
    mov eax, [ebp + 8]
    lidt [eax]      

    mov esp, ebp
    pop ebp
    ret                     