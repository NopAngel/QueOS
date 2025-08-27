[BITS 32]
section .multiboot2
align 8

header_start:
    dd 0xE85250D6          ; Multiboot2 magic number
    dd 0                   ; Architecture (0 = i386)
    dd header_end - header_start ; Length
    dd 0x100000000 - (0xE85250D6 + 0 + (header_end - header_start)) ; Checksum

    ; --- Framebuffer Tag ---
    dw 5                   ; Tag type = 5 (Framebuffer)
    dw 0                   ; Reserved
    dd 24                  ; Size of this tag
    dd 800                 ; Desired X resolution
    dd 600                 ; Desired Y resolution
    dd 32                  ; Bits per pixel (24/32 for RGB)
    dd 0                   ; Padding to align to 8 bytes

    ; --- Memory Map Request Tag ---
    dw 6                   ; Tag type = 6 (Memory map request)
    dw 0                   ; Reserved
    dd 16                  ; Size of this tag (fixed for request)
    dd 0                   ; Reserved (MB2 spec says must be zero)
    dd 0                   ; Padding

    ; --- End Tag ---
    dw 0                   ; Tag type 0 = end
    dw 0                   ; Reserved
    dd 8                   ; Size
header_end: