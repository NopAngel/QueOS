; enter_user_mode(entry_point, user_stack)
global enter_user_mode
global switch_to_kernel

extern KERNEL_STACK_TOP
extern saved_kernel_sp

section .text

;void enter_user_mode(void* entry_point, void* user_stack);
enter_user_mode:
    cli                

    ; load user data selector into segments
    mov ax, 0x23        ; user data selector (RPL=3)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax


    mov eax, [esp+4]    ; entry_point
    mov ebx, [esp+8]    ; user_stack (top)

    pushad              
    pushfd     
    mov dword [saved_kernel_sp], esp  ; save current kernel stack pointer

    ; push user stack segment
    push dword 0x23
    push ebx            ; user ESP
    pushfd              ; push EFLAGS
    pop ecx
    or ecx, 0x200       ; ensure IF=1 (enable interrupts)
    push ecx            ; push modified EFLAGS
    push dword 0x1B     ; user code selector (RPL=3)
    push eax            ; entry point (EIP)

    iretd               ; switch to ring 3, user mode




switch_to_kernel:
    ; data segment (0x10 = 2nd GDT entry, RPL=0)
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Switch to kernel stack
    mov eax, [saved_kernel_sp]  
    mov esp, eax
    popfd
    popad                 

    sti 
    
    ret