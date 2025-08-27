; enter_user_mode(entry_point, user_stack)
global enter_user_mode
enter_user_mode:
    mov eax, [esp+4]    ; entry_point
    mov ebx, [esp+8]    ; user_stack (top)

    ; load user data selector into segments
    mov ax, 0x23        ; user data selector (RPL=3)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; push user stack segment
    push dword 0x23
    push ebx            ; user ESP

    ; push EFLAGS
    pushfd
    pop ecx
    or ecx, 0x200       ; ensure IF=1 (enable interrupts)
    push ecx

    ; push user code selector and entry point
    push dword 0x1B     ; user code selector (RPL=3)
    push eax            ; entry point (EIP)

    ; finally, jump with iret
    iretd
