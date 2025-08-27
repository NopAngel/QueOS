[bits 32]

global load_page_directory
global enable_paging

; void load_page_directory(uint32_t phys);
load_page_directory:
    mov eax, [esp + 4]
    mov cr3, eax
    ret


; void enable_paging()
enable_paging:
    mov eax, cr0
    or eax, 0x80000000  ; Set the PG (Paging) bit
    mov cr0, eax
    ret